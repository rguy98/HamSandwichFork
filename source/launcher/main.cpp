// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the example_sdl_opengl3/ folder**
// See imgui_impl_sdl.cpp for details.

#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <string>
#include <fstream>
#include <SDL.h>
#include <SDL_opengl.h>
#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl2.h>
#include <json.hpp>
#include <curl/curl.h>

#ifdef _MSC_VER
#include <filesystem>
namespace filesystem = std::filesystem;
#else
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem::v1;
#endif

#ifdef _WIN32
#include <direct.h>
#define platform_mkdir(path) _mkdir(path)
#else
#include <sys/stat.h>
#define platform_mkdir(path) mkdir(path, 0777)
#endif

struct Asset
{
	std::string filename;
	std::string sha256sum;
	std::string link;
	long file_id;
	std::string description;
	bool required;
	bool enabled;

	CURL* metadata_download = nullptr;
	std::string metadata_progress;
	CURL* content_download = nullptr;
	FILE* content_progress = nullptr;
	curl_off_t content_total_size;
	curl_off_t content_finished_size;

	static size_t metadata_write_callback(void* data, size_t size, size_t nmemb, Asset* self)
	{
		size_t total = size * nmemb;
		self->metadata_progress.append((char*)data, total);
		return total;
	}

	static int content_progress_callback(Asset* self, curl_off_t total_down, curl_off_t finished_down, curl_off_t, curl_off_t)
	{
		self->content_total_size = total_down;
		self->content_finished_size = finished_down;
		return 0;
	}

	void start_download(CURLM* downloads)
	{
		if (metadata_download || content_download)
			return;

		std::ostringstream full_url;
		full_url << link << "/file/" << file_id;

		metadata_progress = "";
		metadata_download = curl_easy_init();
		curl_easy_setopt(metadata_download, CURLOPT_URL, full_url.str().c_str());
		curl_easy_setopt(metadata_download, CURLOPT_POST, true);
		curl_easy_setopt(metadata_download, CURLOPT_POSTFIELDSIZE, 0);
		curl_easy_setopt(metadata_download, CURLOPT_WRITEFUNCTION, Asset::metadata_write_callback);
		curl_easy_setopt(metadata_download, CURLOPT_WRITEDATA, this);
		curl_multi_add_handle(downloads, metadata_download);
	}

	int transfer_completed(CURLM* downloads, CURL* transfer)
	{
		if (transfer == metadata_download)
		{
			metadata_download = nullptr;

			auto message = nlohmann::json::parse(metadata_progress);
			std::string url = message["url"];

			platform_mkdir("installers");
			std::string full_path = "installers/";
			full_path.append(filename);
			full_path.append(".part");
			content_progress = fopen(full_path.c_str(), "wb");  // TODO: error handling

			curl_easy_reset(transfer);
			content_download = transfer;
			curl_easy_setopt(content_download, CURLOPT_URL, url.c_str());
			//curl_easy_setopt(content_download, CURLOPT_BUFFERSIZE, 102400L);
			//curl_easy_setopt(content_download, CURLOPT_HTTP_VERSION, (long)CURL_HTTP_VERSION_2TLS);
			//curl_easy_setopt(content_download, CURLOPT_WRITEFUNCTION, fwrite);
			curl_easy_setopt(content_download, CURLOPT_WRITEDATA, content_progress);
			curl_easy_setopt(content_download, CURLOPT_NOPROGRESS, 0);
			curl_easy_setopt(content_download, CURLOPT_XFERINFOFUNCTION, Asset::content_progress_callback);
			curl_easy_setopt(content_download, CURLOPT_XFERINFODATA, this);

			curl_multi_add_handle(downloads, content_download);
			return 1;
		}
		else if (transfer == content_download)
		{
			fclose(content_progress);
			content_progress = nullptr;
			content_download = nullptr;

			std::string full_path = "installers/";
			full_path.append(filename);
			std::string part_path = full_path;
			part_path.append(".part");

			filesystem::rename(part_path, full_path);  // TODO: error handling
			printf("Finished: %s\n", filename.c_str());

			curl_easy_cleanup(transfer);
			return 0;
		}

		return 0;
	}
};

struct Game
{
	std::string id;
	std::string title;
	std::vector<Asset> assets;

	bool start_missing_downloads(CURLM* downloads)
	{
		bool any_missing = false;
		for (auto& asset : assets)
		{
			if (asset.required || asset.enabled)
			{
				std::string full_path = "installers/";
				full_path.append(asset.filename);
				if (filesystem::exists(full_path))
				{
					printf("OK: %s\n", asset.filename.c_str());
				}
				else
				{
					printf("Downloading: %s\n", asset.filename.c_str());
					asset.start_download(downloads);
					any_missing = true;
				}
			}
		}
		return any_missing;
	}

	bool is_ready_to_play() const
	{
		for (const auto& asset : assets)
		{
			if (asset.required || asset.enabled)
			{
				std::string full_path = "installers/";
				full_path.append(asset.filename);
				if (!filesystem::exists(full_path))
					return false;
			}
		}
		return true;
	}
};

namespace owned
{
	template<typename T, typename Deleter>
	struct convertible_unique_ptr : public std::unique_ptr<T, Deleter>
	{
		operator T*() { return this->get(); }
	};

	struct deleter_CURLM
	{
		void operator()(CURLM* ptr) { curl_multi_cleanup(ptr); }
	};

	typedef convertible_unique_ptr<::CURLM, deleter_CURLM> CURLM;
}

struct Launcher
{
	std::vector<Game> games;
	Game* current_game = nullptr;
	bool wants_to_play = false;
	bool wants_fullscreen = true;

	owned::CURLM downloads;

	Launcher()
	{
		downloads.reset(curl_multi_init());

		std::ifstream infile { "launcher.json" };
		nlohmann::json metadata;
		infile >> metadata;
		for (const auto& [key, value] : metadata.items())
		{
			games.push_back(Game
			{
				.id = key,
				.title = value["title"],
			});
			for (const auto& installer : value["installers"])
			{
				games.back().assets.push_back(Asset
				{
					.filename = installer["filename"],
					//.kind = installer["kind"],
					.sha256sum = installer["sha256sum"],
					.link = installer["link"],
					.file_id = installer["file_id"],
					.description = installer["description"],
					.required = !installer.contains("optional") || !installer["optional"],
				});
			}
		}

		if (!games.empty())
			current_game = &games[0];
	}

	// Returns number of ongoing transfers.
	int update_transfers()
	{
		int running_downloads;
		CURLMcode mc = curl_multi_perform(downloads, &running_downloads);
		if (mc == CURLM_OK)
		{
			int queued;
			while (CURLMsg* msg = curl_multi_info_read(downloads, &queued))
			{
				if (msg->msg == CURLMSG_DONE)
				{
					curl_multi_remove_handle(downloads, msg->easy_handle);
					for (auto& game : games)
					{
						for (auto& asset : game.assets)
						{
							running_downloads += asset.transfer_completed(downloads, msg->easy_handle);
						}
					}
				}
			}
		}
		return running_downloads;
	}
};

// ----------------------------------------------------------------------------
// Main code
enum class Action
{
	Gui,
	MiniCli,
	MiniGui,
};

int main(int argc, char** argv)
{
	const char* bin_dir = get_current_dir_name();  // never free()d because we always need it
	printf("bin_dir: %s\n", bin_dir);

	// Set up curl
	curl_global_init(CURL_GLOBAL_NOTHING);
	Action action = Action::Gui;
	Launcher launcher;

	for (int i = 1; i < argc; ++i)
	{
		std::string_view arg = argv[i];
		if (arg == "window")
		{
			launcher.wants_fullscreen = false;
		}
		else if (arg == "--mini-cli")
		{
			action = Action::MiniCli;
		}
		else if (arg == "--mini-gui")
		{
			action = Action::MiniGui;
		}
		else if (arg == "--all")
		{
			for (auto& game : launcher.games)
			{
				for (auto& asset : game.assets)
				{
					asset.enabled = true;
				}
			}
		}
		else
		{
			launcher.current_game = nullptr;
			for (auto& game : launcher.games)
			{
				if (arg == game.id)
				{
					launcher.current_game = &game;
				}
			}
		}
	}

	if (action == Action::MiniCli || action == Action::MiniGui)
	{
		if (!launcher.current_game)
		{
			fprintf(stderr, "Unknown game\n");
			return 1;
		}

		if (!launcher.current_game->start_missing_downloads(launcher.downloads))
		{
			return 0;
		}

		if (action == Action::MiniCli)
		{
			while (launcher.update_transfers());
			return 0;
		}
	}

	// Setup SDL
	// (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
	// depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// Setup window
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI);
	SDL_Window* window = SDL_CreateWindow("HamSandwich Launcher", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 480, window_flags);
	SDL_GLContext gl_context = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, gl_context);
	SDL_GL_SetSwapInterval(1); // Enable vsync

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// Setup Platform/Renderer backends
	ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
	ImGui_ImplOpenGL2_Init();

	// Load Fonts
	// - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
	// - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
	// - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
	// - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
	// - Read 'docs/FONTS.md' for more instructions and details.
	// - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
	//ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
	//IM_ASSERT(font != NULL);

	// Our state
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 0.00f);

	// Main loop
	bool done = false;
	while (!done)
	{
		int running_downloads = launcher.update_transfers();
		SDL_GL_SetSwapInterval(running_downloads ? 0 : 1); // Enable vsync iff nothing is downloading.

		// Poll and handle events (inputs, window resize, etc.)
		// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
		// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
		// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
		// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			ImGui_ImplSDL2_ProcessEvent(&event);
			if (event.type == SDL_QUIT)
				done = true;
			if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
				done = true;
		}

		int windowWidth, windowHeight;
		float leftPaneWidth = 300;
		SDL_GetWindowSize(window, &windowWidth, &windowHeight);

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL2_NewFrame();
		ImGui_ImplSDL2_NewFrame();
		ImGui::NewFrame();

		ImGui::SetNextWindowPos({ 0, 0 });
		ImGui::SetNextWindowSize({ leftPaneWidth, (float)windowHeight });
		if (action == Action::MiniGui)
			ImGui::BeginDisabled();
		if (ImGui::Begin("Games", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize))
		{
			for (auto& game : launcher.games)
			{
				ImGui::PushID(game.id.c_str());
				if (ImGui::Selectable("", launcher.current_game == &game, ImGuiSelectableFlags_AllowDoubleClick))
				{
					launcher.wants_to_play = false;
					launcher.current_game = &game;
					if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
					{
						launcher.wants_to_play = true;
						game.start_missing_downloads(launcher.downloads);
					}
				}
				ImGui::SameLine(0);
				//ImGui::Text("Icon");
				//ImGui::SameLine(64);
				ImGui::Text("%s", game.title.c_str());
				ImGui::PopID();
			}

			ImGui::Spacing();
			ImGui::Separator();

			ImGui::Text("Transfers: %d", running_downloads);
		}
		ImGui::End();

		if (launcher.current_game)
		{
			ImGui::SetNextWindowPos({ leftPaneWidth, 0 });
			ImGui::SetNextWindowSize({ (float)windowWidth - leftPaneWidth, (float)windowHeight });

			std::string window_title = launcher.current_game->title;
			window_title.append("###current_game");
			ImGui::Begin(window_title.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing);

			const char* message = launcher.wants_to_play ? "Downloading...###game_play"
				: launcher.current_game->is_ready_to_play() ? "Play###game_play"
				: "Download & Play###game_play";
			if (ImGui::Button(message, { 196, 0 }))
			{
				launcher.wants_to_play = !launcher.wants_to_play;
				if (launcher.wants_to_play)
					launcher.current_game->start_missing_downloads(launcher.downloads);
			}
			ImGui::SameLine(212);
			ImGui::Checkbox("Fullscreen", &launcher.wants_fullscreen);

			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Text("Official assets and add-ons:");

			for (auto& asset : launcher.current_game->assets)
			{
				ImGui::PushID(asset.filename.c_str());
				if (asset.required || launcher.wants_to_play)
				{
					bool dummy = asset.required || asset.enabled;
					ImGui::BeginDisabled();
					ImGui::Checkbox(asset.description.c_str(), &dummy);
					ImGui::EndDisabled();
				}
				else
				{
					ImGui::Checkbox(asset.description.c_str(), &asset.enabled);
				}

				ImGui::SameLine(ImGui::GetWindowWidth() - 128);

				std::string fullPath = "installers/";
				fullPath.append(asset.filename);
				if (filesystem::exists(fullPath))
				{
					if (asset.required)
					{
						ImGui::BeginDisabled();
						ImGui::Button("Ready###asset_download", { 128, 0 });
						ImGui::EndDisabled();
					}
					else
					{
						if (ImGui::Button("Delete###asset_download", { 128, 0 }))
						{
							asset.enabled = false;
							filesystem::remove(fullPath);
						}
					}
				}
				else
				{
					if (asset.metadata_download || asset.content_download)
					{
						std::ostringstream message;
						if (asset.content_total_size > 0)
						{
							int percent = asset.content_finished_size * 100 / asset.content_total_size;
							message << percent << "%" << "###asset_download";
						}
						else
						{
							message << "Starting...###asset_download";
						}

						if (ImGui::Button(message.str().c_str(), { 128, 0 }))
						{
							if (asset.metadata_download)
							{
								curl_multi_remove_handle(launcher.downloads, asset.metadata_download);
								curl_easy_cleanup(asset.metadata_download);
								asset.metadata_download = nullptr;
							}
							if (asset.content_download)
							{
								curl_multi_remove_handle(launcher.downloads, asset.content_download);
								curl_easy_cleanup(asset.content_download);
								asset.content_download = nullptr;
							}
						}
					}
					else
					{
						if (ImGui::Button("Download###asset_download", { 128, 0 }))
							asset.start_download(launcher.downloads);
					}
				}
				ImGui::PopID();
			}

			/*
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Text("Local add-ons:");
			ImGui::SameLine(ImGui::GetWindowWidth() - 256);
			if (ImGui::Button("Open folder", { 256, 0 }))
				;

			ImGui::Text("None");
			*/

			ImGui::End();
		}

		if (action == Action::MiniGui)
			ImGui::EndDisabled();
		// Rendering
		ImGui::Render();
		glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
		glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
		glClear(GL_COLOR_BUFFER_BIT);
		//glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
		ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
		SDL_GL_SwapWindow(window);

		if (launcher.current_game && launcher.current_game->is_ready_to_play())
		{
			if (action == Action::MiniGui)
			{
				break;
			}
			else if (launcher.wants_to_play)
			{
				launcher.wants_to_play = false;

				// Hide the window.
				SDL_HideWindow(window);

				std::string cmdline = "./";
				cmdline.append(launcher.current_game->id);
				if (!launcher.wants_fullscreen)
					cmdline.append(" window");
				// TODO: use exec or CreateProcess instead
				if (system(cmdline.c_str()))
				{
					// Child process failed, so bring the launcher back.
					SDL_ShowWindow(window);
				}
				else
				{
					// Success, so clean up and exit in the background.
					break;
				}
			}
		}
	}

	// Cleanup
	launcher.downloads.reset();
	curl_global_cleanup();

	ImGui_ImplOpenGL2_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	SDL_GL_DeleteContext(gl_context);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}

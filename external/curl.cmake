if(APPLE)
	find_library(curl_LIBRARIES curl REQUIRED)
	add_library(libcurl INTERFACE)
	target_link_libraries(libcurl INTERFACE ${curl_LIBRARIES})
else()
	set(HTTP_ONLY ON)
	set(CURL_USE_LIBSSH2 OFF)
	if(WIN32)
		set(CURL_USE_SCHANNEL ON)
	endif()
	add_subdirectory("curl" EXCLUDE_FROM_ALL)
	install(FILES "${CMAKE_CURRENT_SOURCE_DIR}/curl/COPYING" TYPE BIN COMPONENT Executables RENAME "LICENSE.libcurl.txt")
endif()

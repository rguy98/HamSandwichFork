# build-binary-package.ps1
# Arguments / configuration
param([string] $configuration = "release")

$projects = $args

$platform = "x86"
$vs_platform = "Win32"
$sdl_platform = "x86"

# Script
$ErrorActionPreference = "Stop"
Add-Type -AssemblyName System.IO.Compression.FileSystem

# Install dependencies (premake, SDL) and load the $ExeProjects metadata
powershell -NoLogo -ExecutionPolicy Bypass -File  ./tools/msvc/install-dependencies.ps1
./build/premake5.exe binary-package-info
. ./build/binary-package-info.ps1

if ($projects.Length -eq 0) {
	$projects = "mystic", "supreme", "sleepless", "loonyland", "loonyland2"
}

# Invoke vcvars32.bat to get msbuild access
cmd.exe /c 'call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat" >NUL && set' `
| ForEach-Object {
	if ($_ -match "^(.*?)=(.*)$") {
		Set-Content "env:\$($matches[1])" $matches[2]
	}
}

# Install dependencies, generate the solution, and build it
Write-Output "==== Compiling ===="
./build/premake5.exe vs2019
foreach ($project in $projects) {
	msbuild ./build/msc-v142/$project.vcxproj /p:Configuration=$configuration /p:Platform=$vs_platform
}

# Create build/pkgroot
Write-Output "==== Preparing package root ===="
$pkgroot = "$PWD/build/pkgroot"
New-Item build -ItemType Directory -ErrorAction SilentlyContinue > $null
Remove-Item -Recurse $pkgroot -ErrorAction SilentlyContinue
New-Item $pkgroot -ItemType Directory > $null

# Include the project EXEs
foreach ($project in $projects) {
	Copy-Item ./build/msc-v142/$configuration-$platform/$project/$project.exe $pkgroot
}
# Include SDL2 DLLs
Copy-Item -Destination $pkgroot -Path (
	"./build/SDL2-msvc/lib/$sdl_platform/SDL2.dll",
	"./build/SDL2_image-msvc/lib/$sdl_platform/SDL2_image.dll",
	"./build/SDL2_mixer-msvc/lib/$sdl_platform/SDL2_mixer.dll",
	"./build/SDL2_mixer-msvc/lib/$sdl_platform/libogg-0.dll",
	"./build/SDL2_mixer-msvc/lib/$sdl_platform/libvorbis-0.dll",
	"./build/SDL2_mixer-msvc/lib/$sdl_platform/libvorbisfile-3.dll"
)

# Collate installers and build installers/README.txt
$installers_by_link = @{}
foreach ($project in $projects) {
	$info = $ExeProjects["$project|${configuration}_$platform"]
	foreach ($installer in $info["installers"]) {
		$link = $installer["link"]
		if (!$installers_by_link[$link]) {
			$installers_by_link[$link] = @()
		}
		$installers_by_link[$link] += $installer["filename"]
	}

	# Copy while we're here
	$asset_destination = "$pkgroot/assets/$($info["appdata_folder_name"])"
	[array]::Reverse($info["assetdirs"])  # Earlier in the list overwrites later
	foreach ($assetdir in $info["assetdirs"]) {
        Write-Output "==== Copying assets from $assetdir ===="
		New-Item $asset_destination -ItemType Directory > $null
		Copy-Item -Recurse $assetdir/* $asset_destination
	}
}

New-Item $pkgroot/installers -ItemType Directory > $null
$readme = "$pkgroot/installers/README.txt"
@"
From each link below, download the named installers and save them here:

"@ | Out-File -Encoding utf8 $readme

foreach ($link in $installers_by_link.Keys) {
	Write-Output "$link" | Out-File -Encoding utf8 -Append $readme
	foreach ($filename in $installers_by_link[$link]) {
		Write-Output "    $filename" | Out-File -Encoding utf8 -Append $readme
	}
	Write-Output "" | Out-File -Encoding utf8 -Append $readme
}

# Zip it up
Write-Output "==== Zipping ===="
$zip = "$PWD/build/HamSandwich-windows.zip"
Push-Location $pkgroot
7z a $zip .
Pop-Location

param (
    [string]$BoostRoot
)

# Check if $BoostRoot is null or empty, and if $BOOST_ROOT is not already defined
if ([string]::IsNullOrEmpty($BoostRoot) -and [string]::IsNullOrEmpty($env:BOOST_ROOT)) {
    Write-Error "Please provide the Boost root directory path with the -BoostRoot parameter, or set the BOOST_ROOT environment variable."
    Exit 1
}

# Set the BOOST_ROOT environment variable if $BoostRoot is not null or empty
if (![string]::IsNullOrEmpty($BoostRoot)) {
    $env:BOOST_ROOT = $BoostRoot
}

set BOOST_ROOT=$BoostRoot

# Build client

New-Item -ItemType Directory -Path .\client\build -Force

Push-Location -Path .\client\build

cmake ..
cmake --build .

# Get the path of the built executable
$ClientExecutable = Get-ChildItem -Path . -Recurse -Include "client.exe" | Select-Object -ExpandProperty FullName

Pop-Location

# Build server

New-Item -ItemType Directory -Path .\server\build -Force

Push-Location -Path .\server\build

cmake ..
cmake --build .

# Get the path of the built executable
$ServerExecutable = Get-ChildItem -Path . -Recurse -Include "server.exe" | Select-Object -ExpandProperty FullName

Pop-Location

# Put client and server executables in the global build/ folder

New-Item -ItemType Directory -Path ./build -Force

Copy-Item -Path $ClientExecutable -Destination ./build
Copy-Item -Path $ServerExecutable -Destination ./build

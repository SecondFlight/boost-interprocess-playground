# Set the script's location as the current directory
$scriptPath = Split-Path -Parent $MyInvocation.MyCommand.Path
Push-Location $scriptPath

# Build flatbuffers file

New-Item -ItemType Directory -Path .\generated -Force
flatc --cpp -o ./generated/ ../messages/messages.fbs

# Build

New-Item -ItemType Directory -Path .\build -Force

Push-Location -Path .\build

cmake ..
cmake --build .

Pop-Location

Pop-Location

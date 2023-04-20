# Generate Dart code
./bindgen.ps1

# Create assets directory if it doesn't exist
New-Item -ItemType Directory -Path .\assets -Force

# Build engine
.\engine\build.ps1
Copy-Item -Path ".\engine\build\Debug\engine_demo.exe" -Destination ".\assets\Engine.exe"

# Build engine connector
.\engine_connector\build.ps1
Copy-Item -Path ".\engine_connector\build\Debug\EngineConnector.dll" -Destination ".\assets\EngineConnector.dll"

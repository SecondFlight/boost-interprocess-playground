.\engine_connector\build.ps1
New-Item -ItemType Directory -Path .\assets -Force
Copy-Item -Path ".\engine_connector\build\Debug\EngineConnector.dll" -Destination ".\assets\EngineConnector.dll"

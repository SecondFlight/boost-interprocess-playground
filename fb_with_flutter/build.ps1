.\engine\flutter_connector\build.ps1
New-Item -ItemType Directory -Path .\assets -Force
Copy-Item -Path ".\engine\flutter_connector\build\Debug\FlutterEngineConnector.dll" -Destination ".\assets\FlutterEngineConnector.dll"

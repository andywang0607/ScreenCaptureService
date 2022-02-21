# ScreenCaptureService
A service for capture screen image or video, provide api for client to query screen image or stream from zmq server


## How to build
- Support platform: Windows
- Compiler: visual studio 2017 (I develop with 2017 but 2019 and 2022 maybe still work)
  - If you build with other visual studio version, you need to modify CMAKE_GENERATOR_PLATFORM variable [build script](script/build.bat)
- Build Step
  ```
  # Clone this repo and related submodule
  git clone https://github.com/andywang0607/ScreenCaptureService.git --recursive

  # build with script
  cd script
  build.bat Release
  ```
- Everything will be installed at bin folder

## How to use

### Server
- Just need to set ip and port

### Client
- [API header](client/include/ScreenCaptureApi.h)
- [Example](client/demo/main.cpp)

### Run Demo
- Build server and client
- set ip and port at server_config.json in bin folder
- Run ScreenCaptureServer.exe
- Run clientDemo.exe




# BoolkaLoader
Cross-platform Geometry Dash ModLoader

# Requirements
- CMake 3.21 or later
- Android NDK
- LLVM
- Ninja
- Java and ApkTool

# Building
- For Windows: `rd /s /q build & cmake -S . -B build -GNinja -DCMAKE_BUILD_TYPE=Release`
- For Android: `rd /s /q build & cmake -S . -B build -GNinja -DCMAKE_TOOLCHAIN_FILE="your-ndk-root\build\cmake\android.toolchain.cmake" -DANDROID_ABI=armeabi-v7a -DCMAKE_BUILD_TYPE=Release`
- `cmake --build build`
- You can use `-DCMAKE_BUILD_TYPE=RelWithDebInfo` for debugging

# Installing on Windows
- Download and unzip BoolkaLoader from [Releases](https://github.com/BoolkaSDK/Loader/releases)
- Put `boolkaloader.dll` and `version.dll` to Geometry Dash directory

# Installing on Android
- Download and unzip BoolkaLoader from [Releases](https://github.com/BoolkaSDK/Loader/releases)
- Decode Geometry Dash apk with `apktool d GeometryDash.apk`
- Open `GeometryDash/smali/com/robtopx/geometryjump/GeometryJump.smali`
- Go to line 19 and after `invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V` add this
```
.line 75
const-string v0, "boolkaloader"

invoke-static {v0}, Ljava/lang/System;->loadLibrary(Ljava/lang/String;)V
```
- Copy `libboolkaloader.so` to `GeometryDash/lib/armeabi-v7a`
- `apktool b GeometryDash`
- Sign APK (idk how :smirk_cat:)

# Loading mods
- On Windows: put mod DLL in `boolkamods` in Geometry Dash directory
- On Android: put mod DLL in `/sdcard/BoolkaMods/com.package.name` directory
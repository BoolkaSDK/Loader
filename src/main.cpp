#if defined(WIN32)

#include <windows.h>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

typedef int(__cdecl* BoolkaInit)();
#define ENTRYCALL __stdcall

#elif defined(ANDROID)

#include <filesystem>
#include <iostream>
#include <sstream>
#include <string>

#include <android/log.h>
#include <dlfcn.h>
#include <jni.h>
#include <unistd.h>

#define ENTRYCALL

#endif

bool IsLibrary(std::string path)
{
    std::string str;
    std::stringstream stream(path);

    while (std::getline(stream, str, '.'))
        ;
#if defined(WIN32)
    return str == "dll";
#elif defined(ANDROID)
    return str == "so";
#endif
}

void ENTRYCALL Main()
{
#if defined(WIN32)
    std::string boolkamodsPath = "boolkamods";
#elif defined(ANDROID)
    pid_t pid = getpid();
    char path[64] = {0};
    sprintf(path, "/proc/%d/cmdline", pid);
    FILE* cmdline = fopen(path, "r");

    if (!cmdline)
        return;

    char application_id[64] = {0};
    fread(application_id, sizeof(application_id), 1, cmdline);
    fclose(cmdline);

    std::string boolkamodsPath = "/sdcard/BoolkaMods/" + std::string(application_id) + "";
    std::string gamePath = "/data/data/" + std::string(application_id) + "/files";

    // because android secure policy doesn't allow load .so from sdcard
    if (!std::filesystem::is_directory(gamePath + "/BoolkaMods") || !std::filesystem::exists(gamePath + "/BoolkaMods"))
        std::filesystem::create_directories(gamePath + "/BoolkaMods");
#endif

    if (!std::filesystem::is_directory(boolkamodsPath) || !std::filesystem::exists(boolkamodsPath))
        std::filesystem::create_directories(boolkamodsPath);

    for (std::filesystem::directory_entry file : std::filesystem::directory_iterator(boolkamodsPath))
    {
        std::string filepath = file.path().string();
        std::string filename = file.path().filename().string();

        if (IsLibrary(filename))
        {
#if defined(WIN32)
            LoadLibrary(filepath.c_str());
#elif defined(ANDROID)
            std::filesystem::copy_file(filepath, gamePath + "/BoolkaMods" + filename);

            dlopen((gamePath + "/BoolkaMods" + filename).c_str(), RTLD_LAZY);
#endif
        }
    }
}

#if defined(WIN32)
BOOL APIENTRY DllMain(HMODULE hModule, DWORD dwReason, LPVOID lpReserved)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        CreateThread(0, 0x1000, (LPTHREAD_START_ROUTINE)Main, hModule, 0, 0);
    }

    return true;
}
#endif

#ifdef ANDROID
__attribute__((constructor)) void BoolkaInit()
{
    Main();
}
#endif
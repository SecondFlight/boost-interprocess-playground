#include <iostream>

extern "C"
{
    __declspec(dllexport) void __stdcall test()
    {
        std::cout << "hi" << std::endl;
    }
}

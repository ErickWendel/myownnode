#include "v8.h"

#include "./src/capivara.hpp"

int main(int argc, char *argv[])
{
    char *filename = argv[1];
    auto *capivara = new Capivara();
    std::unique_ptr<v8::Platform> platform =
        capivara->initializeV8(argc, argv);

    capivara->initializeVM();
    capivara->InitializeProgram(filename);
    capivara->Shutdown();

    return 0;
}
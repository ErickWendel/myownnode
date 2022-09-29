#include <libplatform/libplatform.h>
#include <uv.h>
#include "v8.h"

#include "./src/myownnode.hpp"


int main(int argc, char *argv[])
{
    char *filename = argv[1];
    MyOwnNode *myOwnNode = new MyOwnNode();
    std::unique_ptr<v8::Platform> platform =
        myOwnNode->initializeV8(argc, argv);

    myOwnNode->initializeVM();
    myOwnNode->InitializeProgram(filename);
    myOwnNode->Shutdown();

    return 0;
}
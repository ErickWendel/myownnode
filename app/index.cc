#include "./myownnode.hpp"
#include "v8.h"

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
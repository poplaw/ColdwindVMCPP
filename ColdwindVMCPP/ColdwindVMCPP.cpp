#include <iostream>
#include "Instance.h"

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cout << "Usage: vm.py <filename>\n";
        return 0;
    }

    VM::Instance instance;
    instance.loadMemoryFromFile(0, argv[1]);
    instance.run();

    return 0;
}

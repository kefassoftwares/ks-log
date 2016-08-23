#include <iostream>
#include "Merge.h"

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        std::string logDir(argv[1]);
        KMergeLogs::Merge merge(logDir);
        merge.start();
        std::cin >> argc;
        return 0;
    }

    std::cout << "Usage: Merge <foldername>" << std::endl;
    return 0;
}

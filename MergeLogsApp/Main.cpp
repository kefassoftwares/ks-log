#include <iostream>
#include "Merge.h"

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        std::string logDir(argv[1]);
        std::string logFormat((argc > 2) ? argv[2] : "");
        KMergeLogs::Merge merge(logDir, logFormat);
        merge.start();
    }
}

#include <iostream>
#include "Merge.h"

int main(int argc, char *argv[])
{
    if (argc > 1)
    {
        std::string logDir(argv[1]);
        std::string dateFormat((argc > 2) ? argv[2] : "");
        KMergeLogs::Merge merge(logDir, dateFormat);
        merge.start();
    }
}

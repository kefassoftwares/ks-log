#include <iostream>
#include <thread>
#include "Log.h"

void threadfun()
{
    static int i = 0;
    LOG_CRITICAL("This is another thread. This should be in other file thread:" );
}

int main()
{
    if (!klog::Log::getInstance().init(".", "ChatApp", klog::Severity::CRITICAL))
    {
        std::cout << "Oops..!!" << std::endl;
    }

    LOG_ERROR("The Developer Needs to test the output!!");
    std::vector<std::thread> tv;
    for (int i = 0; i < 6; i++)
    {
        tv.push_back(std::thread(threadfun));
    }

    for (auto it = tv.begin(); it != tv.end(); ++it)
    {
        it->join();
    }
}

//Linux:
//g++ -g -std=c++11 Log.cpp Main.cpp -lboost_system -lboost_date_time -lboost_filesystem -lpthread
//Windows
//cl /EHsc /I"C:\Program Files\boost\boost_1_59_0" Main.cpp Log.cpp RealtimeTokenParser.cpp /MD /DYNAMICBASE: "C:\Program Files\boost\boost_1_59_0\stage\lib\libboost_system-vc140-mt-1_59.lib" "C:\Program Files\boost\boost_1_59_0\stage\lib\libboost_date_time-vc140-mt-1_59.lib" "C:\Program Files\boost\boost_1_59_0\stage\lib\libboost_filesystem-vc140-mt-1_59.lib"

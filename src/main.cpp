//
// Created by Bradley Remedios on 11/20/24.
//
#include <iostream>

#include "App.h"
#include "bpl/sys/Stopwatch.h"

bpl::sys::Stopwatch g_shutdownStopwatch;

int main (int argc, char**argv) {
    App* app = new App();

    if (!app->Create()) {
        std::cerr << "Failed to create application" << std::endl;
        return -1;
    }

    app->Run();

    app->Destroy();

    delete app;

    g_shutdownStopwatch.Stop();

    std::cout << "Shutdown took " << g_shutdownStopwatch.getMilliseconds() << " ms" << std::endl;
    return 0;
} // main
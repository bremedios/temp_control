//
// Created by Bradley Remedios on 11/20/24.
//
#include <iostream>

#include "App.h"

int main (int argc, char**argv) {
    App app;

    if (!app.Create()) {
        std::cerr << "Failed to create application" << std::endl;
        return -1;
    }

    app.Run();

    app.Destroy();

    return 0;
} // main
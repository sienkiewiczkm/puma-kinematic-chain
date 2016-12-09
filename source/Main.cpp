#include <cstdio>
#include <iostream>
#include "easylogging++.h"
#include "fw/Framework.hpp"
#include "Application.hpp"

int main(int argc, const char* argv[])
{
    fw::initialize(argc, argv);
    LOG(INFO) << "Starting application";

    application::Application app;
    app.create();
    app.run();
    app.destroy();

    return EXIT_SUCCESS;
}

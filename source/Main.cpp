#include <cstdio>
#include <iostream>
#include "easylogging++.h"
#include "fw/Framework.hpp"
#include "Application.hpp"

int main(int argc, const char* argv[])
{
    fw::initialize(argc, argv);
    LOG(INFO) << "Starting application";

    auto app = std::make_shared<application::Application>();
    app->create();
    app->run();
    app->destroy();
    app = nullptr;

    return EXIT_SUCCESS;
}

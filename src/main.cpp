#include "App/App.hpp"

#define UNUSED(x) ((void)(x))

int main(int argc, char** argv)
{
    UNUSED(argc);
    UNUSED(argv);

    App app {};
    app.run();
    return 0;
}
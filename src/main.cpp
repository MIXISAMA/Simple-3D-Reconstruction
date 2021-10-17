#include "app/application.h"
#include "ui/main_window.h"

int main(int argc, const char** argv)
{
    mixi::app::Application app(argv[0]);
    mixi::s3r::MainWindow window;
    app.loop(&window);
    return 0;
}

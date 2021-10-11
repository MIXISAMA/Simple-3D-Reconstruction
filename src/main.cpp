#include "app/my_application.h"
#include "ui/main_window.h"

int main(int argc, char** argv)
{
    MainWindow mainWindow;
    MyApplication app(argv[0], mainWindow);
    LOG(INFO) << "Application Started";
    app.loop();
    LOG(INFO) << "Application Stopped";
    return 0;
}

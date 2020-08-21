#include "neteasecloudfastplaygui.h"
#include "stable.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NeteaseCloudFastPlayGUI w;
    w.show();
    return a.exec();
}

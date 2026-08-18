#include <iostream>

#include "mainwindow.h"

#include <QApplication>
#include <QRegularExpression>
#include <QSize>
#include <QWidget>

namespace
{
bool parseResolution(const std::string& value, QSize& resolution)
{
    static const QRegularExpression resolutionPattern(QStringLiteral("^(\\d+)x(\\d+)$"));
    const QRegularExpressionMatch match = resolutionPattern.match(QString::fromStdString(value));
    if(!match.hasMatch()) {
        return false;
    }

    bool widthOk = false;
    bool heightOk = false;
    const int width = match.captured(1).toInt(&widthOk);
    const int height = match.captured(2).toInt(&heightOk);
    if(!widthOk || !heightOk || width <= 0 || height <= 0) {
        return false;
    }

    resolution = QSize(width, height);
    return true;
}
}

int main(int argc, char *argv[])
{
    const std::string help_str =
            "Ошибка. Используемые параметры ввода:"
            "\n\t--port имя порта (например ttyS4)"
            "\n\t--i путь и имя файла json (информация по весам)"
            "\n\t--resolution <ширина>x<высота> (например 1280x720)";

    std::string port_name;
    std::string file_name;
    QSize resolution;
    bool hasResolution = false;

    QApplication app(argc, argv);

    if(argc > 1)
    {
        if((argc - 1) % 2 != 0) {
            std::cout << help_str << std::endl;
            return -1;
        }

        for(int shift = 1; shift < argc; shift += 2)
        {
            if(std::string(argv[shift]) == "--port") {
                port_name = std::string(argv[shift + 1]);
            } else if(std::string(argv[shift]) == "--i") {
                file_name = std::string(argv[shift + 1]);
            } else if(std::string(argv[shift]) == "--resolution") {
                if(!parseResolution(argv[shift + 1], resolution)) {
                    std::cout << help_str << std::endl;
                    return -1;
                }
                hasResolution = true;
            } else {
                std::cout << help_str << std::endl;
                return -1;
            }
        }
    }

    if(!DEBUG_VERBOSE) {
        //no cout prints
        std::cout.rdbuf( nullptr );
    }

    MainWindow mainWindow(port_name, !hasResolution);
    //mainWindow.setVisible(false);
    if(!file_name.empty()) {
        mainWindow.setJsonFilename(file_name);
    }

    if(hasResolution) {
        mainWindow.resize(resolution);
        mainWindow.show();
    } else {
        mainWindow.showFullScreen();
    }

    return app.exec();
}

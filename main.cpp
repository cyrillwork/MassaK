#include <iostream>

#include "mainwindow.h"

#include <QApplication>
#include <QGuiApplication>
#include <QPoint>
#include <QRegularExpression>
#include <QScreen>
#include <QSize>
#include <QWindow>
#include <QWidget>

namespace
{
struct WindowGeometry
{
    QSize size;
    QPoint position;
    bool hasPosition = false;
};

bool parseGeometry(const std::string& value, WindowGeometry& geometry)
{
    static const QRegularExpression geometryPattern(
            QStringLiteral("^=?(\\d+)[xX](\\d+)(?:([+-]\\d+)([+-]\\d+))?$"));
    const QRegularExpressionMatch match = geometryPattern.match(QString::fromStdString(value));
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

    geometry.size = QSize(width, height);
    geometry.hasPosition = !match.captured(3).isEmpty();
    if(geometry.hasPosition) {
        bool xOk = false;
        bool yOk = false;
        const int x = match.captured(3).toInt(&xOk);
        const int y = match.captured(4).toInt(&yOk);
        if(!xOk || !yOk) {
            return false;
        }
        geometry.position = QPoint(x, y);
    }

    return true;
}
}

int main(int argc, char *argv[])
{
    const std::string help_str =
            "Ошибка. Используемые параметры ввода:"
            "\n\t--port имя порта (например ttyS4)"
            "\n\t--i путь и имя файла json (информация по весам)"
            "\n\t--geometry <ширина>x<высота>[+X+Y] (например 1280x720+0+0)"
            "\n\t--screen <номер экрана, начиная с 0> (например 1)";

    std::string port_name;
    std::string file_name;
    WindowGeometry geometry;
    bool hasGeometry = false;
    int screenNumber = -1;
    bool hasScreen = false;

    // The XCB platform plugin consumes the standard --geometry option while
    // QApplication is being constructed. Parse application arguments first
    // so that the requested windowed mode is not mistaken for fullscreen.
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
            } else if(std::string(argv[shift]) == "--geometry") {
                if(!parseGeometry(argv[shift + 1], geometry)) {
                    std::cout << help_str << std::endl;
                    return -1;
                }
                hasGeometry = true;
            } else if(std::string(argv[shift]) == "--screen") {
                bool screenOk = false;
                screenNumber = QString::fromLocal8Bit(argv[shift + 1]).toInt(&screenOk);
                if(!screenOk || screenNumber < 0) {
                    std::cout << help_str << std::endl;
                    return -1;
                }
                hasScreen = true;
            } else {
                std::cout << help_str << std::endl;
                return -1;
            }
        }
    }

    QApplication app(argc, argv);

    QScreen *targetScreen = nullptr;
    if(hasScreen) {
        const QList<QScreen *> screens = QGuiApplication::screens();
        if(screenNumber >= screens.size()) {
            std::cout << "Ошибка: экран с номером " << screenNumber
                      << " не найден. Доступно экранов: " << screens.size() << std::endl;
            return -1;
        }
        targetScreen = screens.at(screenNumber);
    }

    if(!DEBUG_VERBOSE) {
        //no cout prints
        std::cout.rdbuf( nullptr );
    }

    MainWindow mainWindow(port_name, !hasGeometry);
    //mainWindow.setVisible(false);
    if(!file_name.empty()) {
        mainWindow.setJsonFilename(file_name);
    }

    if(targetScreen) {
        mainWindow.winId();
        if(mainWindow.windowHandle()) {
            mainWindow.windowHandle()->setScreen(targetScreen);
        }
    }

    if(hasGeometry) {
        mainWindow.resize(geometry.size);
        if(geometry.hasPosition) {
            const QPoint screenOrigin = targetScreen
                    ? targetScreen->geometry().topLeft()
                    : QPoint();
            mainWindow.move(screenOrigin + geometry.position);
        } else if(targetScreen) {
            mainWindow.move(targetScreen->availableGeometry().center()
                            - mainWindow.rect().center());
        }
        mainWindow.showNormal();
    } else {
        if(targetScreen) {
            mainWindow.move(targetScreen->geometry().topLeft());
        }
        mainWindow.showFullScreen();
    }

    return app.exec();
}

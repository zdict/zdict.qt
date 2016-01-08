#include <QApplication>
#include <QMainWindow>
#include <QCursor>          // for cursor position
#include <QClipboard>       // for buffers change signals
#include <QSettings>        // for INI settings
#include <QStandardPaths>   // for system config path
#include <QtDebug>

#include <qtermwidget.h>    // for terminal support (we need VT100)

////////////////////////////////////////
// Todo
//
// * user config and reload
//     - set "zdict" path
//     - set font
//     - set window size
//     - set shortcut
//     - set detect buffer (default is Clipboard)
//     - ...
// * zdict's word display on the top of popup window
// * close zdict in popup window means close popup window
//     - hook signal ? (SIGINT/SIGTSTP/SIGQUIT/...)
// * [optional] disable keyboard input in popup window
// * [optional] applet on the top
//
////////////////////////////////////////

int main(int argc, char *argv[]) {

    QApplication app(argc, argv);
    auto *mainWindow = new QMainWindow();

    auto *console = new QTermWidget();  // terminal
    console->sendText("zdict\n");       // start zdict

    mainWindow->setWindowFlags(Qt::FramelessWindowHint | Qt::Popup);    // popup window

    ////////////////////////////////////////
    // Load Settings
    ////////////////////////////////////////

    const auto configPath = QStandardPaths::standardLocations(QStandardPaths::GenericConfigLocation)[0]
                            + "/zdict.qt/config.ini";
    QSettings settings(configPath, QSettings::IniFormat);

    ////////////////////////////////////////
    // Font
    ////////////////////////////////////////

    auto font = QApplication::font();

    const auto fontSize = settings.value("Font/Size", 14).toUInt();
    const auto family   = settings.value("Font/Family", "San Serif").toString();

    font.setPointSize(fontSize);
    font.setFamily(family);

    console->setTerminalFont(font);

    ////////////////////////////////////////
    // Theme
    ////////////////////////////////////////

    const auto colorscheme = settings.value("Theme/ColorScheme", "GreenOnBlack").toString();
    console->setColorScheme(colorscheme);
    console->setScrollBarPosition(QTermWidget::ScrollBarRight);

    ////////////////////////////////////////
    // Window Setting
    ////////////////////////////////////////

    const auto windowSizeX = settings.value("Window/SizeX", 600).toInt();
    const auto windowSizeY = settings.value("Window/SizeY", 400).toInt();
    const QSize windowSize(windowSizeX, windowSizeY);

    mainWindow->move(QCursor::pos());
    mainWindow->setCentralWidget(console);
    mainWindow->resize(windowSize);

    ////////////////////////////////////////
    // Information
    ////////////////////////////////////////

    qDebug() << "========== Information ==========";
    qDebug() << "availableKeyBindings:" << console->availableKeyBindings();
    qDebug() << "keyBindings:" << console->keyBindings();
    qDebug() << "availableColorSchemes:" << console->availableColorSchemes();
    qDebug() << "Font:" << console->getTerminalFont();
    qDebug() << "=================================";

    ////////////////////////////////////////
    // Signal & Slots
    ////////////////////////////////////////

    const auto buffer = settings.value("Buffer/Buffer", "Selection").toString();
    QClipboard::Mode mode = QClipboard::Selection;

    if (buffer == "Clipboard") {
        mode = QClipboard::Clipboard;
    }

    const auto *clipboard = QApplication::clipboard();

    const auto show_and_query = [=] () {
            mainWindow->move(QCursor::pos());
            mainWindow->show();
            const auto word = clipboard->text(mode);
            console->sendText(word + "\n");
        };

    QObject::connect(clipboard, &QClipboard::dataChanged, show_and_query);
    QObject::connect(console, SIGNAL(finished()), mainWindow, SLOT(close()));

    return app.exec();
}

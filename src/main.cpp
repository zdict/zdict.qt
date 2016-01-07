#include <QApplication>
#include <QMainWindow>
#include <QCursor>          // for cursor position
#include <QClipboard>       // for buffers change signals
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
    // Font
    ////////////////////////////////////////

    auto font = QApplication::font();
    font.setPointSize(14);
    console->setTerminalFont(font);

    ////////////////////////////////////////
    // Theme
    ////////////////////////////////////////

    console->setColorScheme("GreenOnBlack");
    console->setScrollBarPosition(QTermWidget::ScrollBarRight);

    ////////////////////////////////////////
    // Window Setting
    ////////////////////////////////////////

    mainWindow->move(QCursor::pos());
    mainWindow->setCentralWidget(console);
    mainWindow->resize(600, 400);

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

    const auto *clipboard = QApplication::clipboard();

    const auto show_and_query = [=] () {
            mainWindow->move(QCursor::pos());
            mainWindow->show();
            const auto word = clipboard->text(clipboard->Selection);
            console->sendText(word + "\n");
        };

    QObject::connect(clipboard, &QClipboard::dataChanged, show_and_query);
    QObject::connect(console, SIGNAL(finished()), mainWindow, SLOT(close()));

    return app.exec();
}

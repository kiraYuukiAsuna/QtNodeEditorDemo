#include <QFontDatabase>
#include "QtAdvancedStylesheet.h"
#include "mainwindow.h"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    QString fontPath = QString(
        R"(C:\Users\kiraYuukiAsuna\Desktop\GoMongoDb\src\SwcManagerClient\Resource\fonts\SourceHanSansCN\SourceHanSansCN-Regular.ttf)");
    int loadedFontID = QFontDatabase::addApplicationFont(fontPath);
    QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
    if (!loadedFontFamilies.empty()) {
        const QString&sansCNFamily = loadedFontFamilies.at(0);
        QFont defaultFont = QApplication::font();
        defaultFont.setFamily(sansCNFamily);
        defaultFont.setPixelSize(14);
        QApplication::setFont(defaultFont);
    }

    QString appDir = qApp->applicationDirPath();
    acss::QtAdvancedStylesheet styleManager;
    styleManager.setStylesDirPath(R"(C:\Users\kiraYuukiAsuna\Desktop\GoMongoDb\src\SwcManagerClient\Resource\styles)");
    styleManager.setOutputDirPath(appDir + "/StylesOutput");
    styleManager.setCurrentStyle("qt_material_modified");
    styleManager.setCurrentTheme("light_blue");
    styleManager.updateStylesheet();
    qApp->setStyleSheet(styleManager.styleSheet());

    MainWindow mainWindow{};
    mainWindow.show();

    return QApplication::exec();
}

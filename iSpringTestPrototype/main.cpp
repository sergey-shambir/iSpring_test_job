#include <QtWidgets/QApplication>
#include <QtWidgets/QStyleFactory>
#include <QQuickView>
#include <QtQml/qqml.h>
#include <QQmlContext>
#include "shapessceneview.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create("Fusion"));
    qmlRegisterType<ShapesSceneView>("name.sshambir.ispringexam", 1, 0, "ShapesSceneView");

    QQuickView view;
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.setMinimumSize(QSize(800, 720));
    view.rootContext()->setContextProperty(QLatin1String("window"), &view);
    view.setSource(QUrl(QLatin1String("qrc:/qml/Window.qml")));
    view.show();

    return app.exec();
}

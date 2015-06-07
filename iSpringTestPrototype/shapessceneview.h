#pragma once

#include <QQuickPaintedItem>
#include <memory>

class ShapesScene;

class ShapesSceneView : public QQuickPaintedItem
{
    Q_OBJECT
public:
    ShapesSceneView(QQuickItem *parent = 0);
    ~ShapesSceneView();

    void paint(QPainter *painter) override;
    bool enabledRedrawEachFrame() const;
    void setEnabledRedrawEachFrame(bool enabledRedrawEachFrame);

signals:
    void editFrameChanged(float x, float y, float width, float height);
    void editFrameDisappeared();

public slots:
    void newDocument();
    void openDocument();
    void saveDocument();
    void saveDocumentAs();
    void insertTriangle();
    void insertRectangle();
    void insertEllipse();
    void clicked(int x, int y);
    void followEditFrame(float x, float y, float width, float height);
    void updateEditFrame();

private:
    void warningOpenFailed(const QString &reason);
    void warningSaveFailed(const QString &reason);
    void saveDocument(const QString &path);
    bool ensureChangesSaved();
    std::unique_ptr<ShapesScene> makeScene();

    QString m_documentPath;
    std::unique_ptr<ShapesScene> m_scene;
};

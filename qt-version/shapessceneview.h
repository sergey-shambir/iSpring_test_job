#pragma once

#include <QQuickPaintedItem>
#include <memory>
#include "../math/rectangle.h"

class ShapesScene;

class ShapesSceneView : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool isUndoEnabled READ isUndoEnabled WRITE setIsUndoEnabled NOTIFY isUndoEnabledChanged)
    Q_PROPERTY(bool isRedoEnabled READ isRedoEnabled WRITE setIsRedoEnabled NOTIFY isRedoEnabledChanged)
public:
    ShapesSceneView(QQuickItem *parent = 0);
    ~ShapesSceneView();

    void paint(QPainter *painter) override;
    bool enabledRedrawEachFrame() const;
    void setEnabledRedrawEachFrame(bool enabledRedrawEachFrame);

    bool isUndoEnabled() const;
    bool isRedoEnabled() const;
    void setIsUndoEnabled(bool isUndoEnabled);
    void setIsRedoEnabled(bool isRedoEnabled);

signals:
    void editFrameChanged(float x, float y, float width, float height);
    void editFrameDisappeared();
    void isUndoEnabledChanged();
    void isRedoEnabledChanged();

public slots:
    void newDocument();
    void openDocument();
    void saveDocument();
    void saveDocumentAs();
    void insertTriangle();
    void insertRectangle();
    void insertEllipse();
    void clicked(int x, int y);
    void onDragStarted();
    void followEditFrame(float x, float y, float width, float height);
    void onDragFinished();
    void deletePickedNode();
    void undo();
    void redo();

private:
    void warningOpenFailed(const QString &reason);
    void warningSaveFailed(const QString &reason);
    void saveDocument(const QString &path);
    bool ensureChangesSaved();
    void updateSceneAndUi();
    std::unique_ptr<ShapesScene> makeScene();

    QString m_documentPath;
    std::unique_ptr<ShapesScene> m_scene;
    rectangle m_dragInitialRect;
    bool m_isUndoEnabled = false;
    bool m_isRedoEnabled = false;
};

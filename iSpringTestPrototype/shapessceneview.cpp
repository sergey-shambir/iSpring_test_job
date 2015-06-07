#include "shapessceneview.h"
#include "src/shapesscene.h"
#include "src/shapecommand.h"
#include "src/trianglenode.h"
#include "src/ellipsenode.h"
#include "src/rectanglenode.h"
#include <QPainter>
#include <QFileDialog>
#include <QMessageBox>
#include <QStandardPaths>
#include <functional>

static const float PEN_WIDTH = 2.f;

ShapesSceneView::ShapesSceneView(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_scene(makeScene())
{
    auto updateSceneSize = [this]() {
        m_scene->setMinimalSize(width(), height());
    };
    connect(this, &ShapesSceneView::widthChanged, updateSceneSize);
    connect(this, &ShapesSceneView::heightChanged, updateSceneSize);
}

ShapesSceneView::~ShapesSceneView() = default;

void ShapesSceneView::paint(QPainter *painter)
{
    painter->setRenderHint(QPainter::Antialiasing);

    QPen qPen{QColor(Qt::red)};
    qPen.setWidthF(PEN_WIDTH);
    GDIPlus::Pen pen{qPen};
    GDIPlus::Brush brush{QBrush(QColor(Qt::yellow))};
    GDIPlus::Graphics graphics{painter};
    AbstractNode::RenderContext context{pen, brush, graphics};
    m_scene->render(context);

    if (const NodePtr &node = m_scene->pickedNode()) {
        const rectangle &br = node->boundingRect();
        QPen framePen(QColor(0x40, 0xa0, 0xff));
        // Draw the frame around node
        framePen.setWidthF(PEN_WIDTH);
        painter->setPen(framePen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(br.x, br.y, br.width, br.height);
        // Draw resize drag points on corners
        framePen.setWidthF(3 * PEN_WIDTH);
        painter->setPen(framePen);
        painter->drawPoint(br.x, br.y);
        painter->drawPoint(br.x, br.y + br.height);
        painter->drawPoint(br.x + br.width, br.y);
        painter->drawPoint(br.x + br.width, br.y + br.height);
    }
}

void ShapesSceneView::newDocument()
{
    if (!ensureChangesSaved())
        return;
    m_documentPath.clear();
    m_scene->clearScene();
    emit editFrameDisappeared();
}

void ShapesSceneView::openDocument()
{
    QStringList dirVariants{QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)};
    QString dir = dirVariants.size() > 0 ? dirVariants[0] : QString();
    QString path = QFileDialog::getOpenFileName(nullptr, QString(), dir, QLatin1String("*.json"));
    if (path.isEmpty())
        return;
    QFile docFile(path);
    if (!docFile.open(QFile::ReadOnly)) {
        warningOpenFailed(tr("File deleted or storage removed before"
                             " document had been opened"));
        return;
    }
    json11::Json json;
    std::string error;
    {
        QByteArray bytes = docFile.readAll();
        json = json11::Json::parse(bytes.constData(), error);
    }
    if (json.is_null()) {
        warningOpenFailed(QString::fromStdString(error));
        return;
    }
    std::unique_ptr<ShapesScene> newScene(makeScene());
    if (!newScene->addFromJson(json, error)) {
        warningOpenFailed(tr("Scene file corrupted, failed on %1")
                          .arg(QString::fromStdString(error)));
    } else {
        m_scene.swap(newScene);
        m_documentPath = path;
        emit editFrameDisappeared();
    }
}

void ShapesSceneView::saveDocument()
{
    if (m_documentPath.isEmpty()) {
        saveDocumentAs();
    } else {
        saveDocument(m_documentPath);
    }
}

void ShapesSceneView::saveDocumentAs()
{
    QStringList dirVariants{QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation)};
    QString dir = dirVariants.size() > 0 ? dirVariants[0] : QString();
    QString path = QFileDialog::getSaveFileName(nullptr, QString(), dir, QLatin1String("*.json"));
    if (!path.isEmpty()) {
        if (!path.endsWith(QLatin1String(".json")))
            path += QLatin1String(".json");
        m_documentPath = path;
        saveDocument(path);
    }
}

void ShapesSceneView::insertTriangle()
{
    auto command = std::make_shared<InsertShapeCommand>(TriangleNode::CLASS_NAME);
    m_scene->doCommand(command);
}

void ShapesSceneView::insertRectangle()
{
    auto command = std::make_shared<InsertShapeCommand>(RectangleNode::CLASS_NAME);
    m_scene->doCommand(command);
}

void ShapesSceneView::insertEllipse()
{
    auto command = std::make_shared<InsertShapeCommand>(EllipseNode::CLASS_NAME);
    m_scene->doCommand(command);
}

void ShapesSceneView::clicked(int x, int y)
{
    if (m_scene->pickNode(x, y)) {
        const rectangle &br = m_scene->pickedNode()->boundingRect();
        emit editFrameChanged(br.x, br.y, br.width, br.height);
    } else {
        emit editFrameDisappeared();
    }
    update();
}

void ShapesSceneView::followEditFrame(float x, float y, float width, float height)
{
    if (NodePtr node = m_scene->pickedNode()) {
        node->moveTo(vec2(x, y));
        node->resizeTo(vec2(width, height));
        update();
    }
}

void ShapesSceneView::updateEditFrame()
{
    if (NodePtr node = m_scene->pickedNode()) {
        const rectangle &br = node->boundingRect();
        emit editFrameChanged(br.x, br.y, br.width, br.height);
    }
}

void ShapesSceneView::warningOpenFailed(const QString &reason)
{
    QMessageBox::warning(nullptr, tr("Failed to open"), reason);
}

void ShapesSceneView::warningSaveFailed(const QString &reason)
{
    QMessageBox::warning(nullptr, tr("Failed to save"), reason);
}

void ShapesSceneView::saveDocument(const QString &path)
{
    QFile output(path);
    if (!output.open(QFile::WriteOnly)) {
        warningOpenFailed(tr("File creation prohibited or storage removed"
                             " before document had been opened"));
        return;
    }
    auto json = m_scene->toJson();
    std::string content;
    json.dump(content);
    qint64 written = output.write(content.c_str(), content.size());
    if (written != qint64(content.size())) {
        warningOpenFailed(tr("I/O error while saving"));
    }
}

bool ShapesSceneView::ensureChangesSaved()
{
    if (m_scene->isModified()) {
        QMessageBox box;
        box.setText(tr("Document changes not saved yet"));
        box.setStandardButtons(QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        box.setDefaultButton(QMessageBox::Save);
        switch (box.exec()) {
        case QMessageBox::Save:
            saveDocument();
            break;
        case QMessageBox::Discard:
            break;
        default:
            return false;
        }
    }
    return true;
}

std::unique_ptr<ShapesScene> ShapesSceneView::makeScene()
{
    std::unique_ptr<ShapesScene> scene{new ShapesScene};
    scene->setMinimalSize(width(), height());
    scene->setUpdateCallback([this]() -> void {
        ShapesSceneView::update();
    });
    return scene;
}

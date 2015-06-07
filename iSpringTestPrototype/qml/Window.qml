import QtQuick 2.2
import QtQuick.Controls 1.1
import name.sshambir.ispringexam 1.0

Rectangle {
    RibbonUI {
        id: ribbonUI
        width: parent.width
        height: 120
        onActionNew: shapesView.newDocument()
        onActionOpen: shapesView.openDocument()
        onActionSave: shapesView.saveDocument()
        onActionSaveAs: shapesView.saveDocumentAs()
        onActionInsertEllipse: shapesView.insertEllipse();
        onActionInsertRectangle: shapesView.insertRectangle();
        onActionInsertTriangle: shapesView.insertTriangle();
    }

    ShapesScene {
        id: shapesView
        anchors.top: ribbonUI.bottom
        width: parent.width
        height: parent.height - ribbonUI.height
    }
}

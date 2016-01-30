import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1
import name.sshambir.ispringexam 1.0

Rectangle {
    id: root
    RibbonUI {
        id: ribbonUI
        width: parent.width
        height: 120
        onActionNew: shapesView.newDocument()
        onActionOpen: shapesView.openDocument()
        onActionSave: shapesView.saveDocument()
        onActionSaveAs: shapesView.saveDocumentAs()
        onActionInsertEllipse: shapesView.insertEllipse()
        onActionInsertRectangle: shapesView.insertRectangle()
        onActionInsertTriangle: shapesView.insertTriangle()
        onActionRedo: shapesView.redo()
        onActionUndo: shapesView.undo()
        isUndoEnabled: shapesView.isUndoEnabled
        isRedoEnabled: shapesView.isRedoEnabled
    }

    ScrollView {
        property int scrollbarWidth: (shapesView.width > scrollView.width || shapesView.height > scrollView.height) ? 10 : 0
        id: scrollView
        anchors.top: ribbonUI.bottom
        width: root.width
        height: root.height - ribbonUI.height

        style: ScrollViewStyle {
            handle: Item {
                implicitWidth: scrollView.scrollbarWidth
                implicitHeight: scrollView.scrollbarWidth
                Rectangle {
                    color: styleData.pressed ? "#ccd5da" : "#afbac0"
                    anchors.fill: parent
                }
            }
            scrollBarBackground: Item {
                implicitWidth: scrollView.scrollbarWidth
                implicitHeight: scrollView.scrollbarWidth
                Rectangle {
                    color: "#e9f0f5"
                    anchors.fill: parent
                }
            }
            decrementControl: Item {}
            incrementControl: Item {}
            minimumHandleLength: 24
        }
        ShapesScene {
            id: shapesView
            width: 800
            height: 600
        }
    }

}

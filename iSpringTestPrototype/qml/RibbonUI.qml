import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Rectangle {
    implicitWidth: 600
    implicitHeight: 120

    signal actionNew()
    signal actionOpen()
    signal actionSave()
    signal actionSaveAs()
    signal actionInsertRectangle()
    signal actionInsertTriangle()
    signal actionInsertEllipse()
    signal actionUndo()
    signal actionRedo()

    TabView {
        anchors.fill: parent
        Tab {
            title: qsTr("File")
            Item {
                Item {
                    x: 2
                    y: 2
                    height: parent.height - 2
                    width: 264
                    Label {
                        text: qsTr("File")
                        color: "#444"
                        font.pixelSize: 10
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 2
                    }
                    Rectangle {
                        height: parent.height
                        width: 1
                        color: "#aaa"
                        anchors.right: parent.right
                    }
                    Row {
                        anchors.fill: parent
                        anchors.leftMargin: 2
                        RibbonButton {
                            id: buttonNew
                            x: 4
                            width: 64
                            height: 64
                            text: qsTr("New")
                            icon: "qrc:/assets/document-new.png"
                            onClicked: actionNew()
                        }
                        RibbonButton {
                            id: buttonOpen
                            x: 4
                            width: 64
                            height: 64
                            text: qsTr("Open...")
                            icon: "qrc:/assets/document-open.png"
                            onClicked: actionOpen()
                        }
                        RibbonButton {
                            id: buttonSave
                            x: 4
                            width: 64
                            height: 64
                            text: qsTr("Save")
                            icon: "qrc:/assets/document-save.png"
                            onClicked: actionSave()
                        }
                        RibbonButton {
                            id: buttonSaveAs
                            x: 4
                            width: 64
                            height: 64
                            text: qsTr("Save as...")
                            icon: "qrc:/assets/document-save-as.png"
                            onClicked: actionSaveAs()
                        }
                    }
                }
            }
        }
        Tab {
            title: qsTr("Insert")
            Item {
                Item {
                    x: 2
                    y: 2
                    height: parent.height - 2
                    width: 200
                    Label {
                        text: qsTr("Shapes")
                        color: "#444"
                        font.pixelSize: 10
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 2
                    }
                    Rectangle {
                        height: parent.height
                        width: 1
                        color: "#aaa"
                        anchors.right: parent.right
                    }
                    Row {
                        anchors.fill: parent
                        anchors.leftMargin: 4
                        RibbonButton {
                            id: buttonRect
                            width: 64
                            height: 64
                            text: qsTr("Rectangle")
                            icon: "qrc:/assets/icon-rect.png"
                            onClicked: actionInsertRectangle()
                        }
                        RibbonButton {
                            id: buttonTriangle
                            width: 64
                            height: 64
                            text: qsTr("Triangle")
                            icon: "qrc:/assets/icon-triangle.png"
                            onClicked: actionInsertTriangle()
                        }
                        RibbonButton {
                            id: buttonCircle
                            width: 64
                            height: 64
                            text: qsTr("Ellipse")
                            icon: "qrc:/assets/icon-circle.png"
                            onClicked: actionInsertEllipse()
                        }
                    }
                }
            }
        }
    }

    Button {
        x: 180
        y: 4
        width: 24
        height: 24
        iconSource: "qrc:/assets/lc_undo.png"
        onClicked: actionUndo()
    }
    Button {
        x: 210
        y: 4
        width: 24
        height: 24
        iconSource: "qrc:/assets/lc_redo.png"
        onClicked: actionRedo()
    }
}

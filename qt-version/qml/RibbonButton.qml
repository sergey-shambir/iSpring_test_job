import QtQuick 2.2
import QtQuick.Controls 1.1
import QtQuick.Controls.Styles 1.1

Button {
    property string icon

    style: ButtonStyle {
        background: Item {
            implicitWidth: 100
            implicitHeight: 25
            Rectangle {
                visible: control.pressed
                anchors.fill: parent
                radius: 4
                gradient: Gradient {
                    GradientStop { position: 0 ; color: "#ccc" }
                    GradientStop { position: 1 ; color: "#aaa" }
                }
            }
            Image {
                y: 4
                anchors.horizontalCenter: parent.horizontalCenter
                source: control.icon
            }
        }
        label: Component {
            Label {
                text: control.text
                verticalAlignment: Text.AlignBottom
                horizontalAlignment: Text.AlignHCenter
                font.pixelSize: 11
            }
        }
    }
}

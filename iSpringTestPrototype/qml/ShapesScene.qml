import QtQuick 2.2
import QtQuick.Controls 1.1
import name.sshambir.ispringexam 1.0

ShapesSceneView {
    id: root
    property int _invariant: 0 // Invariant coordinate when resizing
    property int _MINIMAL_SIZE: 60

    function moveToEditFrame() {
        followEditFrame(editFrame.x, editFrame.y, editFrame.width, editFrame.height)
    }
    function resizeLeftEdge() {
        var dx = _invariant - editFrame.x
        followEditFrame(editFrame.x, editFrame.y, dx + editFrame.width, editFrame.height)
    }
    function resizeRightEdge() {
        var dx = editFrame.x - _invariant
        followEditFrame(_invariant, editFrame.y, dx + editFrame.width, editFrame.height)
    }
    function resizeTopEdge() {
        var dy = _invariant - editFrame.y
        followEditFrame(editFrame.x, editFrame.y, editFrame.width, dy + editFrame.height)
    }
    function resizeBottomEdge() {
        var dy = editFrame.y - _invariant
        followEditFrame(editFrame.x, _invariant, editFrame.width, dy + editFrame.height)
    }
    onEditFrameChanged: {
        editFrame.x = x
        editFrame.y = y
        editFrame.width = width
        editFrame.height = height
        editFrame.visible = true
        edgeLeft.drag.maximumX = editFrame.x + editFrame.width - _MINIMAL_SIZE
        edgeRight.drag.minimumX = editFrame.x - editFrame.width + _MINIMAL_SIZE
        edgeTop.drag.maximumY = editFrame.y + editFrame.heiht - _MINIMAL_SIZE
        edgeBottom.drag.minimumY = editFrame.y - editFrame.height + _MINIMAL_SIZE
    }
    onEditFrameDisappeared: {
        editFrame.visible = false
    }
    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        onClicked: parent.clicked(mouse.x, mouse.y)
    }
    Item {
        id: editFrame
        visible: false
        MouseArea {
            anchors.fill: parent
            id: dragArea
            acceptedButtons: Qt.LeftButton
            cursorShape: pressed ? Qt.ClosedHandCursor : Qt.OpenHandCursor
            drag.target: editFrame
            drag.axis: Drag.XAndYAxis
            drag.minimumX: 0
            drag.minimumY: 0
            drag.maximumX: root.width - editFrame.width
            drag.maximumY: root.height - editFrame.height
            drag.onActiveChanged: {
                if (drag.active)
                    window.frameSwapped.connect(moveToEditFrame)
                else {
                    window.frameSwapped.disconnect(moveToEditFrame)
                    updateEditFrame()
                }
            }
        }
        MouseArea {
            id: edgeLeft
            x: -2
            y: -2
            width: 4
            height: parent.height + 4
            cursorShape: Qt.SizeHorCursor
            drag.axis: Drag.XAxis
            drag.target: editFrame
            drag.minimumX: 0
            drag.onActiveChanged: {
                if (drag.active) {
                    root._invariant = editFrame.x
                    window.frameSwapped.connect(resizeLeftEdge)
                } else {
                    window.frameSwapped.disconnect(resizeLeftEdge)
                    updateEditFrame()
                }
            }
        }
        MouseArea {
            id: edgeRight
            x: parent.width - 2
            y: -2
            width: 4
            height: parent.height + 4
            cursorShape: Qt.SizeHorCursor
            drag.axis: Drag.XAxis
            drag.target: editFrame
            drag.maximumX: root.width - editFrame.width
            drag.onActiveChanged: {
                if (drag.active) {
                    root._invariant = editFrame.x
                    window.frameSwapped.connect(resizeRightEdge)
                } else {
                    window.frameSwapped.disconnect(resizeRightEdge)
                    updateEditFrame()
                }
            }
        }
        MouseArea {
            id: edgeTop
            x: -2
            y: -2
            width: parent.width + 4
            height: 4
            cursorShape: Qt.SizeVerCursor
            drag.axis: Drag.YAxis
            drag.target: editFrame
            drag.minimumY: 0
            drag.onActiveChanged: {
                if (drag.active) {
                    root._invariant = editFrame.y
                    window.frameSwapped.connect(resizeTopEdge)
                } else {
                    window.frameSwapped.disconnect(resizeTopEdge)
                    updateEditFrame()
                }
            }
        }
        MouseArea {
            id: edgeBottom
            x: -2
            y: parent.height - 2
            width: parent.width + 4
            height: 4
            cursorShape: Qt.SizeVerCursor
            drag.axis: Drag.YAxis
            drag.target: editFrame
            drag.maximumY: root.height - editFrame.height
            drag.onActiveChanged: {
                if (drag.active) {
                    root._invariant = editFrame.y
                    window.frameSwapped.connect(resizeBottomEdge)
                } else {
                    window.frameSwapped.disconnect(resizeBottomEdge)
                    updateEditFrame()
                }
            }
        }
        // TODO: fix corners positions - assign x/y/w/h right here
        FrameCornerArea {
            id: cornerTopLeft
            cursorShape: Qt.SizeFDiagCursor
        }
        FrameCornerArea {
            id: cornerBottomLeft
            cursorShape: Qt.SizeBDiagCursor
        }
        FrameCornerArea {
            id: cornerTopRight
            cursorShape: Qt.SizeBDiagCursor
        }
        FrameCornerArea {
            id: cornerBottomRight
            cursorShape: Qt.SizeFDiagCursor
        }
    }
}

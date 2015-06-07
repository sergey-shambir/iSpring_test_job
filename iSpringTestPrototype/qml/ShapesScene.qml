import QtQuick 2.2
import QtQuick.Controls 1.1
import name.sshambir.ispringexam 1.0

ShapesSceneView {
    id: root
    property int _dragInvariant: 0 // Invariant coordinate when resizing
    property int _MINIMAL_SIZE: 60

    function movePickedNode() {
        followEditFrame(editFrame.x, editFrame.y, editFrame.width, editFrame.height)
    }
    function resizeLeftEdge() {
        var dx = _dragInvariant - editFrame.x
        followEditFrame(editFrame.x, editFrame.y, dx + editFrame.width, editFrame.height)
    }
    function resizeRightEdge() {
        var dx = editFrame.x - _dragInvariant
        followEditFrame(_dragInvariant, editFrame.y, dx + editFrame.width, editFrame.height)
    }
    function resizeTopEdge() {
        var dy = _dragInvariant - editFrame.y
        followEditFrame(editFrame.x, editFrame.y, editFrame.width, dy + editFrame.height)
    }
    function resizeBottomEdge() {
        var dy = editFrame.y - _dragInvariant
        followEditFrame(editFrame.x, _dragInvariant, editFrame.width, dy + editFrame.height)
    }
    function toggleDragState(dragActive, dragFunction) {
        if (dragActive) {
            window.frameSwapped.connect(dragFunction)
            onDragStarted()
        } else {
            window.frameSwapped.disconnect(dragFunction)
            onDragFinished()
        }
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
        focus: visible
        Keys.onDeletePressed: deletePickedNode()
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
            drag.onActiveChanged: toggleDragState(drag.active, movePickedNode)
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
                if (drag.active)
                    root._dragInvariant = editFrame.x
                toggleDragState(drag.active, resizeLeftEdge)
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
                if (drag.active)
                    root._dragInvariant = editFrame.x
                toggleDragState(drag.active, resizeRightEdge)
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
                if (drag.active)
                    root._dragInvariant = editFrame.y
                toggleDragState(drag.active, resizeTopEdge)
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
                if (drag.active)
                    root._dragInvariant = editFrame.y
                toggleDragState(drag.active, resizeBottomEdge)
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

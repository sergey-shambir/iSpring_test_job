import QtQuick 2.2
import QtQuick.Controls 1.1
import name.sshambir.ispringexam 1.0

ShapesSceneView {
    id: root
    property vector2d _dragStart;
    property int _MINIMAL_SIZE: 60

    function movePickedNode() {
        followEditFrame(editFrame.x, editFrame.y, editFrame.width, editFrame.height)
    }
    function resizeLeftEdge() {
        var dx = _dragStart.x - editFrame.x
        followEditFrame(editFrame.x, editFrame.y, dx + editFrame.width, editFrame.height)
    }
    function resizeRightEdge() {
        var dx = editFrame.x - _dragStart.x
        followEditFrame(_dragStart.x, editFrame.y, dx + editFrame.width, editFrame.height)
    }
    function resizeTopEdge() {
        var dy = _dragStart.y - editFrame.y
        followEditFrame(editFrame.x, editFrame.y, editFrame.width, dy + editFrame.height)
    }
    function resizeBottomEdge() {
        var dy = editFrame.y - _dragStart.y
        followEditFrame(editFrame.x, _dragStart.y, editFrame.width, dy + editFrame.height)
    }
    function resizeTopLeftCorner() {
        var dx = _dragStart.x - editFrame.x
        var dy = _dragStart.y - editFrame.y
        followEditFrame(editFrame.x, editFrame.y, dx + editFrame.width, dy + editFrame.height)
    }
    function resizeTopRightCorner() {
        var dx = editFrame.x - _dragStart.x
        var dy = _dragStart.y - editFrame.y
        followEditFrame(_dragStart.x, editFrame.y, dx + editFrame.width, dy + editFrame.height)
    }
    function resizeBottomLeftCorner() {
        var dx = _dragStart.x - editFrame.x
        var dy = editFrame.y - _dragStart.y
        followEditFrame(editFrame.x, _dragStart.y, dx + editFrame.width, dy + editFrame.height)
    }
    function resizeBottomRightCorner() {
        var dx = editFrame.x - _dragStart.x
        var dy = editFrame.y - _dragStart.y
        followEditFrame(_dragStart.x, _dragStart.y, dx + editFrame.width, dy + editFrame.height)
    }
    function toggleDragState(dragActive, dragFunction) {
        if (dragActive) {
            _dragStart = Qt.vector2d(editFrame.x, editFrame.y)
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
        {
            var edge = x + width - _MINIMAL_SIZE
            edgeLeft.drag.maximumX = edge
            cornerTopLeft.drag.maximumX = edge
            cornerBottomLeft.drag.maximumX = edge
        }
        {
            var edge = x - width + _MINIMAL_SIZE
            edgeRight.drag.minimumX = edge
            cornerTopRight.drag.minimumX = edge
            cornerBottomRight.drag.minimumX = edge
        }
        {
            var edge = y + height - _MINIMAL_SIZE
            edgeTop.drag.maximumY = edge
            cornerTopLeft.drag.maximumY = edge
            cornerTopRight.drag.maximumY = edge
        }
        {
            var edge = y - height + _MINIMAL_SIZE
            edgeBottom.drag.minimumY = edge
            cornerBottomLeft.drag.minimumY = edge
            cornerBottomRight.drag.minimumY = edge
        }
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
            drag.onActiveChanged: toggleDragState(drag.active, resizeLeftEdge)
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
            drag.onActiveChanged: toggleDragState(drag.active, resizeRightEdge)
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
            drag.onActiveChanged: toggleDragState(drag.active, resizeTopEdge)
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
            drag.onActiveChanged: toggleDragState(drag.active, resizeBottomEdge)
        }
        MouseArea {
            id: cornerTopLeft
            cursorShape: Qt.SizeFDiagCursor
            x: -4
            y: -4
            width: 8
            height: 8
            drag.axis: Drag.XAndYAxis
            drag.target: editFrame
            drag.minimumX: 0
            drag.minimumY: 0
            drag.onActiveChanged: toggleDragState(drag.active, resizeTopLeftCorner)
        }
        MouseArea {
            id: cornerBottomLeft
            cursorShape: Qt.SizeBDiagCursor
            x: -4
            y: editFrame.height - 4
            width: 8
            height: 8
            drag.axis: Drag.XAndYAxis
            drag.target: editFrame
            drag.minimumX: 0
            drag.maximumY: root.height - editFrame.height
            drag.onActiveChanged: toggleDragState(drag.active, resizeBottomLeftCorner)
        }
        MouseArea {
            id: cornerTopRight
            cursorShape: Qt.SizeBDiagCursor
            x: editFrame.width - 4
            y: -4
            width: 8
            height: 8
            drag.axis: Drag.XAndYAxis
            drag.target: editFrame
            drag.maximumX: root.width - editFrame.width
            drag.minimumY: 0
            drag.onActiveChanged: toggleDragState(drag.active, resizeTopRightCorner)
        }
        MouseArea {
            id: cornerBottomRight
            cursorShape: Qt.SizeFDiagCursor
            x: editFrame.width - 4
            y: editFrame.height - 4
            width: 8
            height: 8
            drag.axis: Drag.XAndYAxis
            drag.target: editFrame
            drag.maximumX: root.width - editFrame.width
            drag.maximumY: root.height - editFrame.height
            drag.onActiveChanged: toggleDragState(drag.active, resizeBottomRightCorner)
        }
    }
}

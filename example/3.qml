import QtQuick 2.12
import com.bckmnn.properties 1.0

Rectangle {
    id: root
    property bool editing: false

    PropertySettings{
        id: props
        property string label: "MyText Label";
    }
    width: parent.width
    height: 100
    color: "blue"

    Text{
        anchors.fill: parent
        text: props.label
        visible: !root.editing
    }
    MouseArea{
        anchors.fill: parent
        onDoubleClicked: {
            root.editing = !root.editing
        }
    }
    TextEdit{
        id: edit
        anchors.fill: parent
        visible: root.editing
        text: props.label
        onEditingFinished: props.label = text
    }
    Rectangle{
        width: 20
        height: 20
        anchors.top: edit.top
        anchors.right: edit.right
        visible:  root.editing
        MouseArea{
            anchors.fill: parent
            onClicked: {
                edit.focus = false
                root.editing = false
            }
        }
    }


}

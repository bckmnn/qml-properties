import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle{
    id: root
    width: parent.width
    height: modelData.multiline ? 45*2 :45
    color: "lightgrey"
    Row{
        id: row
        spacing: 5
        leftPadding: 10
        height: parent.height
        Text {
            width: 100-10
            id: label
            text: modelData.label
            height: parent.height
            verticalAlignment: Text.AlignVCenter
        }
        TextField{
            id: control1
            visible: modelData.multiline === false
            text: modelData.value
            onTextEdited: {
                modelData.value = text
            }
            height: parent.height-parent.padding*2
            width: root.width - (100+5)
        }
        Rectangle{
            visible: modelData.multiline === true
            height: parent.height-parent.padding*2
            width: root.width - (100+5)
            border.color: control2.activeFocus ? "blue":"lightgrey"
            border.width: control2.activeFocus ? 2 : 1
            TextArea{
                id: control2
                anchors.fill: parent
                padding: 5
                text: modelData.value
                onEditingFinished: {
                    modelData.value = text
                }
            }
        }

    }
}

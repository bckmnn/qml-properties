import QtQuick 2.12
import QtQuick.Controls 2.12

Rectangle{
    id:root
    width: parent.width
    height: 45
    color: "lightgrey"
    Row{
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
        CheckBox{
            id: control
            checked: modelData.value
            onCheckedChanged: {
                if(checked !== modelData.value){
                    modelData.value = checked
                }

            }
            height: parent.height-parent.padding*2
        }
    }
}

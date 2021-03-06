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
            text: modelData.label + "\n" + "("+modelData.from +"-"+modelData.to+")"
            height: parent.height
            verticalAlignment: Text.AlignVCenter
        }
        SpinBox{
            id: control
            editable: true
            from: validator.bottom
            to: validator.top
            stepSize: modelData.stepSize !== 0 ? modelData.stepSize : 1
            validator: IntValidator {
                locale: control.locale.name
            }
            value: modelData.value
            onValueModified: {
                modelData.value = value
            }
            height: parent.height-parent.padding*2
        }
    }
}

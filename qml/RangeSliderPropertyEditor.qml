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
        Slider{
            id: control
            from: modelData.from
            to: modelData.to
            value: modelData.value
            stepSize: modelData.stepSize
            snapMode: stepSize>0 ? Slider.SnapAlways:Slider.NoSnap
            onMoved: {
                modelData.value = valueAt(position)
            }
            height: parent.height-parent.padding*2
        }
    }
}

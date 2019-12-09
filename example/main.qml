import QtQuick 2.12
import QtQuick.Window 2.12
import com.bckmnn.properties 1.0

Window {
    id: win
    visible: true
    width: 640
    height: 480
    title: qsTr("QML-Properties Example")


    Rectangle{
        Properties.list: [
            BoolProperty{
                id: propShowBorder
                label: "show Border"
                value: false
            },
            DoubleProperty{
                id: propXpos
                label: "xpos"
                value: 50
            },
            RangeProperty{
                id: propYpos
                label: "ypos"
                value: 50
                from: 0
                to: 640
            },
            RangeProperty{
                id: propWidth
                label: "width"
                value: 100
                from: 0
                to: win.width
                delegate: "RangeSliderPropertyEditor.qml"
            },
            StringProperty{
                id: propText
                label: "txt"
                value: "hallo"
                multiline: true
            }

        ]

        x: propXpos.value
        y: propYpos.value
        border.color: "red"
        border.width: propShowBorder.value ? 5 : 0

        color: "green"

        width: propWidth.value
        height: 200

        Text {
            id: name
            text: propText.value
        }

        MouseArea{
            anchors.fill: parent
            onClicked: PropertyManager.edit(parent)
        }

    }



    PropertyPanel{
        id: panel
        width: 300
        height: parent.height
        anchors.right: parent.right
        model: PropertyManager.props
    }
}

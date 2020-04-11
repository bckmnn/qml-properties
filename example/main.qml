import QtQuick 2.12
import QtQuick.Window 2.12
import com.bckmnn.properties 1.0

Window {
    id: win
    visible: true
    width: 640
    height: 480
    title: mySettings.windowTitle


    PropertySettings{
        id: mySettings
        readonly property string readOnly: "test"
        property color bgColor: "#f0f0f0";
        property string windowTitle: "QML-Example"
        property var list: ["1.qml","2.qml","3.qml"]
        property var object: {"1":"hallo","2":"das","3":"ist"}
        property int myInt: 10
        property point coord: Qt.point(12,30)
    }

    ListView{
        id: list
        anchors.fill: parent
        model: mySettings.list.length
        delegate: Component{
            Loader{
                id:loader
                width: parent.width
                source: mySettings.list[index]
            }
        }
    }
    Rectangle{
        anchors.top: parent.top
        anchors.topMargin: mySettings.coord.y
        anchors.rightMargin: mySettings.coord.x
        anchors.right: parent.right
        width: 100
        height: 100
        color: "purple"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                mySettings.load()
            }
        }
    }



    Rectangle{
        anchors.centerIn: parent
        width: 100
        height: 100
        color: mySettings.bgColor
        MouseArea{
            anchors.fill: parent
            onClicked: {
                mySettings.bgColor = "red"
                //mySettings.load()
                mySettings.save()
            }
        }
    }
}

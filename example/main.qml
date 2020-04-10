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
        property color bgColor: "#f0f0f0";
        property string windowTitle: "QML-Example"
        property var list: ["1.qml","2.qml","3.qml"]
        property var object: {"1":"hallo","2":"das","3":"ist"}
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

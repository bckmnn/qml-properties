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
        property var list: ["1.qml","2.qml","3.qml","3.qml"]
        property var object: {"1":"hallo","2":"das","3":"ist"}
        property int myInt: 10
        property bool onoff: true
        property point coord: Qt.point(12,30)
        property rect rect: Qt.rect(0,0,10,10);
        property url url: Qt.resolvedUrl("c:/windows")
        property var listModel: ListModel{
            ListElement{
                color: "red"
                text: "Hallo0"
                point: 10.2
            }
            ListElement{
                color: "green"
                text: "Hallo1"
            }
            ListElement{
                color: "blue"
                text: "Hallo2"
            }
        }
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

    ListView{
        id: secondList
        width: parent.width
        height: parent.height/2
        anchors.bottom: parent.bottom
        orientation: ListView.Horizontal
        model: mySettings.listModel
        delegate: Component{
            Rectangle{
                id:wrapper
                width: 100
                height: parent.height
                color: model.color
                border.width: 2
                border.color: "black"
                PropertySettings{
                    id: props
                    property color boxColor: "#ff00ff"
                    property string label: "a label"
                    listIndex: index
                    list: wrapper.ListView.view
                }
                Text {
                    anchors.centerIn: parent
                    font.pixelSize: 24
                    text: model.text
                }
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

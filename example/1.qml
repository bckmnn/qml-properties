import QtQuick 2.12
import com.bckmnn.properties 1.0

Rectangle {
    id: root
    PropertySettings{
        id: props
        property int w: 10
        property color bgColor: "yellow";
        listIndex: index !== undefined ? index : -1
        list: root.parent.ListView ? root.parent.ListView.view : undefined
    }
    width: parent.width
    height: 100
    color: props.bgColor
}

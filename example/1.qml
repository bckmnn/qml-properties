import QtQuick 2.12
import com.bckmnn.properties 1.0

Rectangle {
    PropertySettings{
        id: props
        property int w: 10
        property color bgColor: "yellow";
    }
    width: parent.width
    height: 100
    color: props.bgColor
}

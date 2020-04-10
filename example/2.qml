import QtQuick 2.12
import com.bckmnn.properties 1.0

Rectangle {
    PropertySettings{
        id: props
        property real h: 12.320
        property color bgColor: "green";
    }
    width: parent.width
    height: 100
    color: props.bgColor
}

import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Controls.Styles 1.4

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
            id: label2
            text: modelData.label
            height: parent.height
            verticalAlignment: Text.AlignVCenter
        }
        SpinBox{
            // FIXME: This is the worst double spin box
            id: control
            property int decimals: 2
            property real realValue: modelData.value
            value: realValue*100
            stepSize: 100

            from: -21000000
            to: 21000000

            editable: true
            validator: DoubleValidator {
                bottom: control.from
                top:   control.to
            }
            onValueModified: {
                modelData.value = Number(value/100)
            }
            textFromValue: function(value, locale) {
                return Number(value / 100).toLocaleString(locale, 'f', control.decimals)
            }

            valueFromText: function(text, locale) {
                return Number.fromLocaleString(locale, text) * 100
            }
            height: parent.height-parent.padding*2
        }



    }
}

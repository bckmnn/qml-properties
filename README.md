# qml-properties

A editable property system for QML to be able to modify arbitrary properties through a property panel user interface.

This is currently nothing more as a proof of concept. While writing this readme, I have to admit that it might be confusing since the term property is already used in qml. I am using the word property sometimes to refer to "regular" qml properties and sometimes to describe "custom" editable properties you might encounter in a property editor panel in programs like Unreal Editor/Unity Editor. But since you are all educated readers you will figure out which meaning I intend to convey.

## Installation

### qpm

with qpm just go to your project directory and install with this command:

```
qpm install com.bckmnn.properties
```

after installation you just need to add `include(vendor/vendor.pri)` to your *.pro file if it's not
there already.

## Usage

To use the property system you will need to import it in your qml files.

```
import com.bckmnn.properties 1.0
```

You can then use the attached property `Properties.list: []` to store editable properties in your qml items. The `PropertyPanel` will allow you to edit properties that are stored in the `props` property of the `PropertyManager` singleton. To populate this `props` property use the `PropertyManager.edit(<item>)` method with the item as parameter, whose attached property `Properties.list[]` you would like to edit.

### Available Property Types

The following property types are available:
  * BoolProperty
  * StringPropertyv
  * IntProperty 
  * DoubleProperty
  * RangeProperty

For details see `propertytypes.h` the delegates for editing the properties in the `PropertyPanel` can be found in the `qrc`. An `RangeProperty` could have a `RangeSliderPropertyEditor` delegate (with a slider) or a `RangePropertyEditor` delegate (just a spinbox).


## Example:

See the following for a complete example:

```
import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import com.bckmnn.properties 1.0

Window {
    id: win
    visible: true
    width: 640
    height: 480
    title: qsTr("Click on Items to view in PropertyManager")

    Rectangle{
        x: myXProp.value
        y: myYProp.value
        width: myWidth.value
        height: 100
        border.color: "green"
        border.width: showBorder.value ? 5:0
        Properties.list: [
            IntProperty{
                id: myXProp
                label: "X Pos"
                value: 100
            },
            RangeProperty{
                id: myYProp
                label: "Y Pos"
                from: -50
                to: 200
                value: 100
                stepSize: 5
                delegate: "DoublePropertyEditor.qml"
            },
            RangeProperty{
                id: myWidth
                label: "Width"
                from: 0
                to: 250
                value: 100
                stepSize: 25
                delegate: "RangeSliderPropertyEditor.qml"
            },
            BoolProperty{
                id: showBorder
                label: "Show Border"
                value: true
            }

        ]

        color: "red"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                PropertyManager.edit(parent)
            }
        }
    }

    Rectangle{
        x: xProp.value
        y: yProp.value
        width: 100
        height: myHeight.value

        Properties.list: [
            IntProperty{
                id: xProp
                label: "X Pos"
                value: 0
            },
            RangeProperty{
                id: yProp
                label: "Y Pos"
                from: -50
                to: 200
                value: 0
                stepSize: 5
            },
            RangeProperty{
                id: myHeight
                label: "Height"
                from: 0
                to: 250
                value: 100
                stepSize: 25
                delegate: "RangeSliderPropertyEditor.qml"
            },
            StringProperty{
                id: myLabel
                label: "Text"
                value: "Hello"
                multiline: true
            }
        ]

        Text {
            id: name
            text: myLabel.value
            anchors.centerIn: parent
            font.pixelSize: 30
        }
        color: "yellow"
        MouseArea{
            anchors.fill: parent
            onClicked: {
                PropertyManager.edit(parent)
            }
        }

        Rectangle{
            x: childXpos.value
            y: childYpos.value
            width: childWidth.value
            height: childHeight.value
            Properties.list: [
                IntProperty{
                    id: childXpos
                    label: "X Pos"
                    value: 80
                },
                RangeProperty{
                    id: childYpos
                    label: "Y Pos"
                    from: -50
                    to: 200
                    value: 20
                    stepSize: 5
                    delegate: "DoublePropertyEditor.qml"
                },
                RangeProperty{
                    id: childWidth
                    label: "Width"
                    from: 0
                    to: 250
                    value: 100
                    stepSize: 25
                    delegate: "RangeSliderPropertyEditor.qml"
                },
                RangeProperty{
                    id: childHeight
                    label: "Height"
                    from: 0
                    to: 250
                    value: 100
                    stepSize: 25
                    delegate: "RangeSliderPropertyEditor.qml"
                }
            ]
            color: "blue"

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    PropertyManager.edit(parent)
                }
            }
        }
    }

    PropertyPanel{
        width: 300
        height: parent.height
        anchors.right: parent.right
        model: PropertyManager.props
    }
}

```

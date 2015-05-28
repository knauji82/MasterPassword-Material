import Material 0.1
import QtQuick 2.3

TextField {
    property int value: 0
    property int minValue: 0
    property int maxValue: 999

    text: value
    inputMask: new Array(maxValue.toString().length+1).join("0")

    Keys.onUpPressed: if (value < maxValue) value += 1
    Keys.onDownPressed: if (value > minValue) value -= 1
}

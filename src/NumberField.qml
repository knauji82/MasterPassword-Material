import Material 0.1
import QtQuick 2.3

TextField {
    property int value: 0
    property int minValue: 0
    property int maxValue: 999

    inputMask: new Array(maxValue.toString().length+1).join("0")
    text: value

    onTextChanged: {
        if (text.trim() == "")
            value = minValue
        var v = parseInt(text)
        if (inRange(v) && v !== value)
            value = v
    }

    Keys.onUpPressed: if (value < maxValue) value += 1
    Keys.onDownPressed: if (value > minValue) value -= 1

    function inRange(value) {
        return value >= minValue && value <= maxValue
    }
}

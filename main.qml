import QtQuick 2.3
import QtQuick.Window 2.2

Window {
  id: root
  visible: true
  width: 300
  height: 600

  property Rectangle correctAnswer

  Rectangle {
    anchors.fill: parent
    color: "black"
  }

  Column {
    y: spacing
    spacing: root.height / (6*6)
    Rectangle {
      x: width * 0.05
      y: 10
      width: root.width * 0.9
      height: root.height / 6
      color: "#070"
      radius: height / 3
      Text {
        anchors.centerIn: parent
        text: app_language.wordToGuess
        font.pixelSize: parent.height * 0.5
        color: "white"
      }
    }
    Repeater
    {
      model: app_language.answers
      delegate: Rectangle {
        id: rect
        x: width * 0.05
        y: 10
        width: root.width * 0.9
        height: root.height / 6
        color: "#059"
        radius: height / 3
        Text {
          anchors.centerIn: parent
          text: modelData
          font.pixelSize: parent.height * 0.5
          color: "white"
          onTextChanged: {
            parent.color = "#059"
          }
        }
        MouseArea {
          anchors.fill: parent
          onClicked: {
            if(modelData != app_language.correctAnswer)
            {
              parent.color = "red"
            }
            nextMouseArea.enabled = true
            root.correctAnswer.color = "green"
          }
        }
        Component.onCompleted: {
          if(modelData == app_language.correctAnswer)
          {
            root.correctAnswer = rect
          }
        }
      }
    }
  }
  MouseArea
  {
    id: nextMouseArea
    anchors.fill: parent
    enabled: false
    onClicked:
    {
      app_language.nextWord()
      nextMouseArea.enabled = false
    }
  }
}

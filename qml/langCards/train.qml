import QtQuick 2.3
import QtQuick.Window 2.2

Item {
  id: root

  property Rectangle correctAnswer
  property int state: 0
  property date startdate: new Date()

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
        visible: root.state > 0
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
            app_language.userAnswer(modelData, (new Date().getTime() - root.startdate.getTime()))
            root.correctAnswer.color = "green"
            root.state = 2
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
    anchors.fill: parent
    enabled: root.state != 1
    onClicked:
    {
      switch(root.state)
      {
      case 0:
        root.state = 1
        break;
      case 2:
        app_language.nextWord();
        root.state = 0;
        root.startdate  = new Date();
        break;
      }
    }
  }
}

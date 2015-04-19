import QtQuick 2.3
import QtQuick.Window 2.2
import langCards 1.0

Window {
  id: root
  visible: true
  width: 300
  height: 600

  Rectangle
  {
    id: rootRectangle
    anchors.fill: parent
    color: "black"
    state: "menu"

    Column {
      id: menu
      y: spacing
      spacing: root.height / (6*6)
      visible: false
      ListModel {
        id: menuModel
        ListElement { itemText: "Train"; itemState: "train" }
        ListElement { itemText: "Debug"; itemState: "debug" }
      }

      Repeater
      {
        model: menuModel
        Rectangle {
          x: width * 0.05
          width: root.width * 0.9
          height: root.height / 6
          color: "#059"
          radius: height / 3
          Text {
            anchors.centerIn: parent
            text: itemText
            font.pixelSize: parent.height * 0.5
            color: "white"
          }
          MouseArea
          {
            anchors.fill: parent
            onClicked: {
              rootRectangle.state = itemState
              console.log("test", itemState, rootRectangle.state)
            }
          }
        }
      }
    }
    Train {
      id: train
      anchors.fill: parent
      visible: false
    }
    Debug {
      id: debug
      anchors.fill: parent
      visible: false
    }
    states: [
      State {
        name: "menu"
        PropertyChanges { target: menu; visible: true }
      },
      State {
        name: "train"
        PropertyChanges { target: train; visible: true }
      },
      State {
        name: "debug"
        PropertyChanges { target: debug; visible: true }
      }

    ]
  }
}

import QtQuick 2.3
import QtQuick.Controls 1.0

Item
{
  TextArea {
    id: text
    anchors.fill: parent

  }

  onVisibleChanged: {
    text.text = app_language.databaseDump()
  }
}

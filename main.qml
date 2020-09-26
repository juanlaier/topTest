import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.12
import CTopControl 1.0

Window {
    id: root
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    CTopControl{
        id: topControl
    }

    Rectangle{
        anchors.left: root.left
        width: root.width
        height: root.height - recButton.height
        id: recList
        ListView{
            id: listWindows
            anchors.fill: parent

            highlightFollowsCurrentItem: true

            model: ListModel{
                id: listModel
            }
            delegate: Rectangle{
                id: recItem
                width: root.width
                height: 32
                color: ListView.isCurrentItem ? "silver": "brown"

                Text{
                    id: textname
                    text: name
                    height: parent.height
                    anchors.left: parent.left
                    anchors.leftMargin: 16

                    verticalAlignment: Text.AlignVCenter
                }

                MouseArea{
                    anchors.fill: parent
                    onClicked: {
                        listWindows.currentIndex =index;
                    }
                    onDoubleClicked: {
                        topControl.setWindowOnTop(index, true);
                    }
                }
            }

            ScrollBar.vertical: ScrollBar{
                id: scrollBar
                hoverEnabled: true
                active: hovered || pressed
                policy: ScrollBar.AsNeeded
                orientation: Qt.Vertical
                size: 0.8
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                contentItem: Rectangle  {
                   implicitWidth: 6
                   implicitHeight: 100
                   radius: width / 2
                   color: scrollBar.pressed ? "#81e889" : "#c2f4c6"

                }
            }
        }
    }

    Rectangle{
        id: recButton
        height: 30
        width: root.width
        anchors.top: recList.bottom
        anchors.left: root.left

        color: "blue"

        Text {
            id: btnrefresh
            text: qsTr("Refresh")
            anchors.centerIn: parent

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    console.log("onclicked refresh." + listWindows.currentIndex)
                    //topControl.setWindowOnTop(0, true)
                }
            }
        }

        Text {
            id: btnVisible
            text: qsTr("Visible")
            anchors.right: parent.right

            MouseArea{
                anchors.fill: parent
                onClicked: {
                    var arr = []
                    arr.push("Hello")
                    arr.push("hi")
                    topControl.printStringList(arr);
                }
            }
        }
    }

    Component.onCompleted: {
        var list = topControl.refreshHWNDList()
        for(var i in list)
            listModel.append({"name": list[i]})

        timer.start()
    }

    Timer{
        id: timer
        interval: 1000
        onTriggered: {
            console.log("Hello")
            listModel.clear()

            var list = topControl.arrWindowsName()
            for(var i in list)
                listModel.append({"name":list[i]})
        }
    }
}

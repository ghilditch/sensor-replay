import QtQuick 2.2

Rectangle {
    id: delegate
    property bool selected: ListView.isCurrentItem
    property real itemSize
    width: itemSize
    height: itemSize - 40

    Image {
        //anchors.centerIn: parent
        anchors.fill: delegate
        source: {
            if (type === "Swimming")
            {
                return "images/swimming.jpg";
            }
            else if (type === "Biking")
            {
                return "images/biking.jpg";
            }
            else if (type === "Running")
            {
                return "images/running.jpg";
            }
            else
            {
                return "images/all-sports.png";
            }
        }
    }

    Text {
        id: titleText

        anchors {
            left: parent.left; leftMargin: 20
            right: parent.right; rightMargin: 20
            top: parent.top; topMargin: 20
        }

        font { pixelSize: 12; bold: true }
        text: name
        color: selected ? "#dd1600": "#2b2b2b"
        scale: selected ? 1.15 : 1.0
        Behavior on color { ColorAnimation { duration: 150 } }
        Behavior on scale { PropertyAnimation { duration: 300 } }
    }
    Text {
        id: dateText
        anchors {
            left: parent.left; leftMargin: 20
            right: parent.right; rightMargin: 20
            top: parent.top; topMargin: 40
        }

        font { pixelSize: 10; bold: true }
        text: date
        color: selected ? "#dd1600": "#2b2b2b"
        scale: selected ? 1.15 : 1.0
        Behavior on color { ColorAnimation { duration: 150 } }
        Behavior on scale { PropertyAnimation { duration: 300 } }
    }

    BusyIndicator {
        scale: 0.8
        visible: delegate.ListView.isCurrentItem && mainview.loading
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: delegate
        onClicked: {
            delegate.ListView.view.currentIndex = index
            if (mainview.currentWorkout !== name)
            {
                mainview.currentWorkout = name
                mainview.currentWorkoutId = id
            }
        }
    }
}

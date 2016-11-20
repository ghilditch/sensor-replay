
import QtQuick 2.0

Rectangle {
    id: infoSheet

    width: 200
    height: 450
    anchors.verticalCenter: parent.verticalCenter

    property alias workout: workoutText.name
    property alias time: infoText.time
    property alias sampleCount: infoText.sampleCount
    property alias currentIndex: infoText.currentIndex
    property alias heartrate: infoText.heartrate
    property alias power: infoText.power
    property alias body_angle: infoText.body_angle
    property alias arm_angle_r: infoText.arm_angle_r
    property alias arm_angle_l: infoText.arm_angle_l
    property alias leg_angle_r: infoText.leg_angle_r
    property alias leg_angle_l: infoText.leg_angle_l

    Behavior on opacity { PropertyAnimation {} }

    color: "black"

    Text {
        id: workoutText
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.horizontalCenter: parent.horizontalCenter

        property string name: ""

        font.family: "Helvetica"
        font.pixelSize: 32
        font.weight: Font.Light
        color: "white"

        text: "<p>" + name + "</p>"

    }

    Text {
        id: infoText
        anchors.top: workoutText.bottom
        //anchors.horizontalCenter: parent.horizontalCenter

        property string time: ""
        property int sampleCount: 0
        property int currentIndex: 0
        property string heartrate: ""
        property string power: ""
        property string body_angle: ""
        property string arm_angle_r: ""
        property string arm_angle_l: ""
        property string leg_angle_r: ""
        property string leg_angle_l: ""

        font.family: "Helvetica"
        font.pixelSize: 16
        font.weight: Font.Light
        lineHeight: 0.625 * 16
        lineHeightMode: Text.FixedHeight
        color: "black"

        text: {

                "<p>Sample time:" + time + "</p></br>"
                + "<p>Sample count:" + sampleCount + "</p></br>"
                + "<p>Current index:" + currentIndex + "</p></br>"
                + "<p>Heart Rate:" + heartrate + "</p></br>"
                + "<p>Power:" + power + "</p></br>"
                + "<p>Body Angle:" + body_angle + "</p></br>"
                + "<p>Right Arm Angle:" + arm_angle_r + "</p></br>"
                + "<p>Left Arm Angle:" + arm_angle_l + "</p></br>"
                + "<p>Right Leg Angle:" + leg_angle_r + "</p></br>"
                + "<p>Left Leg Angle:" + leg_angle_l + "</p></br>"
        }
    }

    Rectangle {
        id: openButton
        height: 54
        width: 54
        anchors.bottom: parent.bottom
        //anchors.bottomMargin: width
        x: parent.width / 2 + buttonHorizontalMargin
        color: "transparent"

        Image {
            id: openButtonImage
            source: "qrc:/images/fileopen.png"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: mainview.currentWorkoutId = 0
        }
    }

    BusyIndicator {
        scale: 0.8
        visible: mainview.loading
        anchors.centerIn: parent
    }

}


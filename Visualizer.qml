import QtQuick 2.4
import QtCanvas3D 1.1
import QtQuick.Window 2.2
import Workout 1.0
import "visualizer.js" as GLCode

Window {
    title: qsTr("Sensor-Replay")
    id: mainview
    width: 1280
    height: 768
    visible: true

    // button state
    property bool isHoverEnabled: false

    // slider controls
    property real xLookAtOffset: 0
    property real yLookAtOffset: 0
    property real zLookAtOffset: 0
    property real xCameraOffset: 0
    property real yCameraOffset: 0
    property real zCameraOffset: 0
    property real cameraNear: 0
    property int sliderLength: (width < height) ? width / 2 : height / 2

    // UI setup
    property real textSize: (sliderLength < 320) ? (sliderLength / 20) : 16
    property real controlButtonSize: (height < 768) ? (height / 11) : 70

    // Current workout
    property var currentWorkout: null
    property int currentWorkoutId: -1
    property bool loading: false
    property bool workoutReady: false

    // Orientation
    property bool isPortrait: Screen.primaryOrientation === Qt.PortraitOrientation

    Workouts{
        id: workouts
        onWorkoutLoaded :
        {
            loading = false;
            workoutReady = true;
        }
        onWorkoutLoadError :
        {
            info.workout = "Failed to load workout";
            loading = false;
        }

        onMoveBody : {
           GLCode.updateMovement(GLCode.body_parts.body, x, y, z, q1, q2, q3, q4);
        }
        onMoveupperArmL: {
            GLCode.updateMovement(GLCode.body_parts.upperArmL,x, y, z, q1, q2, q3, q4);
        }
        onMoveupperArmR : {
            GLCode.updateMovement(GLCode.body_parts.upperArmR,x, y, z, q1, q2, q3, q4);
        }
        onMoveforearmL: {
            GLCode.updateMovement(GLCode.body_parts.lowerArmL,x, y, z, q1, q2, q3, q4);
        }

        onMoveforearmR: {
            GLCode.updateMovement(GLCode.body_parts.lowerArmR,x, y, z, q1, q2, q3, q4);
        }
        onMovethighL: {
            GLCode.updateMovement(GLCode.body_parts.upperLegL,x, y, z, q1, q2, q3, q4);
        }
        onMovethighR: {
            GLCode.updateMovement(GLCode.body_parts.upperLegR,x, y, z, q1, q2, q3, q4);
        }
        onMoveshinL: {
            GLCode.updateMovement(GLCode.body_parts.lowerLegL,x, y, z, q1, q2, q3, q4);
        }
        onMoveshinR: {
            GLCode.updateMovement(GLCode.body_parts.lowerLegR,x, y, z, q1, q2, q3, q4);
        }
        onMoveHipToChestZ: {
            GLCode.setBodyPosition(angle);
        }
        onMoveRightThighZ: {
            GLCode.setRightThigh(angle);
        }
        onMoveLeftThighZ: {
            GLCode.setLeftThigh(angle);
        }
        onMoveRightShinZ: {
            GLCode.setRightShin(angle);
        }
        onMoveLeftShinZ: {
            GLCode.setLeftShin(angle);
        }
    }

    onCurrentWorkoutChanged: {
        updateWorkoutInfo();
    }

    onCurrentWorkoutIdChanged: {
        if (currentWorkoutId !== -1)
        {
            loading = true;
            workouts.loadWorkout(currentWorkoutId);
        }
    }

    ListView {
        id: categories
        property int itemWidth: 210

        width: isPortrait ? parent.width : itemWidth
        height: isPortrait ? itemWidth : parent.height
        orientation: isPortrait ? ListView.Horizontal : ListView.Vertical
        anchors.top: parent.top
        model: workouts.workouts
        delegate: WorkoutDelegate { itemSize: categories.itemWidth }
        spacing: 3
    }

    ScrollBar {
        id: listScrollBar

        orientation: isPortrait ? Qt.Horizontal : Qt.Vertical
        height: isPortrait ? 8 : categories.height;
        width: isPortrait ? categories.width : 8
        scrollArea: categories;
        anchors.right: categories.right
    }

    Canvas3D {
        id: canvas3d
        //anchors.fill: parent
        anchors.bottom: parent.bottom
        anchors.top: parent.top
        anchors.right: parent.right
        anchors.left: listScrollBar.right

        onInitializeGL: {
            GLCode.initializeGL(canvas3d, eventSource, mainview);
        }

        onPaintGL: {
            // Update the skeleton
            if (workoutReady)
                updateSensorInfo();
            GLCode.paintGL(canvas3d);
            fpsDisplay.fps = canvas3d.fps;
        }

        onResizeGL: {
            GLCode.onResizeGL(canvas3d);
        }

        ControlEventSource {
            anchors.fill: parent
            focus: true
            id: eventSource
        }
    }

    property int buttonHorizontalMargin: 10
    Rectangle {
        id: playButton
        height: 54
        width: 54
        anchors.bottom: parent.bottom
        anchors.bottomMargin: width
        x: parent.width / 2 - width - buttonHorizontalMargin
        color: "transparent"

        Image {
            id: playButtonImage
            source: "qrc:/images/pausenormal.png"
        }

        MouseArea {
            id: playButtonMouseArea
            anchors.fill: parent
            hoverEnabled: isHoverEnabled
            onClicked: {
                if (mainview.state === 'paused' || mainview.state == 'stopped')
                    mainview.state = 'playing'
                else
                    mainview.state = 'paused'
            }
            onEntered: {
                if (mainview.state === 'playing')
                    playButtonImage.source = "qrc:/images/pausehoverpressed.png"
                else
                    playButtonImage.source = "qrc:/images/playhoverpressed.png"
            }
            onExited: {
                if (mainview.state === 'playing')
                    playButtonImage.source = "qrc:/images/pausenormal.png"
                else
                    playButtonImage.source = "qrc:/images/playnormal.png"
            }
        }
    }

    Rectangle {
        id: stopButton
        height: 54
        width: 54
        anchors.bottom: parent.bottom
        anchors.bottomMargin: width
        x: parent.width / 2 + buttonHorizontalMargin
        color: "transparent"

        Image {
            id: stopButtonImage
            source: "qrc:/images/stopnormal.png"
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: isHoverEnabled
            onClicked: mainview.state = 'stopped'
            onEntered: {
                if (mainview.state !== 'stopped')
                    stopButtonImage.source = "qrc:/images/stophoverpressed.png"
            }
            onExited: {
                if (mainview.state !== 'stopped')
                    stopButtonImage.source = "qrc:/images/stopnormal.png"
            }
        }
    }

    InfoSheet {
        id: info
        width: 400
        anchors.right: parent.right
        anchors.rightMargin: 10
        opacity: 0.5

        workout: "No Workout Selected"
    }

    function updateWorkoutInfo(){
        info.width = 400;
        if (currentWorkout !== null)
        {
            info.workout = currentWorkout;
        }
    }

     function updateSensorInfo() {
        // go to next frame
        workouts.moveNotchSample(currentWorkoutId, 1);
    }

    StyledSlider {
        id: rotateSlider
        anchors.top: parent.top
        anchors.topMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: sliderLength
        value: 0
        minimumValue: -180
        maximumValue: 180
        onValueChanged: GLCode.onRotateCamera(value);
    }
    Text {
        anchors.right: rotateSlider.left
        anchors.verticalCenter: rotateSlider.verticalCenter
        anchors.rightMargin: 10
        font.family: "Helvetica"
        font.pixelSize: textSize
        font.weight: Font.Light
        color: "black"
        text: "Rotate"
    }

    StyledSlider {
        id: scaleSlider
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: sliderLength
        value: 1200
        minimumValue: 1
        maximumValue: 2000
        onValueChanged: GLCode.setScale(value);
    }
    Text {
        anchors.right: scaleSlider.left
        anchors.verticalCenter: scaleSlider.verticalCenter
        anchors.rightMargin: 10
        font.family: "Helvetica"
        font.pixelSize: textSize
        font.weight: Font.Light
        color: "black"
        text: "Playback Speed"
    }

    // FPS display, initially hidden, clicking will show it
    FpsDisplay {
        id: fpsDisplay
        anchors.left: parent.left
        anchors.top: parent.top
        width: 32
        height: 64
        hidden: true
    }

    Component.onCompleted: {
        workouts.loadWorkouts();
    }
}

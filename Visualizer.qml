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
    property bool isHoverEnabled: true

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
    property int currentWorkoutId: -1
    property bool loading: false
    property bool workoutReady: false

    Item{
        id: stateMachine
        state: "stopped"
        states: [
            State {
                name: "playing"
                PropertyChanges {
                    target: playButtonImage
                    source: {
                        if (playButtonMouseArea.containsMouse)
                            "qrc:/images/pausehoverpressed.png"
                        else
                            "qrc:/images/pausenormal.png"
                    }
                }
                PropertyChanges {
                    target: stopButtonImage
                    source: "qrc:/images/stopnormal.png"
                }
            },
            State {
                name: "paused"
                PropertyChanges {
                    target: playButtonImage
                    source: {
                        if (playButtonMouseArea.containsMouse)
                            "qrc:/images/playhoverpressed.png"
                        else
                            "qrc:/images/playnormal.png"
                    }
                }
                PropertyChanges {
                    target: stopButtonImage
                    source: "qrc:/images/stopnormal.png"
                }
            },
            State {
                name: "stopped"
                PropertyChanges {
                    target: playButtonImage
                    source: "qrc:/images/playnormal.png"
                }
                PropertyChanges {
                    target: stopButtonImage
                    source: "qrc:/images/stopdisabled.png"
                }
            }
        ]
    }


    // Orientation
    property bool isPortrait: Screen.primaryOrientation === Qt.PortraitOrientation

    BoneOrientation{

    }
    BoneOrientation{

    }

    Workouts{
        id: workouts
        onWorkoutLoaded : {
            updateWorkoutInfo(wo.name, wo.date, wo.type, wo.sampleCount)
            loading = false;
            workoutReady = true;
        }
        onWorkoutLoadError : {
            info.workout = "Failed to load workout";
            loading = false;
        }
        onUpdateMesh : {
            GLCode.updateMovement (sample.sensorName,
                                   sample.pos_x, sample.pos_y, sample.pos_z,
                                   sample.q1, sample.q2, sample.q3, sample.q4, sample.angle);
            if (sample.sensorName === 'Hip'){
                info.body_angle = sample.angle;
                info.currentIndex = sample.index;
            }else if(sample.sensorName === 'LeftForeArm'){
                info.arm_angle_l = 180 - (sample.angle * -1);
            }else if(sample.sensorName === 'LeftLowerLeg'){
                info.leg_angle_l = 180 - sample.angle;
            }else if(sample.sensorName === 'RightForeArm'){
                info.arm_angle_r = 180 - (sample.angle * -1);
            }else if(sample.sensorName === 'RightLowerLeg'){
                info.leg_angle_r = 180 - sample.angle;
            }
        }
    }

   onCurrentWorkoutIdChanged: {
        if (currentWorkoutId !== -1)
        {
            loading = true;
            workouts.loadWorkout(currentWorkoutId);
        }
    }

    Canvas3D {
        id: canvas3d
        anchors.fill: parent

        onInitializeGL: {
            GLCode.initializeGL(canvas3d, eventSource, mainview);
        }

        onPaintGL: {
            // Update the skeleton
            if (workoutReady && stateMachine.state === 'playing')
            {
                workouts.getNextFrame (currentWorkoutId)
            }
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
        anchors.bottom: scaleSlider.top
        //anchors.bottomMargin: width
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
                if (stateMachine.state === 'paused' || stateMachine.state === 'stopped')
                {
                    stateMachine.state = 'playing'
                    workouts.startPlayback(currentWorkoutId)
                }
                else
                   stateMachine.state = 'paused'
            }
            onEntered: {
                if (stateMachine.state === 'playing')
                    playButtonImage.source = "qrc:/images/pausehoverpressed.png"
                else
                    playButtonImage.source = "qrc:/images/playhoverpressed.png"
            }
            onExited: {
                if (stateMachine.state === 'playing')
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
        anchors.bottom: scaleSlider.top
        //anchors.bottomMargin: width
        x: parent.width / 2 + buttonHorizontalMargin
        color: "transparent"

        Image {
            id: stopButtonImage
            source: "qrc:/images/stopnormal.png"
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: isHoverEnabled
            onClicked: stateMachine.state = 'stopped'
            onEntered: {
                if (stateMachine.state !== 'stopped')
                    stopButtonImage.source = "qrc:/images/stophoverpressed.png"
            }
            onExited: {
                if (stateMachine.state !== 'stopped')
                    stopButtonImage.source = "qrc:/images/stopnormal.png"
            }
        }
    }

    InfoSheet {
        id: info
        width: 400
        anchors.left: parent.left
        anchors.leftMargin: 10
        opacity: 0.5

        workout: "Workout not opened"
    }

    function updateWorkoutInfo(name, date, type, sampleCount){
        info.width = 400;
        info.time = date
        info.workout = name;
        info.sampleCount = sampleCount;
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
        stepSize: 1
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
        text: {

            return "Rotation = " + rotateSlider.value + " degrees";
        }
    }

    StyledSlider {
        id: scaleSlider
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.horizontalCenter: parent.horizontalCenter
        width: sliderLength
        value: 1
        stepSize: 0.5
        minimumValue: 0.5
        maximumValue: 2
        onValueChanged: {
            if (currentWorkoutId !== -1){
                workouts.setPlaybackSpeed(currentWorkoutId, scaleSlider.value)
            }
        }
    }
    Text {
        anchors.right: scaleSlider.left
        anchors.verticalCenter: scaleSlider.verticalCenter
        anchors.rightMargin: 10
        font.family: "Helvetica"
        font.pixelSize: textSize
        font.weight: Font.Light
        color: "black"
        text: {

            return "Playback Speed = " + scaleSlider.value;
        }
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

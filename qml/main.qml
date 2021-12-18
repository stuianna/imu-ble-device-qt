import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick3D 1.15

Window {
    width: 640
    height: 480
    visible: true
    color: "black"
    title: qsTr("IMU-BLE")

    View3D {
        id: view
        anchors.fill: parent
        camera: camera
        renderMode: View3D.Overlay

        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(0, 0, 400)
            eulerRotation.x: 0 
        }

        DirectionalLight {
            eulerRotation.x: 0
        }

        Cube {
            objectName: "cube"
            id: cube
            visible: true
            position: Qt.vector3d(0, 0, 0)
            function update_model(newEuler) {
                eulerRotation = newEuler
            }

            scale.x: 1
            scale.y: 1
            scale.z: 1

            eulerRotation.x: 0
            eulerRotation.y: 0
            eulerRotation.z: 0
        }
    }
}

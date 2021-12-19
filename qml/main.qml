import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.0
import QtQuick3D 1.15

Window {
    id: mainview
    width: 800
    height: 600
    visible: true
    color: "black"
    title: qsTr("IMU-BLE")
    property real fontSize: 10 

    ColumnLayout {
        id: dataview
        objectName: "dataview"
        anchors.top: mainview.top
        anchors.left: mainview.left
        height: mainview.height
        spacing: 0

        function connected() {
            status_ble.text = "BLE: Connected"
        }

        function disconnected() {
            status_ble.text = "BLE: Disconnected"
            accel_x.text = "X:"
            accel_y.text = "Y:"
            accel_z.text = "Z:"
            gyro_x.text = "X:"
            gyro_y.text = "Y:"
            gyro_z.text = "Z:"
            mag_x.text = "X:"
            mag_y.text = "Y:"
            mag_z.text = "Z:"
            euler_x.text = "X:"
            euler_y.text = "Y:"
            euler_z.text = "Z:"
            quarter_w.text = "W:"
            quarter_y.text = "X:"
            quarter_x.text = "Y:"
            quarter_z.text = "Z:"
        }

        function scanning() {
            status_ble.text = "BLE: Scanning"
        }

        GroupBox {
            objectName: "status"
            flat: true
            Layout.fillWidth: true
            Column {
                spacing: 2
                Label {
                    font.pointSize: fontSize
                    font.bold: true
                    text: "Status"
                }

                Label {
                    id: status_ble
                    font.pointSize: fontSize
                    text: "BLE"
                }
            }
        }

        GroupBox {
            objectName: "accel"
            flat: true
            Layout.fillWidth: true
            function accel_update(x, y, z) {
                accel_x.text = "X: " + Number(x.toFixed(3))
                accel_y.text = "Y: " + Number(y.toFixed(3))
                accel_z.text = "Z: " + Number(z.toFixed(3))
            }
            Column {
                spacing: 2
                Label {
                    font.pointSize: fontSize
                    font.bold: true
                    text: "Accelerometer (m/s^2)"
                }

                Label {
                    objectName: "accel_x"
                    id: accel_x
                    font.pointSize: fontSize
                    text: "X:"
                }

                Label {
                    objectName: "accel_y"
                    id: accel_y
                    font.pointSize: fontSize
                    text: "Y:"
                }

                Label {
                    objectName: "accel_z"
                    id: accel_z
                    font.pointSize: fontSize
                    text: "Z:"
                }
            }
        }

        GroupBox {
            objectName: "gyro"
            flat: true
            Layout.fillWidth: true
            function gyro_update(x, y, z) {
                gyro_x.text = "X: " + Number(x.toFixed(3))
                gyro_y.text = "Y: " + Number(y.toFixed(3))
                gyro_z.text = "Z: " + Number(z.toFixed(3))
            }
            Column {
                spacing: 2
                Label {
                    font.pointSize: fontSize
                    font.bold: true
                    text: "Gyroscope (deg/s)"
                }

                Label {
                    objectName: "gyro_x"
                    id: gyro_x
                    font.pointSize: fontSize
                    text: "X:"
                }

                Label {
                    objectName: "gyro_y"
                    id: gyro_y
                    font.pointSize: fontSize
                    text: "Y:"
                }

                Label {
                    objectName: "gyro_z"
                    id: gyro_z
                    font.pointSize: fontSize
                    text: "Z:"
                }
            }
        }

        GroupBox {
            objectName: "mag"
            flat: true
            Layout.fillWidth: true
            function mag_update(x, y, z) {
                mag_x.text = "X: " + Number(x.toFixed(3))
                mag_y.text = "Y: " + Number(y.toFixed(3))
                mag_z.text = "Z: " + Number(z.toFixed(3))
            }
            Column {
                spacing: 2
                Label {
                    font.pointSize: fontSize
                    font.bold: true
                    text: "Magnometer (gauss)"
                }

                Label {
                    objectName: "mag_x"
                    id: mag_x
                    font.pointSize: fontSize
                    text: "X:"
                }

                Label {
                    objectName: "mag_y"
                    id: mag_y
                    font.pointSize: fontSize
                    text: "Y:"
                }

                Label {
                    objectName: "mag_z"
                    id: mag_z
                    font.pointSize: fontSize
                    text: "Z:"
                }
            }
        }
        GroupBox {
            objectName: "euler"
            flat: true
            Layout.fillWidth: true
            function euler_update(vec) {
                euler_x.text = "Roll: " + Number(vec.x.toFixed(3))
                euler_y.text = "Pitch: " + Number(vec.z.toFixed(3))
                euler_z.text = "Yaw: " + Number(vec.y.toFixed(3))
            }
            Column {
                spacing: 2
                Label {
                    font.pointSize: fontSize
                    font.bold: true
                    text: "Euler Orientation (deg)"
                }

                Label {
                    objectName: "euler_x"
                    id: euler_x
                    font.pointSize: fontSize
                    text: "Roll:"
                }

                Label {
                    objectName: "euler_y"
                    id: euler_y
                    font.pointSize: fontSize
                    text: "Pitch:"
                }

                Label {
                    objectName: "euler_z"
                    id: euler_z
                    font.pointSize: fontSize
                    text: "Yaw:"
                }
            }
        }

        GroupBox {
            objectName: "quarternion"
            flat: true
            Layout.fillWidth: true
            function quarternion_update(vec) {
                quarter_x.text = "X: " + Number(vec.x.toFixed(3))
                quarter_y.text = "Y: " + Number(vec.y.toFixed(3))
                quarter_z.text = "Z: " + Number(vec.z.toFixed(3))
                quarter_w.text = "W: " + Number(vec.scalar.toFixed(3))
            }
            Column {
                spacing: 2
                Label {
                    font.pointSize: fontSize
                    font.bold: true
                    text: "Quarternion Orientation"
                }

                Label {
                    objectName: "quarter_x"
                    id: quarter_x
                    font.pointSize: fontSize
                    text: "X:"
                }

                Label {
                    objectName: "quarter_y"
                    id: quarter_y
                    font.pointSize: fontSize
                    text: "Y:"
                }

                Label {
                    objectName: "quarter_z"
                    id: quarter_z
                    font.pointSize: fontSize
                    text: "Z:"
                }

                Label {
                    objectName: "quarter_w"
                    id: quarter_w
                    font.pointSize: fontSize
                    text: "W:"
                }
            }
        }

        Item {
            // spacer item
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    View3D {
        id: view
        objectName: "view3d"
        anchors.top: parent.top
        anchors.right: parent.right;
        width: parent.width - dataview.width
        height: parent.height

        function connected() {
            light.brightness = 100
        }

        function disconnected() {
            light.brightness = 20
        }

        camera: camera
        renderMode: View3D.Overlay

        PerspectiveCamera {
            id: camera
            position: Qt.vector3d(0, 0, 400)
            eulerRotation.x: 0 
        }

        DirectionalLight {
            id: light
            eulerRotation.x: 0
        }

        Cube {
            objectName: "cube"
            id: cube
            visible: true
            position: Qt.vector3d(0, 0, 0)
            function update_model(newQuarternion) {
                rotation = newQuarternion
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

import QtQuick 2.15
import QtQuick3D 1.15

Node {
    id: rootNode

    PointLight {
        id: light
        x: 407.625
        y: 590.386
        z: -100.545
        eulerRotation.x: -42.4664
        eulerRotation.y: 72.7792
        eulerRotation.z: -169.256
        color: "#ffffffff"
        quadraticFade: 2.22222e-07
    }

    PerspectiveCamera {
        id: camera
        x: 735.889
        y: 495.831
        z: 692.579
        eulerRotation.x: -153.559
        eulerRotation.y: -46.6919
        eulerRotation.z: -180
        fieldOfView: 39.5978
        fieldOfViewOrientation: Camera.Horizontal
    }

    Model {
        id: cube
        eulerRotation.x: -90
        scale.x: 100
        scale.y: 60
        scale.z: 15
        source: "meshes/cube.mesh"

        DefaultMaterial {
            id: cyan_material
            diffuseColor: "#ff01ccb5"
        }

        DefaultMaterial {
            id: red_material
            diffuseColor: "#ffcc0006"
        }

        DefaultMaterial {
            id: yellow_material
            diffuseColor: "#ffe6ae00"
        }
        materials: [
            cyan_material,
            red_material,
            yellow_material
        ]
    }
}

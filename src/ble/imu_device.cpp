#include <qbluetoothdevicediscoveryagent.h>
#include <qlowenergycontroller.h>

#include <QDateTime>
#include <QtEndian>
#include <ble/imu_device.hpp>
#include <constants/uuid.hpp>

ImuDevice::ImuDevice(QBluetoothDeviceInfo* device) : _device(device) {
  _controller = QLowEnergyController::createCentral(*_device);
  _controller->setRemoteAddressType(QLowEnergyController::PublicAddress);

  QObject::connect(_controller, &QLowEnergyController::serviceDiscovered, this, &ImuDevice::serviceDiscovered);
  QObject::connect(_controller, &QLowEnergyController::discoveryFinished, this, &ImuDevice::serviceScanDone);
  QObject::connect(_controller, static_cast<void (QLowEnergyController::*)(QLowEnergyController::Error)>(&QLowEnergyController::error),
                   this, [this](QLowEnergyController::Error error) {
                     Q_UNUSED(error);
                     Q_UNUSED(this);
                   });
  QObject::connect(_controller, &QLowEnergyController::connected, this, [this]() {
    Q_UNUSED(this);
    _controller->discoverServices();
  });
  QObject::connect(_controller, &QLowEnergyController::disconnected, this, [this]() {
    Q_UNUSED(this);
    qDebug() << "Device disconnected";
    emit disconnected();
  });
}

ImuDevice::~ImuDevice() {
  for(auto service: _services) {
    if(service) {
      delete service;
    }
  }
  delete _device;
  delete _controller;
}

void ImuDevice::serviceDiscovered(const QBluetoothUuid& gatt) {
  qDebug() << "Found service: " << gatt.toString();
  Q_UNUSED(gatt);
}

void ImuDevice::serviceScanDone() {
  _services[SYS_STATE] =
  new NotificationService(_controller, QBluetoothUuid(QString(UUID_SERVICE_ACC)), QBluetoothUuid(QString(UUID_CHARACTERISTIC)));
  QObject::connect(_services[SYS_STATE], &NotificationService::ready, this, &ImuDevice::serviceReady);
  QObject::connect(_services[SYS_STATE], &NotificationService::dataAvailable, this, &ImuDevice::sysStateDataAvailable);
  _serviceScanIndex = 0;
  _services[_serviceScanIndex]->scan();
}

void ImuDevice::serviceReady() {
  _serviceScanIndex++;
  if(_serviceScanIndex != Services::COUNT) {
    _services[_serviceScanIndex]->scan();
    return;
  }
  for(auto service: _services) {
    service->enableNotification(true);
  }
  qDebug() << "Device connected";
  emit connected();
}

void ImuDevice::sysStateDataAvailable(const QByteArray& data) {
  auto readings = reinterpret_cast<const float*>(data.constData());
  emit accelerometer(readings[0], readings[1], readings[2]);
  emit gyroscope(readings[3], readings[4], readings[5]);
  emit magnometer(readings[6], readings[7], readings[8]);
  emit euler(readings[9], readings[10], readings[11]);
  emit quarternions(readings[12], readings[13], readings[14], readings[15]);
}

void ImuDevice::connect() {
  _controller->connectToDevice();
}

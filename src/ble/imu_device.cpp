#include <ble/imu_device.hpp>
#include <QDateTime>
#include <QtEndian>
#include <qbluetoothdevicediscoveryagent.h>
#include <qlowenergycontroller.h>

ImuDevice::ImuDevice(QBluetoothDeviceInfo *device) : _device(device) {
  _controller = QLowEnergyController::createCentral(*_device);
  _controller->setRemoteAddressType(QLowEnergyController::PublicAddress);

  QObject::connect(_controller, &QLowEnergyController::serviceDiscovered, this,
                   &ImuDevice::serviceDiscovered);
  QObject::connect(_controller, &QLowEnergyController::discoveryFinished, this,
                   &ImuDevice::serviceScanDone);
  QObject::connect(
      _controller,
      static_cast<void (QLowEnergyController::*)(QLowEnergyController::Error)>(
          &QLowEnergyController::error),
      this, [this](QLowEnergyController::Error error) {
        Q_UNUSED(error);
        Q_UNUSED(this);
      });
  QObject::connect(_controller, &QLowEnergyController::connected, this,
                   [this]() {
                     Q_UNUSED(this);
                     _controller->discoverServices();
                   });
  QObject::connect(_controller, &QLowEnergyController::disconnected, this,
                   [this]() {
                     Q_UNUSED(this);
                     emit disconnected();
                   });
}

ImuDevice::~ImuDevice() {
  delete _controller;
  //    delete _hr_service;
}

void ImuDevice::serviceDiscovered(const QBluetoothUuid &gatt) {
  qDebug() << gatt.toString();
  Q_UNUSED(gatt);
}

void ImuDevice::serviceScanDone() {
  _services[ACC] = new NotificationService(
      _controller,
      QBluetoothUuid(QString("{12345678-1234-5678-1234-56789abcdefA}")),
      QBluetoothUuid(QString("{12345678-1234-5678-1234-56789abcdef1}")));
  _services[GYR] = new NotificationService(
      _controller,
      QBluetoothUuid(QString("{12345678-1234-5678-1234-56789abcdee0}")),
      QBluetoothUuid(QString("{12345678-1234-5678-1234-56789abcdef1}")));
  _services[MAG] = new NotificationService(
      _controller,
      QBluetoothUuid(QString("12345678-1234-5678-1234-56789abcded0")),
      QBluetoothUuid(QString("12345678-1234-5678-1234-56789abcdef1")));
  QObject::connect(_services[ACC], &NotificationService::ready, this,
                   &ImuDevice::serviceReady);
  QObject::connect(_services[ACC], &NotificationService::dataAvailable, this,
                   &ImuDevice::accelerometerDataAvailable);
  QObject::connect(_services[GYR], &NotificationService::ready, this,
                   &ImuDevice::serviceReady);
  QObject::connect(_services[GYR], &NotificationService::dataAvailable, this,
                   &ImuDevice::gyroscopeDataAvailable);
  QObject::connect(_services[MAG], &NotificationService::ready, this,
                   &ImuDevice::serviceReady);
  QObject::connect(_services[MAG], &NotificationService::dataAvailable, this,
                   &ImuDevice::magnometerDataAvailable);
  _serviceScanIndex = 0;
  _services[_serviceScanIndex]->scan();
}

void ImuDevice::serviceReady() {
  _serviceScanIndex++;
  if (_serviceScanIndex != Services::COUNT) {
    _services[_serviceScanIndex]->scan();
    return;
  }
  for (auto service : _services) {
    service->enableNotification(true);
  }
  emit connected();
}

void ImuDevice::accelerometerDataAvailable(const QByteArray &data) {
  auto readings = reinterpret_cast<const float *>(data.constData());
  emit accelerometer(readings[0], readings[1], readings[2]);
}

void ImuDevice::gyroscopeDataAvailable(const QByteArray &data) {
  auto readings = reinterpret_cast<const float *>(data.constData());
  emit gyroscope(readings[0], readings[1], readings[2]);
}

void ImuDevice::magnometerDataAvailable(const QByteArray &data) {
  auto readings = reinterpret_cast<const float *>(data.constData());
  emit magnometer(readings[0], readings[1], readings[2]);
}

void ImuDevice::connect() { _controller->connectToDevice(); }

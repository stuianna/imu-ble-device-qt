#include <qmath.h>

#include <QVector3D>
#include <worker.hpp>

Worker::Worker(const QString deviceName) : _deviceName(deviceName) {
}

Worker::~Worker() {
  delete _scanner;
}

void Worker::start() {
  _scanner = new DeviceScanner();
  _scanner->addDeviceNameFilter(_deviceName);
  _scanner->setMaximumDiscoveredDeviceCount(1);
  connect(_scanner, &DeviceScanner::scanComplete, this, &Worker::_deviceScanComplete);
  connect(_scanner, &DeviceScanner::deviceFound, this, &Worker::_deviceFound);
  emit deviceScanStarted();
  _scanner->scan();
}

void Worker::_deviceScanComplete() {
  emit deviceScanComplete();
}

void Worker::_deviceFound(QBluetoothDeviceInfo* device) {
  _device = new ImuDevice(device);
  connect(_device, &ImuDevice::connected, this, &Worker::_connected);
  connect(_device, &ImuDevice::disconnected, this, &Worker::_disconnected);
  connect(_device, &ImuDevice::accelerometer, this, &Worker::_accelerometer);
  connect(_device, &ImuDevice::gyroscope, this, &Worker::_gyroscope);
  connect(_device, &ImuDevice::magnometer, this, &Worker::_magnometer);
  connect(_device, &ImuDevice::euler, this, &Worker::_euler);
  _device->connect();
}

void Worker::_connected() {
  emit deviceConnected();
}

void Worker::_disconnected() {
  emit deviceDisconnected();
}


void Worker::_accelerometer(float x, float y, float z) {
  emit accelAvailable(x, y, z);
}

void Worker::_gyroscope(float x, float y, float z) {
  emit gyroAvailable(x, y, z);
}

void Worker::_magnometer(float x, float y, float z) {
  emit magnoAvailable(x, y, z);
}

void Worker::_euler(float x, float y, float z) {
  emit eulerAvailable(QVector3D(y, z, x));
}
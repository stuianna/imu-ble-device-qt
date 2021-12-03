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

  static double roll = (atan2(y, z) * 57.3);
  static double pitch = (atan2((-x), sqrt(y * y + z * z)) * 57.3);
  double yaw = 0;

  roll = (roll * 0.8) + (atan2(y, z) * 57.3) * 0.2;
  pitch = (pitch * 0.8) + (atan2((-x), sqrt(y * y + z * z)) * 57.3) * 0.2;
  qDebug() << roll;

  emit basic_euler(QVector3D(-pitch, yaw, roll));
}

void Worker::_gyroscope(float x, float y, float z) {
  emit gyroAvailable(x, y, z);
}

void Worker::_magnometer(float x, float y, float z) {
  emit magnoAvailable(x, y, z);
}

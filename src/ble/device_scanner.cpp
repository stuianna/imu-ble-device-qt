#include <QDebug>
#include <QSharedPointer>
#include <QThread>
#include <ble/device_scanner.hpp>

DeviceScanner::DeviceScanner() {
  _discoveredDevices.empty();
  _deviceNameFilters.empty();
  _maximumDeviceCount = 0;
  _storeUnnamedDevices = false;
}

DeviceScanner::~DeviceScanner() {
  _discoveryAgent->stop();
  delete _discoveryAgent;
}

void DeviceScanner::addDeviceNameFilter(const QString &name) {
  if (!_deviceNameFilters.contains(name)) {
    _deviceNameFilters.append(name);
  }
}

QList<QString> DeviceScanner::getDeviceNameFilters() {
  return _deviceNameFilters;
}

void DeviceScanner::setMaximumDiscoveredDeviceCount(const size_t count) {
  _maximumDeviceCount = count;
}

size_t DeviceScanner::getMaximumDiscoveredDeviceCount() {
  return _maximumDeviceCount;
}

bool DeviceScanner::getStoreUnnamedDevices() { return _storeUnnamedDevices; }

void DeviceScanner::setStoreUnnamedDevices(bool store) {
  _storeUnnamedDevices = store;
}

void DeviceScanner::scan() {
  _discoveryAgent = new QBluetoothDeviceDiscoveryAgent();
  QObject::connect(_discoveryAgent,
                   &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this,
                   &DeviceScanner::addDevice);
  QObject::connect(_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::finished,
                   this, &DeviceScanner::scanFinished);
  QObject::connect(_discoveryAgent, &QBluetoothDeviceDiscoveryAgent::canceled,
                   this, &DeviceScanner::scanFinished);
  qDebug() << "Device scan started.";
  emit scanStarted();
  _discoveredDevices.clear();
  _discoveryAgent->start();
}

void DeviceScanner::stopScan() { _discoveryAgent->stop(); }

bool DeviceScanner::_isDeviceUnnamed(const QString &name,
                                     const QBluetoothAddress &address) {
  QString n = name;
  return (n.replace('-', ':') == address.toString());
}

bool DeviceScanner::_isDeviceNameListed(const QString &name) {
  if (_deviceNameFilters.empty()) {
    return true;
  }
  return _deviceNameFilters.contains(name);
}

void DeviceScanner::addDevice(const QBluetoothDeviceInfo &info) {
  qDebug() << "Found device: " << info.name() << " - "
           << info.address().toString();
  if (_isDeviceUnnamed(info.name(), info.address()) && !_storeUnnamedDevices) {
    return;
  }

  if (!_isDeviceNameListed(info.name())) {
    return;
  }

  auto newDevice = new QBluetoothDeviceInfo(info);
  if (!_discoveredDevices.contains(newDevice)) {
    qDebug() << "Found new matching device: " << info.name() << " - "
             << info.address().toString();
    _discoveredDevices.append(newDevice);
  } else {
    delete newDevice;
  }
  if (_discoveredDevices.length() == static_cast<int>(_maximumDeviceCount)) {
    stopScan();
  }
}

void DeviceScanner::scanFinished() {
  qDebug() << "Device scan finished.";
  if (_discoveredDevices.isEmpty()) {
    emit scanComplete();
  }
  for (auto device : qAsConst(_discoveredDevices)) {
    emit deviceFound(device);
  }
}

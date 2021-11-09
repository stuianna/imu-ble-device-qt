#ifndef __BLE_DEVICE_SCANNER_HPP
#define __BLE_DEVICE_SCANNER_HPP

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QList>
#include <QString>

class DeviceScanner : public QObject {
  Q_OBJECT
public:
  DeviceScanner();
  ~DeviceScanner();
  void addDeviceNameFilter(const QString &name);
  QList<QString> getDeviceNameFilters();
  void setMaximumDiscoveredDeviceCount(const size_t count);
  size_t getMaximumDiscoveredDeviceCount();
  bool getStoreUnnamedDevices();
  void setStoreUnnamedDevices(bool store);
  void scan();
  void stopScan();

signals:
  void scanStarted();
  void deviceFound(QBluetoothDeviceInfo *device);
  void scanComplete();

private:
  QBluetoothDeviceDiscoveryAgent *_discoveryAgent = nullptr;
  QList<QBluetoothDeviceInfo *> _discoveredDevices;
  QList<QString> _deviceNameFilters;
  size_t _maximumDeviceCount;
  bool _storeUnnamedDevices;

  bool _isDeviceUnnamed(const QString &name, const QBluetoothAddress &address);
  bool _isDeviceNameListed(const QString &name);

private slots:
  void scanFinished();
  void addDevice(const QBluetoothDeviceInfo &);
};

#endif // __BLE_DEVICE_SCANNER_HPP
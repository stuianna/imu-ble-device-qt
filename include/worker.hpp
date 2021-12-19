#ifndef __WORKER_HPP
#define __WORKER_HPP

#include <ble/device_scanner.hpp>
#include <ble/imu_device.hpp>
#include <QQuaternion>

class Worker : public QObject {
  Q_OBJECT
 public:
  explicit Worker(const QString _deviceName);
  ~Worker();
  void start();

 signals:
  void deviceScanStarted();
  void deviceScanComplete();
  void deviceConnected();
  void deviceDisconnected();
  void accelAvailable(QVariant x, QVariant y, QVariant z);
  void gyroAvailable(QVariant x, QVariant y, QVariant z);
  void magnoAvailable(QVariant x, QVariant y, QVariant z);
  void eulerAvailable(QVariant);
  void quarternionsAvailable(QVariant);

 private:
  const QString _deviceName;
  DeviceScanner* _scanner;
  ImuDevice* _device;
  void _startScan();

 private slots:
  void _deviceScanComplete();
  void _deviceFound(QBluetoothDeviceInfo* device);
  void _connected();
  void _disconnected();
  void _accelerometer(float x, float y, float z);
  void _gyroscope(float x, float y, float z);
  void _magnometer(float x, float y, float z);
  void _euler(float x, float y, float z);
  void _quarternions(float w, float x, float y, float z);
};

#endif  // __WORKER_HPP
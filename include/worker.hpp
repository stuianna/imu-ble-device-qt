#ifndef __WORKER_HPP
#define __WORKER_HPP

#include <ble/device_scanner.hpp>
#include <ble/imu_device.hpp>

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
  void accelAvailable(float x, float y, float z);
  void gyroAvailable(float x, float y, float z);
  void magnoAvailable(float x, float y, float z);

 private:
  const QString _deviceName;
  DeviceScanner* _scanner;
  ImuDevice* _device;
 private slots:
  void _deviceScanComplete();
  void _deviceFound(QBluetoothDeviceInfo* device);
  void _connected();
  void _disconnected();
  void _accelerometer(float x, float y, float z);
  void _gyroscope(float x, float y, float z);
  void _magnometer(float x, float y, float z);
};

#endif  // __WORKER_HPP
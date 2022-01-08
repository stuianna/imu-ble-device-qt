#ifndef __BLE_IMU_DEVICE_HPP
#define __BLE_IMU_DEVICE_HPP

#include <QLowEnergyService>
#include <ble/notification_service.hpp>
QT_FORWARD_DECLARE_CLASS(QLowEnergyController)
QT_FORWARD_DECLARE_CLASS(QBluetoothDeviceInfo)

class ImuDevice : public QObject {
  Q_OBJECT
 public:
  explicit ImuDevice(QBluetoothDeviceInfo* device);
  ~ImuDevice();
  void connect();

 signals:
  void connected();
  void disconnected();
  void accelerometer(float x, float y, float z);
  void gyroscope(float x, float y, float z);
  void magnometer(float x, float y, float z);
  void euler(float x, float y, float z);
  void quarternions(float w, float x, float y, float z);

 private:
  enum Services {
    SYS_STATE,
    COUNT,
  };
  static_assert(SYS_STATE == 0, "Accelerometer seervice is expected to have index 0");
  QBluetoothDeviceInfo* _device = nullptr;
  QLowEnergyController* _controller = nullptr;
  NotificationService* _services[Services::COUNT];
  size_t _serviceScanIndex;

 private slots:
  void serviceDiscovered(const QBluetoothUuid& gatt);
  void serviceScanDone();
  void serviceReady();
  void sysStateDataAvailable(const QByteArray& data);
};

#endif  // __BLE_IMU_DEVICE_HPP
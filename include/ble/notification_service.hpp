#ifndef __BLE_NOTIFICATION_SERVICE_HPP
#define __BLE_NOTIFICATION_SERVICE_HPP

#include <QLowEnergyService>
#include <QObject>

QT_FORWARD_DECLARE_CLASS(QLowEnergyController)

class NotificationService : public QObject {
  Q_OBJECT
public:
  NotificationService(QLowEnergyController *controller,
                      QBluetoothUuid serviceUUID,
                      QBluetoothUuid characteristicUUID,
                      QObject *parent = nullptr);
  ~NotificationService();
  void enableNotification(bool enable);
  void scan();

signals:
  void ready();
  void dataAvailable(const QByteArray &data);

private:
  QLowEnergyController *_controller;
  QBluetoothUuid _serviceUUID;
  QBluetoothUuid _characteristicUUID;
  QLowEnergyService *_service;
  bool _notificationEnabled;

private slots:
  void updateCharacteristic(const QLowEnergyCharacteristic &c,
                            const QByteArray &value);
  void serviceStateChanged(QLowEnergyService::ServiceState s);
};

#endif // __BLE_NOTIFICATION_SERVICE_HPP

#include <QLowEnergyController>
#include <ble/notification_service.hpp>

NotificationService::NotificationService(QLowEnergyController *controller,
                                         QBluetoothUuid serviceUUID,
                                         QBluetoothUuid characteristicUUID,
                                         QObject *parent)
    : _controller(controller), _serviceUUID(serviceUUID),
      _characteristicUUID(characteristicUUID) {

  Q_UNUSED(parent);
  _notificationEnabled = false;
  _service = _controller->createServiceObject(_serviceUUID);
  if (_service) {
    QObject::connect(_service, &QLowEnergyService::stateChanged, this,
                     &NotificationService::serviceStateChanged);
    QObject::connect(_service, &QLowEnergyService::characteristicChanged, this,
                     &NotificationService::updateCharacteristic);
  } else {
    qDebug() << "Failed to find expected characteristic UUIDs";
    // Handle no service
  }
}

NotificationService::~NotificationService() { delete _service; }

void NotificationService::serviceStateChanged(
    QLowEnergyService::ServiceState s) {
  switch (s) {
  case QLowEnergyService::DiscoveringServices:
    break;
  case QLowEnergyService::ServiceDiscovered: {
    const QLowEnergyCharacteristic characteristic =
        _service->characteristic(_characteristicUUID);
    if (!characteristic.isValid()) {
      // Handle no valid characteristic
      qDebug() << "No valid characteristic discovered for service. Expected: "
               << _characteristicUUID.toString();
      break;
    }
    QLowEnergyDescriptor m_notificationDesc = characteristic.descriptor(
        QBluetoothUuid::ClientCharacteristicConfiguration);
    if (m_notificationDesc.isValid()) {
      _service->writeDescriptor(m_notificationDesc,
                                QByteArray::fromHex("0100"));
    } else {
      qDebug() << "Notifications not supported by characteristic.";
      break;
    }
    emit ready();
    break;
  }
  default:
    // nothing for now
    break;
  }
}

void NotificationService::updateCharacteristic(
    const QLowEnergyCharacteristic &c, const QByteArray &value) {
  if (_notificationEnabled) {
    if (c.uuid() != _characteristicUUID) {
      qDebug() << "Got unexpected characteristic " << c.uuid().toString();
    }
    emit dataAvailable(value);
  }
}

void NotificationService::scan() {
  if (_service) {
    _service->discoverDetails();
  }
}

void NotificationService::enableNotification(bool enable) {
  _notificationEnabled = enable;
}

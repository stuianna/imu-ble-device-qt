#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <ble/device_scanner.hpp>
#include <constants/ble.hpp>
#include <worker.hpp>

int main(int argc, char* argv[]) {
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
  QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;
  const QUrl url(QStringLiteral("qrc:/main.qml"));
  QObject::connect(
  &engine, &QQmlApplicationEngine::objectCreated, &app,
  [url](QObject* obj, const QUrl& objUrl) {
    if(!obj && url == objUrl)
      QCoreApplication::exit(-1);
  },
  Qt::QueuedConnection);
  engine.load(url);

  auto worker = new Worker(BT_DEVICE_NAME);

  QObject* rootObject = engine.rootObjects().at(0);
  QObject* cube = rootObject->findChild<QObject*>("cube");
  QObject* view3d = rootObject->findChild<QObject*>("view3d");
  QObject* dataview = rootObject->findChild<QObject*>("dataview");
  QObject* accel = rootObject->findChild<QObject*>("accel");
  QObject* gyro = rootObject->findChild<QObject*>("gyro");
  QObject* mag = rootObject->findChild<QObject*>("mag");
  QObject* euler = rootObject->findChild<QObject*>("euler");
  QObject::connect(worker, SIGNAL(deviceConnected()), view3d, SLOT(connected()));
  QObject::connect(worker, SIGNAL(deviceDisconnected()), view3d, SLOT(disconnected()));
  QObject::connect(worker, SIGNAL(deviceConnected()), dataview, SLOT(connected()));
  QObject::connect(worker, SIGNAL(deviceDisconnected()), dataview, SLOT(disconnected()));
  QObject::connect(worker, SIGNAL(deviceScanStarted()), dataview, SLOT(scanning()));
  QObject::connect(worker, SIGNAL(eulerAvailable(QVariant)), cube, SLOT(update_model(QVariant)));
  QObject::connect(worker, SIGNAL(accelAvailable(QVariant, QVariant, QVariant)), accel, SLOT(accel_update(QVariant, QVariant, QVariant)));
  QObject::connect(worker, SIGNAL(gyroAvailable(QVariant, QVariant, QVariant)), gyro, SLOT(gyro_update(QVariant, QVariant, QVariant)));
  QObject::connect(worker, SIGNAL(magnoAvailable(QVariant, QVariant, QVariant)), mag, SLOT(mag_update(QVariant, QVariant, QVariant)));
  QObject::connect(worker, SIGNAL(eulerAvailable(QVariant)), euler, SLOT(euler_update(QVariant)));

  worker->start();
  return app.exec();
}

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

  QObject *rootObject = engine.rootObjects().at(0);
  QObject *cube = rootObject->findChild<QObject*>("cube");
  QObject::connect(worker, SIGNAL(basic_euler(QVariant)), cube, SLOT(update_model(QVariant)));

  worker->start();
  return app.exec();
}

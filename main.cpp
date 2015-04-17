#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "Language.h"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  Language language;

  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("app_language", QVariant::fromValue(&language));
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  return app.exec();
}

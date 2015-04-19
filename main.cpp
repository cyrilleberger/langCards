#include <QDebug>
#include <QDir>
#include <QStandardPaths>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSqlDatabase>

#include "Database.h"
#include "Language.h"

int main(int argc, char *argv[])
{
  QGuiApplication app(argc, argv);

  QSqlDatabase sql_db = QSqlDatabase::addDatabase("QSQLITE");
  QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
  sql_db.setDatabaseName(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + "/langCards.db");
  LangCardsDB::Database lang_db(sql_db);

  Language language(&lang_db);

  QQmlApplicationEngine engine;
  engine.rootContext()->setContextProperty("app_language", QVariant::fromValue(&language));
  engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

  return app.exec();
}

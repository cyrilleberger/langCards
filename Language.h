#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>
#include <QStringList>

#include "fWord.h"

#include "LanguageBase.h"

class Language : public LanguageBase
{
  Q_OBJECT
public:
  explicit Language(LangCardsDB::Database* _db, QObject *parent = 0);

  Q_INVOKABLE void userAnswer(const QString& _answer, qreal _time);
  Q_INVOKABLE QString databaseDump() const;
  Q_INVOKABLE void reload();
  Q_INVOKABLE void nextWord();
private:
  QList<fWord> m_available_words;
  qreal m_totalProbability, m_total_available_probability;
};

#endif // LANGUAGE_H

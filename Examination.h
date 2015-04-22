#ifndef EXAMINATION_H
#define EXAMINATION_H

#include <QObject>

#include "LanguageBase.h"

class Examination : public LanguageBase
{
  Q_OBJECT
public:
  explicit Examination(LangCardsDB::Database* _db, QObject *parent = 0);

  Q_INVOKABLE void userAnswer(const QString& _answer, qreal _time);
  Q_INVOKABLE void reload();
  Q_INVOKABLE void nextWord();
protected:
  int m_index;
};

#endif // EXAMINATION_H

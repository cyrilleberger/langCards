#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>
#include <QStringList>

#include "Words.h"
#include "Word.h"
#include "fWord.h"

class Language : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString wordToGuess READ wordToGuess NOTIFY wordChanged)
  Q_PROPERTY(QString correctAnswer READ correctAnswer NOTIFY wordChanged)
  Q_PROPERTY(QStringList answers READ answers NOTIFY wordChanged)
public:
  explicit Language(LangCardsDB::Database* _db, QObject *parent = 0);

  QString wordToGuess() const { return m_wordToGuess; }
  QStringList answers() const { return m_answers; }
  QString correctAnswer() const { return m_correctAnswer; }

  Q_INVOKABLE void userAnswer(const QString& _answer, qreal _time);
signals:
  void wordChanged();
public slots:
  void nextWord();
private:
  LangCardsDB::Database* m_db;
  LangCardsDB::Words m_words;
  QList<LangCardsDB::Word> m_currentWords;
  LangCardsDB::Word* m_currentWord;
  qreal m_totalProbability;
  QString m_wordToGuess, m_correctAnswer;
  QStringList m_answers;
  QList<fWord> m_basic, m_common, m_rare;
};

#endif // LANGUAGE_H

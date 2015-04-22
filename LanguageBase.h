#ifndef LANGUAGEBASE_H
#define LANGUAGEBASE_H

#include <QObject>

#include "Words.h"
#include "Word.h"

class LanguageBase : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString wordToGuess READ wordToGuess NOTIFY wordChanged)
  Q_PROPERTY(QString correctAnswer READ correctAnswer NOTIFY wordChanged)
  Q_PROPERTY(QStringList answers READ answers NOTIFY wordChanged)
public:
  explicit LanguageBase(LangCardsDB::Database* _db, QObject *parent = 0);

  QString wordToGuess() const { return m_wordToGuess; }
  QStringList answers() const { return m_answers; }
  QString correctAnswer() const { return m_correctAnswer; }
signals:
  void wordChanged();
protected:
  void updateWords();
protected:
  LangCardsDB::Database* m_db;
  LangCardsDB::Words m_words;
  QList<LangCardsDB::Word> m_currentWords;
  LangCardsDB::Word* m_currentWord;
  QString m_wordToGuess, m_correctAnswer;
  QStringList m_answers;
};

#endif // LANGUAGEBASE_H

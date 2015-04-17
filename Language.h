#ifndef LANGUAGE_H
#define LANGUAGE_H

#include <QObject>
#include <QStringList>
#include "WordsSerie.h"

class Language : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString wordToGuess READ wordToGuess NOTIFY wordChanged)
  Q_PROPERTY(QString correctAnswer READ correctAnswer NOTIFY wordChanged)
  Q_PROPERTY(QStringList answers READ answers NOTIFY wordChanged)
public:
  explicit Language(QObject *parent = 0);

  QString wordToGuess() const { return m_wordToGuess; }
  QStringList answers() const { return m_answers; }
  QString correctAnswer() const { return m_correctAnswer; }

signals:
  void wordChanged();
public slots:
  void nextWord();
private:
  QString m_wordToGuess, m_correctAnswer;
  QStringList m_answers;
  WordsSerie m_basic, m_common, m_rare;
};

#endif // LANGUAGE_H

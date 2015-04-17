#ifndef WORDSSERIE_H
#define WORDSSERIE_H

#include <QList>
#include <QPair>

#include "Word.h"

class WordsSerie
{
public:
  WordsSerie() {}
  WordsSerie(const QList<Word>& _words, qreal m_total_probability);
  void selectWord(QString& _quest, QString& _answer, QStringList& _answers);
  qreal total_probability() const { return m_total_probability; }
private:
  QList<Word> m_words;
  qreal m_total_probability;
};

#endif // WORDSSERIE_H

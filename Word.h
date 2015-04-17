#ifndef WORD_H
#define WORD_H

#include <QString>

struct Word
{
  QString word_lang1, word_lang2;
  qreal probability;
  qreal probability_base;
  bool operator ==(const Word& _rhs)
  {
    return word_lang1 == _rhs.word_lang1 and word_lang2 == _rhs.word_lang2;
  }
  int lang1_success, lang1_failed, lang2_success, lang2_failed;
};

#endif // WORD_H

#include "WordsSerie.h"

#include <algorithm>

#include <QDebug>
#include <QStringList>

WordsSerie::WordsSerie(const QList<Word>& _words, qreal _total_probability)
  : m_words(_words), m_total_probability(_total_probability)
{

}

template< class RandomIt >
void random_shuffle( RandomIt first, RandomIt last )
{
    typename std::iterator_traits<RandomIt>::difference_type i, n;
    n = last - first;
    for (i = n-1; i > 0; --i) {
        using std::swap;
        swap(first[i], first[std::rand() % (i+1)]);
    }
}

void WordsSerie::selectWord(QString& _quest, QString& _answer, QStringList& _answers)
{
  qreal random_word = qrand() / qreal(RAND_MAX);
  Word word;
  qreal proba = 0.0;

  for(int i = 0; i < m_words.length(); ++i)
  {
    proba += m_words[i].probability / m_total_probability;
    if(random_word < proba)
    {
      word = m_words[i];
      break;
    }
  }

  QList<Word> words;
  words.append(word);
  for(int i = 0; i < 3; ++i)
  {
    qreal random_word = qrand() / qreal(RAND_MAX);
    words.append(m_words[random_word * m_words.length()]);
  }

  random_shuffle(words.begin(), words.end());

  if(qrand() < RAND_MAX / 2)
  {
    _quest  = word.word_lang1;
    _answer = word.word_lang2;
    for(int i = 0; i < 4; ++i)
    {
      _answers.append(words[i].word_lang2);
    }
  } else {
    _quest  = word.word_lang2;
    _answer = word.word_lang1;
    for(int i = 0; i < 4; ++i)
    {
      _answers.append(words[i].word_lang1);
    }
  }
}

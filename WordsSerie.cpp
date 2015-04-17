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

void WordsSerie::selectWord(Word*& _currentWord, QString& _quest, QString& _answer, QStringList& _answers)
{
  qreal random_word = qrand() / qreal(RAND_MAX);
  qreal proba = 0.0;

  for(int i = 0; i < m_words.length(); ++i)
  {
    proba += m_words[i].probability / m_total_probability;
    if(random_word < proba)
    {
      _currentWord = &m_words[i];
      break;
    }
  }

  QList<Word> words;
  words.append(*_currentWord);
  for(int i = 0; i < 3; ++i)
  {
    qreal random_word = qrand() / qreal(RAND_MAX);
    Word newword = m_words[random_word * m_words.length()];
    if(words.contains(newword))
    {
      --i;
    } else {
      words.append(newword);
    }
  }

  random_shuffle(words.begin(), words.end());

  if(qrand() < RAND_MAX / 2)
  {
    _quest  = _currentWord->word_lang1;
    _answer = _currentWord->word_lang2;
    for(int i = 0; i < 4; ++i)
    {
      _answers.append(words[i].word_lang2);
    }
  } else {
    _quest  = _currentWord->word_lang2;
    _answer = _currentWord->word_lang1;
    for(int i = 0; i < 4; ++i)
    {
      _answers.append(words[i].word_lang1);
    }
  }
}

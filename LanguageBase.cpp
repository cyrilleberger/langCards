#include "LanguageBase.h"

#include "Database.h"
#include "Words.h"
#include "Word.h"

LanguageBase::LanguageBase(LangCardsDB::Database* _db, QObject *parent) :
  QObject(parent), m_db(_db)
{
  if((LangCardsDB::Words::byLang1(m_db, "sv") && (LangCardsDB::Words::byLang2(m_db, "en"))).count() == 0)
  {
    m_words = m_db->createWords("sv", "en");
  } else {
    m_words = (LangCardsDB::Words::byLang1(m_db, "sv") && (LangCardsDB::Words::byLang2(m_db, "en"))).exec(1).first();
  }
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

void LanguageBase::updateWords()
{
  QList<LangCardsDB::Word> words;
  words.append(*m_currentWord);

  words += (LangCardsDB::Word::byTag(m_words, m_currentWord->tag())
            and LangCardsDB::Word::byLang1word(m_words, m_currentWord->lang1word(), Sql::NEQ)).randomOrder().exec(3);

  if(words.size() < 4)
  {
    LangCardsDB::Word::SelectQuery sq = LangCardsDB::Word::all(m_words);
    foreach(const LangCardsDB::Word& w, words)
    {
      sq = sq && LangCardsDB::Word::byLang1word(m_words, w.lang1word(), Sql::NEQ)
              && LangCardsDB::Word::byLang2word(m_words, w.lang2word(), Sql::NEQ);
    }
    words += sq.randomOrder().exec(4 - words.size());
  }

  random_shuffle(words.begin(), words.end());

  m_answers.clear();

  qreal coef = 1.0 / (1.0 + std::max(1.0, m_currentWord->lang1success()) /std::max(1.0, m_currentWord->lang2success()));

  if(qrand() < coef * RAND_MAX)
  {
    m_wordToGuess   = m_currentWord->lang1word();
    m_correctAnswer = m_currentWord->lang2word();
    for(int i = 0; i < 4; ++i)
    {
      m_answers.append(words[i].lang2word());
    }
  } else {
    m_wordToGuess   = m_currentWord->lang2word();
    m_correctAnswer = m_currentWord->lang1word();
    for(int i = 0; i < 4; ++i)
    {
      m_answers.append(words[i].lang1word());
    }
  }

  emit(wordChanged());
}

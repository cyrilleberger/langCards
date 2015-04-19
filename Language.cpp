#include "Language.h"

#include <cmath>

#include <QDebug>

#include "Parser.h"

#include "Database.h"
#include "Words.h"
#include "Word.h"

double compute_probability(const LangCardsDB::Word& w)
{
  return 1.0 / std::max(1.0, w.success());
}

Language::Language(LangCardsDB::Database* _db, QObject *parent) :
  QObject(parent), m_db(_db)
{
  if((LangCardsDB::Words::byLang1(_db, "sv") && (LangCardsDB::Words::byLang2(_db, "en"))).count() == 0)
  {
    m_words = _db->createWords("sv", "en");
  } else {
    m_words = (LangCardsDB::Words::byLang1(_db, "sv") && (LangCardsDB::Words::byLang2(_db, "en"))).exec(1).first();
  }

  Parser parser;
  m_basic   = parser.words(":/data/sv-en/basic.txt", &m_words);
  m_common  = parser.words(":/data/sv-en/common.txt", &m_words);
  m_rare    = parser.words(":/data/sv-en/rare.txt", &m_words);

  m_currentWords = LangCardsDB::Word::all(m_words).exec();
  m_totalProbability    = 0.0;
  for(int i = 0; i < m_currentWords.size(); ++i)
  {
    m_totalProbability += compute_probability(m_currentWords[i]);
  }

  nextWord();
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

void Language::nextWord()
{
  if(LangCardsDB::Word::bySuccess(m_words, 5, Sql::LT).count() < 10)
  {
    // introduce 5 new words in the pool
    for(int i = 0; i < 5; ++i)
    {
      qreal total_probability   = m_basic.length() * 1.0  + m_common.length() * 0.1 + m_rare.length() * 0.01;
      qreal basic_probability   = m_basic.length() * 1.0 / total_probability;
      qreal common_probability  = m_common.length() * 0.1 / total_probability;

      qreal random_word = qrand() / qreal(RAND_MAX);

      QList<fWord>* list = 0;

      if(random_word < basic_probability)
      {
        list = &m_basic;
      }
      else if(random_word < basic_probability + common_probability)
      {
        list = &m_common;
      } else {
        list = &m_rare;
      }
      if(list->isEmpty())
      {
        qDebug() << "You know everything! You are the anti-jon Snow!";
        break;
      }
      int idx = qrand() % list->size();
      fWord new_word = list->at(idx);
      list->removeAt(idx);
      m_currentWords.append(m_words.createWord(new_word.word_lang1, new_word.word_lang2, LangCardsDB::Tag::byName(m_db, new_word.tag).exec(1).first() ));
      m_totalProbability += 1.0;
    }
  }

  qreal random_word = qrand() / qreal(RAND_MAX);
  qreal current_proba = 0.0;
  for(int i = 0; i < m_currentWords.size(); ++i)
  {
    LangCardsDB::Word* w = &m_currentWords[i];
    current_proba += compute_probability(*w) / m_totalProbability;

    if(random_word < current_proba)
    {
      m_currentWord = w;
      break;
    }
  }
  Q_ASSERT(m_currentWord);
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
  if(qrand() < RAND_MAX / 2)
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

void Language::userAnswer(const QString& _answer, qreal _answerTime)
{
  double old_proba = compute_probability(*m_currentWord);
  if(_answer == m_correctAnswer)
  {
    qreal a = 2.0;
    qreal b = 6.0;
    _answerTime = qMax(a, qMin(_answerTime * 1e-3, b));
    qreal coef = 1.0 - 0.8 * (_answerTime - a) / (b-a);
    m_currentWord->setSuccess(m_currentWord->success() + coef);
    m_currentWord->record();
  } else {
    m_currentWord->setSuccess(0);
    m_currentWord->record();
  }
  double new_proba = compute_probability(*m_currentWord);
  m_totalProbability += new_proba - old_proba;
}

QString Language::databaseDump() const
{
  QString dump;

  foreach(const LangCardsDB::Word& w, m_currentWords)
  {
    dump += w.lang1word() + " / " + w.lang2word() + " = " + QString::number(w.success()) + "\n";
  }

  return dump;
}

#include "Language.h"

#include <cmath>

#include <QDebug>

#include "Parser.h"

#include "Database.h"
#include "Words.h"
#include "Word.h"

double compute_probability(const LangCardsDB::Word& w)
{
  return 1.0 / std::pow(std::max(1.0, std::min(w.lang1success(), w.lang2success())), 3.0);
}

Language::Language(LangCardsDB::Database* _db, QObject *parent) :
  LanguageBase(_db, parent)
{
  reload();
}

void Language::reload()
{

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

void Language::nextWord()
{
  if( (LangCardsDB::Word::byLang1success(m_words, 5, Sql::LT)
       and LangCardsDB::Word::byLang2success(m_words, 5, Sql::LT)).count() < 10)
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

  updateWords();
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
    if(m_wordToGuess == m_currentWord->lang1word())
    {
      m_currentWord->setLang1success(m_currentWord->lang1success() + coef);
    } else {
      m_currentWord->setLang2success(m_currentWord->lang2success() + coef);
    }
  } else {
    if(m_wordToGuess == m_currentWord->lang1word())
    {
      m_currentWord->setLang1success(0);
    } else {
      m_currentWord->setLang2success(0);
    }
  }
  m_currentWord->record();
  double new_proba = compute_probability(*m_currentWord);
  m_totalProbability += new_proba - old_proba;
}

QString Language::databaseDump() const
{
  QString dump;

  foreach(const LangCardsDB::Word& w, m_currentWords)
  {
    dump += w.lang1word() + " / " + w.lang2word() + " = " + QString::number(w.lang1success()) + " / " + QString::number(w.lang2success()) + "\n";
  }

  return dump;
}

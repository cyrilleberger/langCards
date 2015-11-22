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
  m_total_available_probability = 0.0;

  Parser parser;
  m_available_words   = parser.words(":/data/sv-en.txt", &m_words, m_total_available_probability);

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
      qreal random_word = qrand() / qreal(RAND_MAX);

      qreal current_proba = 0.0;
      for(int i = 0; i < m_available_words.count(); ++i)
      {
        const fWord& w = m_available_words.at(i);
        current_proba += w.proba / m_total_available_probability;
        if(random_word < current_proba)
        {
          m_total_available_probability -= w.proba;
          m_currentWords.append(m_words.createWord(w.word_lang1, w.word_lang2, LangCardsDB::Tag::byName(m_db, w.tag).exec(1).first()));
          m_totalProbability += 1.0;
          m_available_words.removeAt(i);
          break;
        }
      }
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

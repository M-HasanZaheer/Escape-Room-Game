#ifndef PUZZLE_H
#define PUZZLE_H

#include <QString>

// Abstract base Puzzle class
class Puzzle
{
public:
    Puzzle(const QString& question, const QString& answer, const QString& hint)
        : question(question), answer(answer), hint(hint) {}

    virtual ~Puzzle() {}

    virtual bool checkAnswer(const QString& userAnswer) const
    {
        return userAnswer.compare(answer, Qt::CaseInsensitive) == 0;
    }

    QString getQuestion() const { return question; }
    QString getHint() const { return hint; }

protected:
    QString question;
    QString answer;
    QString hint;
};


// Example derived classes for puzzle types
class CrackTheCode : public Puzzle
{
public:
    CrackTheCode(const QString& q, const QString& a, const QString& h)
        : Puzzle(q, a, h) {}
};

class WordUnscramble : public Puzzle
{
public:
    WordUnscramble(const QString& q, const QString& a, const QString& h)
        : Puzzle(q, a, h) {}
};

class CipherCode : public Puzzle
{
public:
    CipherCode(const QString& q, const QString& a, const QString& h)
        : Puzzle(q, a, h) {}
};

class LogicPuzzle : public Puzzle
{
public:
    LogicPuzzle(const QString& q, const QString& a, const QString& h)
        : Puzzle(q, a, h) {}
};

#endif // PUZZLE_H

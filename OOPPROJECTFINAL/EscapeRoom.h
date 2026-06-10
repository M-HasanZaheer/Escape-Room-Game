#ifndef ESCAPEROOM_H
#define ESCAPEROOM_H

#include <QWidget>
#include <QStackedWidget>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>
#include "Puzzle.h"

class EscapeRoom : public QWidget
{
    Q_OBJECT

public:
    explicit EscapeRoom(QWidget *parent = nullptr);
    ~EscapeRoom();

private slots:
    void startFromWelcome();
    void selectPuzzleMode();
    void selectStatementMode();
    void startEasy();
    void startMedium();
    void startHard();
    void submitAnswer();
    void showHint();
    void exitGame();

private:
    enum Mode {
        NoneMode,
        PuzzleMode,
        StatementMode
    };

    enum Difficulty {
        NoneDifficulty,
        Easy,
        Medium,
        Hard
    };

    Mode currentMode;
    Difficulty currentDifficulty;
    int currentPuzzleIndex;
    int wrongAttempts;

    // UI widgets
    QStackedWidget* stack;

    // Welcome Screen
    QWidget* welcomeScreen;
    QLabel* welcomeLabel;
    QPushButton* startBtn;

    // Mode Selection Screen
    QWidget* modeSelectionScreen;
    QLabel* modeLabel;
    QPushButton* puzzleModeBtn;
    QPushButton* statementModeBtn;

    // Difficulty Selection Screen
    QWidget* difficultySelectionScreen;
    QLabel* difficultyLabel;
    QPushButton* easyBtn;
    QPushButton* mediumBtn;
    QPushButton* hardBtn;

    // Puzzle Screen
    QWidget* puzzleScreen;
    QLabel* puzzleQuestionLabel;
    QLineEdit* answerInput;
    QPushButton* submitAnswerBtn;
    QPushButton* hintBtn;
    QLabel* feedbackLabel;
    QLabel* attemptsLabel;

    // Ending Screen
    QWidget* endingScreen;
    QLabel* endingMessageLabel;
    QTextEdit* feedbackInput;
    QPushButton* exitBtn;

    // Puzzles list
    QList<Puzzle*> puzzles;

    // Setup functions for different screens
    void setupWelcomeScreen();
    void setupModeSelectionScreen();
    void setupDifficultySelectionScreen();
    void setupPuzzleScreen();
    void setupEndingScreen();

    void loadPuzzles(Mode mode, Difficulty level);
    void showCurrentPuzzle();
};

#endif // ESCAPEROOM_H

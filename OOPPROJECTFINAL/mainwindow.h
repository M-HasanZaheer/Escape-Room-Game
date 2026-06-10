#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <memory>

class PuzzleMode;  // Forward declaration

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onStartGame();
    void onModePuzzleSelected();
    void onModeStatementSelected();
    void onEasySelected();
    void onMediumSelected();
    void onHardSelected();
    void onBackToMode();

    void onSubmitAnswer();
    void onHintRequested();
    void onEndGame();
    void onPlayAgain();

private:
    void showPuzzle();

    QStackedWidget *stack;

    // Pages
    QWidget *welcomePage;
    QWidget *modePage;
    QWidget *difficultyPage;
    QWidget *puzzlePage;
    QWidget *endingPage;

    // Mode page buttons
    QPushButton *modePuzzleBtn;
    QPushButton *modeStatementBtn;

    // Difficulty page buttons
    QPushButton *easyBtn;
    QPushButton *mediumBtn;
    QPushButton *hardBtn;
    QPushButton *backToModeBtn;

    // Puzzle page widgets
    QLabel *puzzleLabel;
    QLineEdit *answerInput;
    QPushButton *submitBtn;
    QPushButton *hintBtn;
    QPushButton *endGameBtn;
    QLabel *attemptsLabel;

    // Ending page
    QPushButton *playAgainBtn;

    std::unique_ptr<PuzzleMode> currentLevel;
    int currentPuzzleIndex = 0;
};

#endif // MAINWINDOW_H

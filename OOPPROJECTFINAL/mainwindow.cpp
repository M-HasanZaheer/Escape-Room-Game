#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QFont>
#include <vector>
#include <memory>

// =============== Puzzle Base Classes ===============

class Puzzle {
protected:
    QString question;
    QString answer;
    QString hint;

public:
    Puzzle(QString q, QString a, QString h) : question(q), answer(a), hint(h) {}
    virtual ~Puzzle() {}
    virtual QString getQuestion() const { return question; }
    virtual bool checkAnswer(const QString &userAnswer) const {
        return userAnswer.trimmed().compare(answer, Qt::CaseInsensitive) == 0;
    }
    virtual QString getHint() const { return hint; }
};

// Derived puzzle types
class CrackTheCode : public Puzzle {
public:
    CrackTheCode(QString q, QString a, QString h) : Puzzle(q, a, h) {}
};

class CipherCode : public Puzzle {
public:
    CipherCode(QString q, QString a, QString h) : Puzzle(q, a, h) {}
};

class WordUnscramble : public Puzzle {
public:
    WordUnscramble(QString q, QString a, QString h) : Puzzle(q, a, h) {}
};

class LogicPuzzle : public Puzzle {
public:
    LogicPuzzle(QString q, QString a, QString h) : Puzzle(q, a, h) {}
};

// =============== PuzzleMode Base Class ===============

class PuzzleMode {
protected:
    std::vector<std::unique_ptr<Puzzle>> puzzles;
    int wrongAttempts = 0;

public:
    virtual ~PuzzleMode() = default;
    virtual void loadPuzzles() = 0;

    int getTotalPuzzles() const { return puzzles.size(); }
    Puzzle* getPuzzle(int index) {
        if(index < 0 || index >= (int)puzzles.size())
            return nullptr;
        return puzzles[index].get();
    }

    void incrementWrongAttempts() { wrongAttempts++; }
    int getWrongAttempts() const { return wrongAttempts; }
    void resetWrongAttempts() { wrongAttempts = 0; }
};

// =============== Difficulty Modes ===============

class EasyMode : public PuzzleMode {
public:
    void loadPuzzles() override {
        puzzles.clear();
        puzzles.push_back(std::make_unique<CrackTheCode>("Find the next number: 2, 4, 8, 16, ?", "32", "It's a power of 2."));
        puzzles.push_back(std::make_unique<WordUnscramble>("Unscramble: LPEAP", "Apple", "Common fruit."));
        puzzles.push_back(std::make_unique<CipherCode>("Decode Caesar cipher (shift 1): UIF", "The", "Shift each letter one back."));
        puzzles.push_back(std::make_unique<LogicPuzzle>("What must be broken before use?", "Egg", "Found in kitchen."));
        puzzles.push_back(std::make_unique<WordUnscramble>("Unscramble: ANANAB", "Banana", "Yellow fruit."));
        puzzles.push_back(std::make_unique<CrackTheCode>("What's next? 1, 3, 6, 10, 15, ?", "21", "Triangular numbers."));
        puzzles.push_back(std::make_unique<LogicPuzzle>("Once in a minute, twice in a moment, never in a thousand years?", "M", "Focus on letters."));
    }
};

class MediumMode : public PuzzleMode {
public:
    void loadPuzzles() override {
        puzzles.clear();
        puzzles.push_back(std::make_unique<CrackTheCode>("Complete pattern: 2, 6, 12, 20, ?", "30", "Look at difference."));
        puzzles.push_back(std::make_unique<CipherCode>("ROT13: CYNPR", "Place", "ROT13 means shift 13 letters."));
        puzzles.push_back(std::make_unique<WordUnscramble>("Unscramble: OCREPTUM", "Computer", "Your device."));
        puzzles.push_back(std::make_unique<LogicPuzzle>("I speak without mouth, hear without ears. What am I?", "Echo", "It's a sound."));
        puzzles.push_back(std::make_unique<CrackTheCode>("What next? 0,1,1,2,3,5,?", "8", "Fibonacci."));
        puzzles.push_back(std::make_unique<LogicPuzzle>("Tall when young, short when old?", "Candle", "Used during outages."));
    }
};

class HardMode : public PuzzleMode {
public:
    void loadPuzzles() override {
        puzzles.clear();
        puzzles.push_back(std::make_unique<CrackTheCode>("Next in pattern: 1,4,9,16,25,?", "36", "Squares."));
        puzzles.push_back(std::make_unique<CipherCode>("Binary to ASCII: 01001000 01001001", "Hi", "8-bit letters."));
        puzzles.push_back(std::make_unique<WordUnscramble>("Unscramble: NIAGTMLROPH", "Polymorphism", "Programming concept."));
        puzzles.push_back(std::make_unique<LogicPuzzle>("More you take, more you leave behind?", "Footsteps", "Think walking."));
        puzzles.push_back(std::make_unique<CrackTheCode>("Sum 'ESCAPE' with A=1, B=2...", "47", "Add letter values."));
        puzzles.push_back(std::make_unique<CipherCode>("Atbash cipher: ZOO", "All", "A->Z, B->Y..."));
        puzzles.push_back(std::make_unique<LogicPuzzle>("What disappears when you say its name?", "Silence", "Don't speak."));
    }
};

class StatementPuzzleMode : public PuzzleMode {
public:
    void loadPuzzles() override {
        puzzles.clear();
        puzzles.push_back(std::make_unique<LogicPuzzle>("I have cities but no houses, forests but no trees. What am I?", "Map", "Used to navigate."));
        puzzles.push_back(std::make_unique<CrackTheCode>("Broken clock shows 9:13. What 4-digit code?", "0913", "Correct twice a day."));
        puzzles.push_back(std::make_unique<LogicPuzzle>("ɾʰʇ sʰʇʇʐ ɔʇ ʰʇʰʰʇ", "The thorn is walking", "Reverse letters."));
        puzzles.push_back(std::make_unique<WordUnscramble>("ROYGBIV", "ROYGBIV", "Rainbow colors."));
        puzzles.push_back(std::make_unique<CrackTheCode>("First of sun, second of fire... What code?", "SFNS", "Initial letters."));
        puzzles.push_back(std::make_unique<CrackTheCode>("Magic square missing number: 8 1 6, 3 5 7, 4 9 ?", "2", "Rows add to 15."));
        puzzles.push_back(std::make_unique<LogicPuzzle>("You entered Echo Chamber. Room name is password. What is it?", "Echo", "Say aloud."));
    }
};

// =============== MainWindow Implementation ===============

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    stack = new QStackedWidget(this);
    setCentralWidget(stack);

    // ===== Welcome Page =====
    welcomePage = new QWidget;
    QVBoxLayout *welcomeLayout = new QVBoxLayout(welcomePage);

    QLabel *welcomeLabel = new QLabel(
        "===== Welcome to the Virtual Escape Room Challenge =====");
    QFont titleFont = welcomeLabel->font();
    titleFont.setPointSize(18);
    titleFont.setBold(true);
    welcomeLabel->setFont(titleFont);
    welcomeLabel->setAlignment(Qt::AlignCenter);

    QPushButton *startBtn = new QPushButton("Start Game");
    startBtn->setMinimumHeight(50);
    startBtn->setFont(QFont("Arial", 14));
    welcomeLayout->addWidget(welcomeLabel);
    welcomeLayout->addStretch();
    welcomeLayout->addWidget(startBtn);
    welcomeLayout->addStretch();

    connect(startBtn, &QPushButton::clicked, this, &MainWindow::onStartGame);

    // ===== Mode Selection Page =====
    modePage = new QWidget;
    QVBoxLayout *modeLayout = new QVBoxLayout(modePage);

    QLabel *modeLabel = new QLabel("Choose Mode:");
    modeLabel->setFont(QFont("Arial", 16));
    modeLabel->setAlignment(Qt::AlignCenter);

    modePuzzleBtn = new QPushButton("Puzzle-based Mode");
    modeStatementBtn = new QPushButton("Statement-based Mode");

    modePuzzleBtn->setMinimumHeight(60);
    modeStatementBtn->setMinimumHeight(60);
    modePuzzleBtn->setFont(QFont("Arial", 14));
    modeStatementBtn->setFont(QFont("Arial", 14));

    modeLayout->addWidget(modeLabel);
    modeLayout->addSpacing(20);
    modeLayout->addWidget(modePuzzleBtn);
    modeLayout->addWidget(modeStatementBtn);

    connect(modePuzzleBtn, &QPushButton::clicked, this, &MainWindow::onModePuzzleSelected);
    connect(modeStatementBtn, &QPushButton::clicked, this, &MainWindow::onModeStatementSelected);

    // ===== Difficulty Selection Page =====
    difficultyPage = new QWidget;
    QVBoxLayout *difficultyLayout = new QVBoxLayout(difficultyPage);

    QLabel *diffLabel = new QLabel("Choose Difficulty:");
    diffLabel->setFont(QFont("Arial", 16));
    diffLabel->setAlignment(Qt::AlignCenter);

    easyBtn = new QPushButton("Easy");
    mediumBtn = new QPushButton("Medium");
    hardBtn = new QPushButton("Hard");
    backToModeBtn = new QPushButton("Back");

    for(auto btn : {easyBtn, mediumBtn, hardBtn, backToModeBtn}) {
        btn->setMinimumHeight(50);
        btn->setFont(QFont("Arial", 14));
    }

    difficultyLayout->addWidget(diffLabel);
    difficultyLayout->addSpacing(15);
    difficultyLayout->addWidget(easyBtn);
    difficultyLayout->addWidget(mediumBtn);
    difficultyLayout->addWidget(hardBtn);
    difficultyLayout->addSpacing(15);
    difficultyLayout->addWidget(backToModeBtn);

    connect(easyBtn, &QPushButton::clicked, this, &MainWindow::onEasySelected);
    connect(mediumBtn, &QPushButton::clicked, this, &MainWindow::onMediumSelected);
    connect(hardBtn, &QPushButton::clicked, this, &MainWindow::onHardSelected);
    connect(backToModeBtn, &QPushButton::clicked, this, &MainWindow::onBackToMode);


// ===== Puzzle Page Setup =====
puzzlePage = new QWidget;
QVBoxLayout *puzzleLayout = new QVBoxLayout(puzzlePage);

puzzleQuestionLabel = new QLabel;
puzzleQuestionLabel->setWordWrap(true);
puzzleQuestionLabel->setFont(QFont("Arial", 16));
puzzleQuestionLabel->setAlignment(Qt::AlignCenter);

answerEdit = new QLineEdit;
answerEdit->setMinimumHeight(40);
answerEdit->setFont(QFont("Arial", 14));

QHBoxLayout *buttonLayout = new QHBoxLayout;
submitAnswerBtn = new QPushButton("Submit Answer");
submitAnswerBtn->setMinimumHeight(45);
submitAnswerBtn->setFont(QFont("Arial", 14));
hintBtn = new QPushButton("Hint");
hintBtn->setMinimumHeight(45);
hintBtn->setFont(QFont("Arial", 14));

buttonLayout->addWidget(submitAnswerBtn);
buttonLayout->addWidget(hintBtn);

puzzleLayout->addWidget(puzzleQuestionLabel);
puzzleLayout->addWidget(answerEdit);
puzzleLayout->addLayout(buttonLayout);

connect(submitAnswerBtn, &QPushButton::clicked, this, &MainWindow::onSubmitAnswer);
connect(hintBtn, &QPushButton::clicked, this, &MainWindow::onShowHint);

// ===== Ending Page Setup =====
endingPage = new QWidget;
QVBoxLayout *endingLayout = new QVBoxLayout(endingPage);

endingLabel = new QLabel;
endingLabel->setFont(QFont("Arial", 18));
endingLabel->setAlignment(Qt::AlignCenter);

QPushButton *restartBtn = new QPushButton("Restart Game");
restartBtn->setMinimumHeight(50);
restartBtn->setFont(QFont("Arial", 14));

endingLayout->addStretch();
endingLayout->addWidget(endingLabel);
endingLayout->addSpacing(20);
endingLayout->addWidget(restartBtn);
endingLayout->addStretch();

connect(restartBtn, &QPushButton::clicked, this, &MainWindow::onRestartGame);

// Add pages to stacked widget
stack->addWidget(puzzlePage);   // index 3
stack->addWidget(endingPage);   // index 4

// Initialize gameplay variables
currentPuzzleIndex = 0;
selectedMode = nullptr;
selectedDifficulty = "";

// Set window minimum size for better UI
setMinimumSize(600, 400);
}

// ===== Slots Implementation =====

void MainWindow::onStartGame() {
    stack->setCurrentWidget(modePage);
}

void MainWindow::onModePuzzleSelected() {
    selectedModeType = ModeType::PuzzleBased;
    stack->setCurrentWidget(difficultyPage);
}

void MainWindow::onModeStatementSelected() {
    selectedModeType = ModeType::StatementBased;
    stack->setCurrentWidget(difficultyPage);
}

void MainWindow::onBackToMode() {
    stack->setCurrentWidget(modePage);
}

void MainWindow::onEasySelected() {
    selectedDifficulty = "Easy";
    setupSelectedMode();
}

void MainWindow::onMediumSelected() {
    selectedDifficulty = "Medium";
    setupSelectedMode();
}

void MainWindow::onHardSelected() {
    selectedDifficulty = "Hard";
    setupSelectedMode();
}

void MainWindow::setupSelectedMode() {
    if(selectedMode) {
        delete selectedMode;
        selectedMode = nullptr;
    }

    if(selectedModeType == ModeType::PuzzleBased) {
        if(selectedDifficulty == "Easy")
            selectedMode = new EasyMode();
        else if(selectedDifficulty == "Medium")
            selectedMode = new MediumMode();
        else if(selectedDifficulty == "Hard")
            selectedMode = new HardMode();
    } else if(selectedModeType == ModeType::StatementBased) {
        // StatementPuzzleMode does not differentiate difficulty here, load all puzzles
        selectedMode = new StatementPuzzleMode();
    }

    if(selectedMode) {
        selectedMode->loadPuzzles();
        currentPuzzleIndex = 0;
        selectedMode->resetWrongAttempts();
        showCurrentPuzzle();
        stack->setCurrentWidget(puzzlePage);
    } else {
        QMessageBox::warning(this, "Error", "Failed to load puzzles.");
        stack->setCurrentWidget(difficultyPage);
    }
}

void MainWindow::showCurrentPuzzle() {
    if(!selectedMode) return;
    Puzzle *puzzle = selectedMode->getPuzzle(currentPuzzleIndex);
    if(puzzle) {
        puzzleQuestionLabel->setText(puzzle->getQuestion());
        answerEdit->clear();
        answerEdit->setFocus();
    }
}

void MainWindow::onSubmitAnswer() {
    if(!selectedMode) return;

    Puzzle *puzzle = selectedMode->getPuzzle(currentPuzzleIndex);
    if(!puzzle) return;

    QString userAnswer = answerEdit->text();
    if(puzzle->checkAnswer(userAnswer)) {
        currentPuzzleIndex++;
        if(currentPuzzleIndex >= selectedMode->getTotalPuzzles()) {
            showEndingScreen(true);
        } else {
            showCurrentPuzzle();
        }
    } else {
        selectedMode->incrementWrongAttempts();
        QMessageBox::warning(this, "Wrong Answer", "Incorrect answer. Try again.");
    }
}

void MainWindow::onShowHint() {
    if(!selectedMode) return;
    Puzzle *puzzle = selectedMode->getPuzzle(currentPuzzleIndex);
    if(puzzle) {
        QMessageBox::information(this, "Hint", puzzle->getHint());
    }
}

void MainWindow::showEndingScreen(bool success) {
    if(success) {
        endingLabel->setText("Congratulations! You have escaped the room!");
    } else {
        endingLabel->setText("Game Over. Better luck next time!");
    }
    stack->setCurrentWidget(endingPage);
}

void MainWindow::onRestartGame() {
    if(selectedMode) {
        delete selectedMode;
        selectedMode = nullptr;
    }
    selectedDifficulty.clear();
    currentPuzzleIndex = 0;
    stack->setCurrentWidget(welcomePage);
}

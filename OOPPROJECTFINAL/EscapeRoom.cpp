#include "EscapeRoom.h"
#include "Puzzle.h"
#include <QMessageBox>
#include <QApplication>
#include <QTimer>

EscapeRoom::EscapeRoom(QWidget *parent)
    : QWidget(parent), currentMode(NoneMode), currentDifficulty(NoneDifficulty), currentPuzzleIndex(0), wrongAttempts(0)
{
    stack = new QStackedWidget(this);

    setupWelcomeScreen();         // NEW: Setup Welcome screen first
    setupModeSelectionScreen();
    setupDifficultySelectionScreen();
    setupPuzzleScreen();
    setupEndingScreen();

    // Add widgets in order
    stack->addWidget(welcomeScreen);        // Show welcome screen first
    stack->addWidget(modeSelectionScreen);
    stack->addWidget(difficultySelectionScreen);
    stack->addWidget(puzzleScreen);
    stack->addWidget(endingScreen);

    stack->setCurrentWidget(welcomeScreen); // Start here

    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(stack);
    setLayout(mainLayout);

    setWindowTitle("Virtual Escape Room");
    resize(600, 400);
}

EscapeRoom::~EscapeRoom()
{
    qDeleteAll(puzzles);
}

void EscapeRoom::setupWelcomeScreen()
{
    welcomeScreen = new QWidget(this);

    welcomeLabel = new QLabel(
        "<h1>Welcome to the Virtual Escape Room!</h1>"
        "<p>Get ready to test your wits and solve puzzles.</p>"
        "<p>Click Start to begin.</p>",
        welcomeScreen);
    welcomeLabel->setAlignment(Qt::AlignCenter);
    welcomeLabel->setStyleSheet("font-size: 24px; font-weight: bold; margin-bottom: 40px;");

    startBtn = new QPushButton("Start", welcomeScreen);
    startBtn->setMinimumHeight(60);
    startBtn->setMinimumWidth(180);
    startBtn->setStyleSheet("font-size: 22px; padding: 15px 30px;");

    QVBoxLayout* layout = new QVBoxLayout(welcomeScreen);
    layout->setContentsMargins(50, 50, 50, 50);
    layout->addWidget(welcomeLabel);
    layout->addWidget(startBtn, 0, Qt::AlignCenter);

    connect(startBtn, &QPushButton::clicked, this, &EscapeRoom::startFromWelcome);
}

void EscapeRoom::startFromWelcome()
{
    stack->setCurrentWidget(modeSelectionScreen);
}

void EscapeRoom::setupModeSelectionScreen()
{
    modeSelectionScreen = new QWidget(this);

    modeLabel = new QLabel(
        "<h1>Select Mode</h1>"
        "<p>Please select Puzzle Mode or Statement Mode:</p>", modeSelectionScreen);
    modeLabel->setAlignment(Qt::AlignCenter);
    modeLabel->setStyleSheet("font-size: 22px; margin-bottom: 30px;");

    puzzleModeBtn = new QPushButton("Puzzle Mode", modeSelectionScreen);
    statementModeBtn = new QPushButton("Statement Mode", modeSelectionScreen);

    QList<QPushButton*> buttons = {puzzleModeBtn, statementModeBtn};
    for (QPushButton* btn : buttons) {
        btn->setMinimumHeight(60);
        btn->setMinimumWidth(150);
        btn->setStyleSheet("font-size: 20px; padding: 12px 24px;");
    }

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(40);
    btnLayout->addWidget(puzzleModeBtn);
    btnLayout->addWidget(statementModeBtn);

    QVBoxLayout* layout = new QVBoxLayout(modeSelectionScreen);
    layout->setContentsMargins(50, 50, 50, 50);
    layout->addWidget(modeLabel);
    layout->addLayout(btnLayout);

    connect(puzzleModeBtn, &QPushButton::clicked, this, &EscapeRoom::selectPuzzleMode);
    connect(statementModeBtn, &QPushButton::clicked, this, &EscapeRoom::selectStatementMode);
}

void EscapeRoom::setupDifficultySelectionScreen()
{
    difficultySelectionScreen = new QWidget(this);

    difficultyLabel = new QLabel(
        "<h1>Select Difficulty</h1>"
        "<p>Choose your difficulty level to start:</p>", difficultySelectionScreen);
    difficultyLabel->setAlignment(Qt::AlignCenter);
    difficultyLabel->setStyleSheet("font-size: 22px; margin-bottom: 30px;");

    easyBtn = new QPushButton("Easy", difficultySelectionScreen);
    mediumBtn = new QPushButton("Medium", difficultySelectionScreen);
    hardBtn = new QPushButton("Hard", difficultySelectionScreen);

    QList<QPushButton*> buttons = {easyBtn, mediumBtn, hardBtn};
    for (QPushButton* btn : buttons) {
        btn->setMinimumHeight(60);
        btn->setMinimumWidth(120);
        btn->setStyleSheet("font-size: 20px; padding: 12px 24px;");
    }

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->setSpacing(40);
    btnLayout->addWidget(easyBtn);
    btnLayout->addWidget(mediumBtn);
    btnLayout->addWidget(hardBtn);

    QVBoxLayout* layout = new QVBoxLayout(difficultySelectionScreen);
    layout->setContentsMargins(50, 50, 50, 50);
    layout->addWidget(difficultyLabel);
    layout->addLayout(btnLayout);

    connect(easyBtn, &QPushButton::clicked, this, &EscapeRoom::startEasy);
    connect(mediumBtn, &QPushButton::clicked, this, &EscapeRoom::startMedium);
    connect(hardBtn, &QPushButton::clicked, this, &EscapeRoom::startHard);
}

void EscapeRoom::setupPuzzleScreen()
{
    puzzleScreen = new QWidget(this);

    puzzleQuestionLabel = new QLabel(puzzleScreen);
    puzzleQuestionLabel->setWordWrap(true);
    puzzleQuestionLabel->setAlignment(Qt::AlignCenter);
    puzzleQuestionLabel->setStyleSheet("font-size: 20px; font-weight: bold; margin-bottom: 20px;");

    answerInput = new QLineEdit(puzzleScreen);
    answerInput->setMinimumHeight(40);
    answerInput->setStyleSheet("font-size: 18px; padding: 8px;");

    submitAnswerBtn = new QPushButton("Submit Answer", puzzleScreen);
    submitAnswerBtn->setMinimumHeight(50);
    submitAnswerBtn->setStyleSheet("font-size: 18px; padding: 10px 20px;");

    hintBtn = new QPushButton("Show Hint", puzzleScreen);
    hintBtn->setMinimumHeight(50);
    hintBtn->setStyleSheet("font-size: 18px; padding: 10px 20px;");

    feedbackLabel = new QLabel("", puzzleScreen);
    feedbackLabel->setStyleSheet("font-size: 16px; margin-top: 15px;");

    attemptsLabel = new QLabel("Wrong Attempts: 0", puzzleScreen);
    attemptsLabel->setStyleSheet("font-size: 16px; margin-top: 10px;");

    QHBoxLayout* btnLayout = new QHBoxLayout();
    btnLayout->addWidget(submitAnswerBtn);
    btnLayout->addWidget(hintBtn);
    btnLayout->setSpacing(20);

    QVBoxLayout* layout = new QVBoxLayout(puzzleScreen);
    layout->setContentsMargins(40, 40, 40, 40);
    layout->setSpacing(15);

    layout->addWidget(puzzleQuestionLabel);
    layout->addWidget(answerInput);
    layout->addLayout(btnLayout);
    layout->addWidget(feedbackLabel);
    layout->addWidget(attemptsLabel);

    connect(submitAnswerBtn, &QPushButton::clicked, this, &EscapeRoom::submitAnswer);
    connect(hintBtn, &QPushButton::clicked, this, &EscapeRoom::showHint);
}

void EscapeRoom::setupEndingScreen()
{
    endingScreen = new QWidget(this);

    endingMessageLabel = new QLabel(endingScreen);
    endingMessageLabel->setWordWrap(true);
    endingMessageLabel->setAlignment(Qt::AlignCenter);
    endingMessageLabel->setStyleSheet("font-size: 20px; font-weight: bold;");

    feedbackInput = new QTextEdit(endingScreen);
    feedbackInput->setPlaceholderText("Enter your feedback or suggestions here...");

    exitBtn = new QPushButton("Exit", endingScreen);

    QVBoxLayout* layout = new QVBoxLayout(endingScreen);
    layout->addWidget(endingMessageLabel);
    layout->addWidget(feedbackInput);
    layout->addWidget(exitBtn);

    connect(exitBtn, &QPushButton::clicked, this, &EscapeRoom::exitGame);
}

void EscapeRoom::loadPuzzles(Mode mode, Difficulty level)
{
    qDeleteAll(puzzles);
    puzzles.clear();
    wrongAttempts = 0;
    currentPuzzleIndex = 0;

    if (mode == PuzzleMode) {
        switch(level) {
        case Easy:
            puzzles.append(new CrackTheCode("Find the next number in the sequence: 2, 4, 8, 16, ?", "32", "It's a power of 2."));
            puzzles.append(new WordUnscramble("Unscramble: LPEAP", "Apple", "Think of a common fruit."));
            puzzles.append(new CipherCode("Decode this Caesar cipher with shift 1: dpef", "Code", "Shift each letter one back in the alphabet."));
            puzzles.append(new LogicPuzzle("What has to be broken before you can use it?", "Egg", "It's found in your kitchen."));
            puzzles.append(new WordUnscramble("Unscramble: ANANAB", "Banana", "Another yellow fruit."));
            puzzles.append(new CrackTheCode("What's next? 1, 3, 6, 10, 15, ?", "21", "Think of triangular numbers."));
            puzzles.append(new LogicPuzzle("What comes once in a minute, twice in a moment, but never in a thousand years?", "M", "Focus on the letters in the words."));
            break;

        case Medium:
            puzzles.append(new CrackTheCode("Complete the pattern: 2, 6, 12, 20, ?", "30", "Look at the difference pattern."));
            puzzles.append(new CipherCode("ROT13: xloblneq", "Keyboard", "ROT13 means shift 13 characters."));
            puzzles.append(new WordUnscramble("Unscramble: OCREPTUM", "Computer", "A device you're using right now."));
            puzzles.append(new LogicPuzzle("I speak without a mouth and hear without ears. I have no body, but I come alive with wind. What am I?", "Echo", "It's a sound."));
            puzzles.append(new WordUnscramble("Unscramble: KROTWREN", "Network", "Trick puzzle. No real word—skip."));
            puzzles.append(new CrackTheCode("What comes next? 0, 1, 1, 2, 3, 5, ?", "8", "Fibonacci sequence."));
            puzzles.append(new LogicPuzzle("I'm tall when I'm young, and I'm short when I'm old. What am I?", "Candle", "Used during power outages."));
            break;
        case Hard:
            puzzles.append(new CrackTheCode("Next in pattern: 1, 4, 9, 16, 25, ?", "36", "Think squares."));
            puzzles.append(new CipherCode("Binary to ASCII: 01000011 01010000 01010101", "Cpu", "Each 8-bit represents a letter."));
            puzzles.append(new WordUnscramble("Unscramble: NIOGPMLROHPY", "Polymorphing", "A coding concept."));
            puzzles.append(new LogicPuzzle("The more you take, the more you leave behind. What are they?", "Footsteps", "Think of walking."));
            puzzles.append(new CrackTheCode("If A=1, B=2... What is the sum of 'ESCAPE'?", "49", "Add letter values."));
            puzzles.append(new CipherCode("Atbash cipher for: WZGZ", "Data", "A becomes Z, B becomes Y..."));
            puzzles.append(new LogicPuzzle("What disappears the moment you say its name?", "Silence", "Think carefully before speaking."));
            break;
        default:
            break;
        }
    }
    else if (mode == StatementMode) {
        switch(level) {
        case Easy:
            puzzles.append(new LogicPuzzle("I have cities but no houses, forests but no trees, and rivers but no water. What am I?", "Map", "Think of something you use to navigate."));
            puzzles.append(new CrackTheCode("A broken clock shows 9:13. What 4-digit code does it represent?", "0913", "The time is correct twice a day."));
            puzzles.append(new LogicPuzzle("ɾʰʇ sʰʇʐ ɔʇ ʰʇʰʰʇ", "The thorn is walking", "Use a mirror or reverse the letters."));
            puzzles.append(new WordUnscramble("ROYGBIV", "ROYGBIV", "Colors of the rainbow in reverse."));
            puzzles.append(new CrackTheCode("First of sun, second of fire, third of night, fourth of sky. What code do the initials make?", "SFNS", "Take the first letter of each word."));
            puzzles.append(new CrackTheCode("Magic square: Fill the missing number. 8 1 6, 3 5 7, 4 9 ?", "2", "All rows and columns add up to 15."));
            puzzles.append(new LogicPuzzle("You entered the Echo Chamber. The room's name is the password. What is it?", "Echo", "Say the room's name aloud."));
            break;
        case Medium:
            puzzles.append(new LogicPuzzle("I have cities but no houses, forests but no trees, and rivers but no water. What am I?", "Map", "Think of something you use to navigate."));
            puzzles.append(new CrackTheCode("A broken clock shows 9:13. What 4-digit code does it represent?", "0913", "The time is correct twice a day."));
            puzzles.append(new LogicPuzzle("ɾʰʇ sʰʇʐ ɔʇ ʰʇʰʰʇ", "The thorn is walking", "Use a mirror or reverse the letters."));
            puzzles.append(new WordUnscramble("ROYGBIV", "ROYGBIV", "Colors of the rainbow in reverse."));
            puzzles.append(new CrackTheCode("First of sun, second of fire, third of night, fourth of sky. What code do the initials make?", "SFNS", "Take the first letter of each word."));
            puzzles.append(new CrackTheCode("Magic square: Fill the missing number. 8 1 6, 3 5 7, 4 9 ?", "2", "All rows and columns add up to 15."));
            puzzles.append(new LogicPuzzle("You entered the Echo Chamber. The room's name is the password. What is it?", "Echo", "Say the room's name aloud."));
            break;
        case Hard:
            puzzles.append(new LogicPuzzle("I have cities but no houses, forests but no trees, and rivers but no water. What am I?", "Map", "Think of something you use to navigate."));
            puzzles.append(new CrackTheCode("A broken clock shows 9:13. What 4-digit code does it represent?", "0913", "The time is correct twice a day."));
            puzzles.append(new LogicPuzzle("ɾʰʇ sʰʇʐ ɔʇ ʰʇʰʰʇ", "The thorn is walking", "Use a mirror or reverse the letters."));
            puzzles.append(new WordUnscramble("ROYGBIV", "ROYGBIV", "Colors of the rainbow in reverse."));
            puzzles.append(new CrackTheCode("First of sun, second of fire, third of night, fourth of sky. What code do the initials make?", "SFNS", "Take the first letter of each word."));
            puzzles.append(new CrackTheCode("Magic square: Fill the missing number. 8 1 6, 3 5 7, 4 9 ?", "2", "All rows and columns add up to 15."));
            puzzles.append(new LogicPuzzle("You entered the Echo Chamber. The room's name is the password. What is it?", "Echo", "Say the room's name aloud."));
            break;
        default:
            break;
        }
    }
}

void EscapeRoom::showCurrentPuzzle()
{
    if (currentPuzzleIndex >= puzzles.size()) {
        // Completed all puzzles/statements successfully
        endingMessageLabel->setText(
            "<h2>Congratulations! You escaped successfully!</h2>"
            "<p>You completed all challenges.</p>");
        stack->setCurrentWidget(endingScreen);
        return;
    }
    Puzzle* current = puzzles[currentPuzzleIndex];
    puzzleQuestionLabel->setText(current->getQuestion());
    answerInput->clear();
    feedbackLabel->clear();
    attemptsLabel->setText("Wrong Attempts: 0");
    wrongAttempts = 0;
    stack->setCurrentWidget(puzzleScreen);
}

void EscapeRoom::selectPuzzleMode()
{
    currentMode = PuzzleMode;
    stack->setCurrentWidget(difficultySelectionScreen);
}

void EscapeRoom::selectStatementMode()
{
    currentMode = StatementMode;
    stack->setCurrentWidget(difficultySelectionScreen);
}

void EscapeRoom::startEasy()
{
    currentDifficulty = Easy;
    loadPuzzles(currentMode, currentDifficulty);
    showCurrentPuzzle();
}

void EscapeRoom::startMedium()
{
    currentDifficulty = Medium;
    loadPuzzles(currentMode, currentDifficulty);
    showCurrentPuzzle();
}

void EscapeRoom::startHard()
{
    currentDifficulty = Hard;
    loadPuzzles(currentMode, currentDifficulty);
    showCurrentPuzzle();
}

void EscapeRoom::submitAnswer()
{
    if (currentPuzzleIndex >= puzzles.size())
        return;

    QString userAnswer = answerInput->text().trimmed();
    Puzzle* current = puzzles[currentPuzzleIndex];

    if (userAnswer.isEmpty()) {
        feedbackLabel->setText("Please enter an answer before submitting.");
        return;
    }

    if (current->checkAnswer(userAnswer)) {
        feedbackLabel->setStyleSheet("color: green; font-weight: bold;");
        feedbackLabel->setText("Correct! Moving to next challenge...");
        currentPuzzleIndex++;
        QTimer::singleShot(1500, this, &EscapeRoom::showCurrentPuzzle);
    } else {
        wrongAttempts++;
        feedbackLabel->setStyleSheet("color: red; font-weight: bold;");
        feedbackLabel->setText("Wrong answer. Try again.");
        attemptsLabel->setText(QString("Wrong Attempts: %1").arg(wrongAttempts));
        if (wrongAttempts >= 3) {
            // Automatically show hint after 3 wrong attempts
            showHint();
        }
    }
}

void EscapeRoom::showHint()
{
    if (currentPuzzleIndex >= puzzles.size())
        return;

    Puzzle* current = puzzles[currentPuzzleIndex];
    QString hint = current->getHint();
    QMessageBox::information(this, "Hint", hint);
}

void EscapeRoom::exitGame()
{
    QString feedback = feedbackInput->toPlainText().trimmed();
    if (!feedback.isEmpty()) {
        // Save feedback or process it as needed
        // For demo, just print it to console
        qDebug() << "User feedback:" << feedback;
    }
    QApplication::quit();
}

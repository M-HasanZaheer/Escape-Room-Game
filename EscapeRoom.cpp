#include <iostream>
#include <string>

using namespace std;

// Abstract base class
class Puzzle {
protected:
    string question;
    string answer;
    string hint;

public:
    Puzzle(string q, string a, string h) {
        question = q;
        answer = a;
        hint = h;
    }

    virtual void display() = 0;
    virtual bool checkAnswer(string userAnswer) {
        return userAnswer == answer;
    }
    virtual string getHint() {
        return hint;
    }
    virtual ~Puzzle() {}
};

// Derived classes (Inheritance + Polymorphism)
class CrackTheCode : public Puzzle {
public:
    CrackTheCode(string q, string a, string h) : Puzzle("", "", "") {
        question = q;
        answer = a;
        hint = h;
    }

    void display() override {
        cout << "Crack the Code: " << question << endl;
    }
};

class CipherCode : public Puzzle {
public:
    CipherCode(string q, string a, string h) : Puzzle("", "", "") {
        question = q;
        answer = a;
        hint = h;
    }

    void display() override {
        cout << "Cipher Puzzle: " << question << endl;
    }
};

class WordUnscramble : public Puzzle {
public:
    WordUnscramble(string q, string a, string h) : Puzzle("", "", "") {
        question = q;
        answer = a;
        hint = h;
    }

    void display() override {
        cout << "Unscramble the Word: " << question << endl;
    }
};

class LogicPuzzle : public Puzzle {
public:
    LogicPuzzle(string q, string a, string h) : Puzzle("", "", "") {
        question = q;
        answer = a;
        hint = h;
    }

    void display() override {
        cout << "Logic Puzzle: " << question << endl;
    }
};

// Difficulty base class
class DifficultyLevel {
protected:
    Puzzle* puzzles[7];
    int wrongAttempts;

public:
    DifficultyLevel() : wrongAttempts(0) {
        for (int i = 0; i < 7; ++i)
            puzzles[i] = nullptr;
    }

    virtual void loadPuzzles() = 0;

    virtual void play() {
        for (int i = 0; i < 7; i++) {
            puzzles[i]->display();
            string answer;
            cout << "Your Answer: ";
            getline(cin, answer);

            if (answer == "hint") {
                cout << "Hint: " << puzzles[i]->getHint() << endl;
                i--;
                continue;
            }

            if (puzzles[i]->checkAnswer(answer)) {
                cout << "Correct!\n\n";
            }
            else {
                wrongAttempts++;
                cout << "Wrong!\n";
                if (wrongAttempts >= 3) {
                    cout << "You triggered a trap and failed the escape room!\n";
                    return;
                }
                cout << "Type 'hint' if you want a clue.\n";
                i--;
            }
        }

        cout << R"(
||    ||  ||||||  ||    ||     ||     || || |||    || 
 ||  ||  ||    || ||    ||     ||     || || ||||   || 
  ||||   ||    || ||    ||     ||  |  || || || ||  || 
   ||    ||    || ||    ||     || ||| || || ||  || || 
   ||     ||||||   ||||||      |||   ||| || ||   |||| )";

        string feedback, experience;
        cout << "\n\nWe value your feedback!\n";
        cout << "How was your experience? ";
        getline(cin, experience);
        cout << "Any feedback or suggestions? ";
        getline(cin, feedback);
        cout << "\nThanks for your input!\n";
    }

    virtual ~DifficultyLevel() {
        for (int i = 0; i < 7; ++i)
            delete puzzles[i];
    }
};

// Easy Mode
class EasyMode : public DifficultyLevel {
public:
    void loadPuzzles() override {
        puzzles[0] = new CrackTheCode("Find the next number in the sequence: 2, 4, 8, 16, ?", "32", "It's a power of 2.");
        puzzles[1] = new WordUnscramble("Unscramble: LPEAP", "Apple", "Think of a common fruit.");
        puzzles[2] = new CipherCode("Decode this Caesar cipher with shift 1: dpef", "Code", "Shift each letter one back in the alphabet.");
        puzzles[3] = new LogicPuzzle("What has to be broken before you can use it?", "Egg", "It's found in your kitchen.");
        puzzles[4] = new WordUnscramble("Unscramble: ANANAB", "Banana", "Another yellow fruit.");
        puzzles[5] = new CrackTheCode("What's next? 1, 3, 6, 10, 15, ?", "21", "Think of triangular numbers.");
        puzzles[6] = new LogicPuzzle("What comes once in a minute, twice in a moment, but never in a thousand years?", "M", "Focus on the letters in the words.");
    }
};

// Medium Mode
class MediumMode : public DifficultyLevel {
public:
    void loadPuzzles() override {
        puzzles[0] = new CrackTheCode("Complete the pattern: 2, 6, 12, 20, ?", "30", "Look at the difference pattern.");
        puzzles[1] = new CipherCode("ROT13: xloblneq", "Keyboard", "ROT13 means shift 13 characters.");
        puzzles[2] = new WordUnscramble("Unscramble: OCREPTUM", "Computer", "A device you're using right now.");
        puzzles[3] = new LogicPuzzle("I speak without a mouth and hear without ears. I have no body, but I come alive with wind. What am I?", "Echo", "It's a sound.");
        puzzles[4] = new WordUnscramble("Unscramble: KROTWREN", "Network", "Trick puzzle. No real word—skip.");
        puzzles[5] = new CrackTheCode("What comes next? 0, 1, 1, 2, 3, 5, ?", "8", "Fibonacci sequence.");
        puzzles[6] = new LogicPuzzle("I'm tall when I'm young, and I'm short when I'm old. What am I?", "Candle", "Used during power outages.");
    }
};

// Hard Mode
class HardMode : public DifficultyLevel {
public:
    void loadPuzzles() override {
        puzzles[0] = new CrackTheCode("Next in pattern: 1, 4, 9, 16, 25, ?", "36", "Think squares.");
        puzzles[1] = new CipherCode("Binary to ASCII: 01000011 01010000 01010101", "Cpu", "Each 8-bit represents a letter.");
        puzzles[2] = new WordUnscramble("Unscramble: NIOGPMLROHPY", "Polymorphing", "A coding concept.");
        puzzles[3] = new LogicPuzzle("The more you take, the more you leave behind. What are they?", "Footsteps", "Think of walking.");
        puzzles[4] = new CrackTheCode("If A=1, B=2... What is the sum of 'ESCAPE'?", "49", "Add letter values.");
        puzzles[5] = new CipherCode("Atbash cipher for: WZGZ", "Data", "A becomes Z, B becomes Y...");
        puzzles[6] = new LogicPuzzle("What disappears the moment you say its name?", "Silence", "Think carefully before speaking.");
    }
};

// Statement Puzzle Mode
class StatementPuzzleMode : public DifficultyLevel {
public:
    void loadPuzzles() override {
        puzzles[0] = new LogicPuzzle("I have cities but no houses, forests but no trees, and rivers but no water. What am I?", "Map", "Think of something you use to navigate.");
        puzzles[1] = new CrackTheCode("A broken clock shows 9:13. What 4-digit code does it represent?", "0913", "The time is correct twice a day.");
        puzzles[2] = new LogicPuzzle("EHCAC YROMEM", "Cache Memory", "Use a mirror or reverse the letters.");
        puzzles[3] = new WordUnscramble("ROYGBIV", "Vibgyor", "Colors of the rainbow in reverse.");
        puzzles[4] = new CrackTheCode("First of sun, second of fire, third of night, fourth of water. What code do the initials make?", "SIGE", "Take the first letter of each word.");
        puzzles[5] = new CrackTheCode("Magic square: Fill the missing number. 8 1 6, 3 5 7, 4 9 ?", "2", "All rows and columns add up to 15.");
        puzzles[6] = new LogicPuzzle("You entered the Echo Chamber. The room's name is the password. What is it?", "Echo", "Say the room's name aloud.");
    }
};

void showWelcomeMessage() {
    cout << R"(
   ('-.    .-')                ('-.      _ (`-.    ('-.         _  .-')                            _   .-')    
 (  OO)  ( OO ).             ( OO ).-. ( (OO  ) _(  OO)       ( \( -O )                          ( '.( OO )  
(,------.()---\)   .-----.  / . --. /_.`     \(,------.       ,------.  .-'),-----.  .-'),-----. ,--.   ,--.)
 |  .---'/    _ |   '  .--./  | \-.  \(...--'' |  .---'       |   /`. '( OO'  .-.  '( OO'  .-.  '|   `.'   | 
 |  |    \  :` `.   |  |('-..-'-'  |  ||  /  | | |  |           |  /  | |/   |  | |  |/   |  | |  ||         | 
(|  '--.  '..`''.) /) |OO  )\| |.'  ||  |.' |(|  '--.        |  |.' |\) |  |\|  |\) |  |\|  ||  |'.'|  | 
 |  .--' .-.)   \ ||  |`-'|  |  .-.  ||  ..' |  .--'        |  .  '.'  \ |  | |  |  \ |  | |  ||  |   |  | 
 |  `---.\       /(_'  '--'\  |  | |  ||  |      |  `---.       |  |\  \    `'  '-'  '   `'  '-'  '|  |   |  | 
 ------' `-----'    `-----'  `--' `--'--'      `------'       `--' '--'     `-----'      `-----' `--'   `--'                                                                                                                                              
)";

    cout << "===== Welcome to the Virtual Escape Room Challenge =====\n";
}

void showEndingMessage() {
    cout << R"END(
**************************************************
*       Thank you for playing ESCAPE ROOM!       *
*        May your mind stay ever sharp!          *
**************************************************
)END" << endl;
}

int main() {
    showWelcomeMessage();

    cout << "Choose Mode:\n1 = Puzzle-based (Crack, Cipher, Unscramble, Logic)\n2 = Statement-based Puzzles\nChoice: ";
    int mode;
    cin >> mode;
    cin.ignore();

    DifficultyLevel* level = nullptr;

    if (mode == 1) {
        cout << "Choose Difficulty (1 = Easy, 2 = Medium, 3 = Hard): ";
        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            level = new EasyMode();
            break;
        case 2:
            level = new MediumMode();
            break;
        case 3:
            level = new HardMode();
            break;
        default:
            cout << "Invalid choice. Exiting.\n";
            return 0;
        }
    }
    else if (mode == 2) {
        level = new StatementPuzzleMode();
    }
    else {
        cout << "Invalid mode selected.\n";
        return 0;
    }

    level->loadPuzzles();
    level->play();

    delete level;
    showEndingMessage();

    return 0;
}

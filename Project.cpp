#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

class Player {
    friend class Word;
protected:
    string name;
    int lives;
    int attempts;
    char* symbols;
    bool isDeadP;
    int indexOfArray;

public:
    Player() : lives{ 1 }, attempts{ 0 }, symbols{ nullptr }, name{ "NULL" }, isDeadP{ false }, indexOfArray{ 0 } {
        symbols = new char[100];
    }

    ~Player() {
        delete[] symbols;
    }

    Player(string name) : Player{} {
        this->name = name;
    }

    Player(const Player& other) {
        this->name = other.name;
        this->lives = other.lives;
        this->attempts = other.attempts;
        this->indexOfArray = other.indexOfArray;
        this->isDeadP = other.isDeadP;

        this->symbols = new char[100];

        for (int i = 0; i < other.indexOfArray; ++i) {
            this->symbols[i] = other.symbols[i];
        }
    }

    void setName(string name) {
        this->name = name;
    }

    string getName() {
        return name;
    }

    bool isDead() {
        if (isDeadP == true)
            return true;
        return false;
    }

    bool ArrayOfLetters(char letter) {
        for (int i = 0; i < indexOfArray; ++i) {
            if (symbols[i] == letter) {
                return false;
            }
        }

        symbols[indexOfArray] = letter;
        indexOfArray++;
        return true;
    }

    void printInfo() const {
        cout << "Name - " << name << "\nLives - " << lives << "\nAttempts - " << attempts << endl;
        cout << "Player letters: " << endl;

        for (int i = 0; i < indexOfArray; ++i) {
            cout << symbols[i] << ' ';
        }
        cout << endl;
    }
};

class Word {
protected:
    string word;
    string currentWord;
    int length;
    vector<char> letterEnteredByPlayers;
public:
    Word() {
        length = 0;
    }

    Word(string word) {
        this->word = word;
        this->currentWord = word;
        length = word.length();
    }

    void printWord() {
        for (int i = 0; i < length; ++i) {
            if (i == 0 || i == length - 1)
                currentWord[i] = word[i];
            else
                currentWord[i] = '_';
        }
        cout << currentWord << endl << endl;
    }

    string getWord() {
        return word;
    }


    void printCurrentWord() {
        cout << currentWord << endl;
    }

    bool enterSymbol(char symbol, Player& other) {
        bool checkSymbol = false;
        for (int i = 0; i < length; ++i) {
            if (i > 0 && i < length - 1) {
                if (word[i] == symbol) {
                    currentWord[i] = symbol;
                    checkSymbol = true;
                }
            }
        }
        if (checkSymbol) {
            other.attempts++;
            return true;
        }
        if (checkSymbol == false) {
            if (other.lives == 0) {
                other.isDeadP = true;
            }
            else {
                other.attempts++;
                other.lives--;
            }
            return false;
        }
    }

    bool isCorrectWord() {
        if (currentWord == word)
            return true;
        else
            return false;
    }

    bool lettersPush(char letter) {
        if (checkLettersWord(letter)) {
            return false;
        }

        letterEnteredByPlayers.push_back(letter);
        return true;
    }

    bool checkLettersWord(char letter) {
        for (int i = 0; i < letterEnteredByPlayers.size(); ++i) {
            if (letterEnteredByPlayers[i] == letter) return true;
        }
        return false;
    }
};

void DeletePlayer(vector<Player>& players, int& i) {
    cout << "Player " << players[i].getName() << " is loser!!!" << endl;
    players.erase(players.begin() + i);

}

void setName(vector<Player>& players, int count) {
    string name = "";
    for (int i = 0; i != count; ++i) {

        cout << "Enter name: ";
        cin >> name;

        players.push_back(Player(name));
    }
}

void ShowInfo(vector<Player>& players) {
    cout << "==========Surviving players==========" << endl;
    for (int i = 0; i < players.size(); ++i) {
        players[i].printInfo();
    }
    cout << "=====================================" << endl;
    cout << endl << endl;
}
void ShowInfoDead(vector<Player>& players) {
    cout << "=============Dead players============" << endl;
    for (int i = 0; i < players.size(); ++i) {
        players[i].printInfo();
    }
    cout << "=====================================" << endl;
    cout << endl << endl;
}

void ShowWinner(string winner) {
    cout << "========== Winner ==========" << endl;
    cout << "           " << winner << endl;
    cout << "============================" << endl;
}

int main() {
    srand(time(NULL));

    vector<Player> players;
    vector<Player> DeletedPlayers;
    string winner;
    string path = "wordFile.txt";
    string arrayOfWords[30];
    int randomIndex = 0;
    ifstream words;

    int count;
    int countOfDeleted = 0;

    words.open(path);

    try{
        if (!words.is_open()) {
            throw exception("The file could not be opened!");
        }

        else {
            int index = 0;

            while (!words.eof()) {
                words >> arrayOfWords[index];
                index++;
            }

            randomIndex = rand() % index;
        }
    }

    catch (exception e) {
        cout << e.what() << endl;
        return 0;
    }

    words.close();

    Word w(arrayOfWords[randomIndex]);

    cout << "Hom many players will play?" << endl;
    cin >> count;


    setName(players, count);

    system("cls");

    cout << "The word - ";
    w.printWord();

    bool endGame = false;
    while (!endGame) {
        for (int i = 0; i != players.size(); ++i) {
            char letter;
            cout << players[i].getName() << " print letter: ";
            cin >> letter;

            if (w.enterSymbol(letter, players[i])) {

                if (w.checkLettersWord(letter)) {
                    cout << "This letter has already been guessed!" << endl;
                    break;
                }

                if (!players[i].ArrayOfLetters(letter)) {
                    cout << "You have already entered such a letter!" << endl;
                    break;
                }

                if (w.isCorrectWord()) {
                    cout << players[i].getName() << " is winner!" << endl;
                    cout << "The word is correct!" << endl;

                    winner = players[i].getName();

                    endGame = true;
                    break;
                }

                else {
                    w.lettersPush(letter);
                    cout << players[i].getName() << " found letter" << endl;
                    w.printCurrentWord();
                }
            }
            else {
                if (!players[i].ArrayOfLetters(letter)) {
                    cout << "You have already entered such a letter!" << endl;
                    break;
                }

                cout << "there is no such letter in this word" << endl << endl;

                if (players[i].isDead()) {
                    DeletedPlayers.push_back(players[i]);
                    countOfDeleted++;

                    DeletePlayer(players, i);
                    i--;

                    if (players.size() == 0) {
                        system("cls");
                        cout << "All players is DEAD!" << endl;
                        cout << "The word was - " << w.getWord() << endl;
                        break;
                    }
                }
            }
        }
    }
    system("cls");


    if (DeletedPlayers.size() == 0) {
        ShowInfo(players);
        ShowWinner(winner);
    }
    if (DeletedPlayers.size() > 0) {
        ShowInfo(players);
        ShowInfoDead(DeletedPlayers);
        ShowWinner(winner);
    }

    

    return 0;
}
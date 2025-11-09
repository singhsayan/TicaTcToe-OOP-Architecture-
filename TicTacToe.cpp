#include <iostream>
#include <vector>
#include <queue>
#include <deque>
using namespace std;

class IObserver {
public:
    virtual void update(string message) = 0;
    virtual ~IObserver() {}
};

class ConsoleNotifier : public IObserver {
public:
    void update(string message) override {
        cout << "[INFO] " << message << endl;
    }
};

class Symbol {
private:
    char value;
public:
    Symbol(char v) : value(v) {}
    char getValue() { return value; }
};

class Board {
private:
    vector<vector<Symbol*>> grid;
    int dimension;
    Symbol* emptySymbol;
public:
    Board(int dim) {
        dimension = dim;
        emptySymbol = new Symbol('-');
        grid = vector<vector<Symbol*>>(dimension, vector<Symbol*>(dimension, emptySymbol));
    }
    
    bool isCellEmpty(int row, int col) {
        if (row < 0 || row >= dimension || col < 0 || col >= dimension) return false;
        return grid[row][col] == emptySymbol;
    }

    bool placeSymbol(int row, int col, Symbol* symbol) {
        if (row < 0 || row >= dimension || col < 0 || col >= dimension) return false;
        if (!isCellEmpty(row, col)) return false;
        grid[row][col] = symbol;
        return true;
    }

    Symbol* getCell(int row, int col) {
        if (row < 0 || row >= dimension || col < 0 || col >= dimension) return emptySymbol;
        return grid[row][col];
    }

    int getSize() { return dimension; }
    Symbol* getEmptySymbol() { return emptySymbol; }

    void display() {
        cout << "\n   ";
        for (int i = 0; i < dimension; i++) cout << i << " ";
        cout << endl;
        
        for (int i = 0; i < dimension; i++) {
            cout << i << "  ";
            for (int j = 0; j < dimension; j++) {
                cout << grid[i][j]->getValue() << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
};

class Player {
private:
    int id;
    string name;
    Symbol* symbol;
    int score;
public:
    Player(int id, string name, Symbol* symbol) {
        this->id = id;
        this->name = name;
        this->symbol = symbol;
        score = 0;
    }
    string getName() { return name; }
    Symbol* getSymbol() { return symbol; }
    int getScore() { return score; }
    void incrementScore() { score++; }

    ~Player() {
        delete symbol;
    }
};

class Rules {
public:
    virtual bool isValidMove(Board* board, int row, int col) = 0;
    virtual bool hasWinner(Board* board, Symbol* symbol) = 0;
    virtual bool isDraw(Board* board) = 0;
    virtual ~Rules() {}
};

class StandardRules : public Rules {
public:
    bool isValidMove(Board* board, int row, int col) override {
        return board->isCellEmpty(row, col);
    }

    bool hasWinner(Board* board, Symbol* symbol) override {
        int n = board->getSize();

        for (int i = 0; i < n; i++) {
            bool win = true;
            for (int j = 0; j < n; j++) {
                if (board->getCell(i, j) != symbol) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }

        for (int j = 0; j < n; j++) {
            bool win = true;
            for (int i = 0; i < n; i++) {
                if (board->getCell(i, j) != symbol) {
                    win = false;
                    break;
                }
            }
            if (win) return true;
        }

        bool win = true;
        for (int i = 0; i < n; i++) {
            if (board->getCell(i, i) != symbol) {
                win = false;
                break;
            }
        }
        if (win) return true;

        win = true;
        for (int i = 0; i < n; i++) {
            if (board->getCell(i, n - 1 - i) != symbol) {
                win = false;
                break;
            }
        }
        return win;
    }

    bool isDraw(Board* board) override {
        int n = board->getSize();
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (board->getCell(i, j) == board->getEmptySymbol()) return false;
            }
        }
        return true;
    }
};

class Game {
private:
    Board* board;
    deque<Player*> players;
    Rules* rules;
    vector<IObserver*> observers;
    bool finished;
public:
    Game(int size) {
        board = new Board(size);
        rules = new StandardRules();
        finished = false;
    }

    void addPlayer(Player* player) {
        players.push_back(player);
    }

    void addObserver(IObserver* observer) {
        observers.push_back(observer);
    }

    void notify(string message) {
        for (auto obs : observers) obs->update(message);
    }

    void play() {
        if (players.size() < 2) {
            cout << "At least two players are required to start the game." << endl;
            return;
        }

        notify("Game started.");

        while (!finished) {
            board->display();

            Player* current = players.front();
            cout << current->getName() << " (" << current->getSymbol()->getValue() << ") - Enter row and column: ";
            
            int row, col;
            cin >> row >> col;

            if (rules->isValidMove(board, row, col)) {
                board->placeSymbol(row, col, current->getSymbol());
                notify(current->getName() + " played at (" + to_string(row) + "," + to_string(col) + ").");

                if (rules->hasWinner(board, current->getSymbol())) {
                    board->display();
                    cout << current->getName() << " wins the match!" << endl;
                    current->incrementScore();
                    notify(current->getName() + " has won the game.");
                    finished = true;
                } 
                else if (rules->isDraw(board)) {
                    board->display();
                    cout << "Match ended in a draw." << endl;
                    notify("The game ended in a draw.");
                    finished = true;
                } 
                else {
                    players.pop_front();
                    players.push_back(current);
                }
            } 
            else {
                cout << "Invalid move. Please try again." << endl;
            }
        }
    }

    ~Game() {
        delete board;
        delete rules;
    }
};

enum GameType {
    STANDARD
};

class GameFactory {
public:
    static Game* create(GameType type, int size) {
        if (type == STANDARD) return new Game(size);
        return nullptr;
    }
};

int main() {
    cout << "TIC TAC TOE" << endl;

    int size;
    cout << "Enter board size: ";
    cin >> size;

    Game* game = GameFactory::create(STANDARD, size);

    IObserver* notifier = new ConsoleNotifier();
    game->addObserver(notifier);

    Player* p1 = new Player(1, "Henry", new Symbol('X'));
    Player* p2 = new Player(2, "John", new Symbol('O'));

    game->addPlayer(p1);
    game->addPlayer(p2);

    game->play();

    delete game;
    delete p1;
    delete p2;
    delete notifier;

    return 0;
}

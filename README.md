#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <string>

using namespace std;

struct Difficulty {
    string name;
    int maxNumber;
    int attempts;
};

int loadHighScore() {
    ifstream file("highscore.txt");
    int score = 0;
    if (file) file >> score;
    return score;
}

void saveHighScore(int score) {
    ofstream file("highscore.txt");
    if (file) file << score;
}

int main() {
    random_device rd;
    mt19937 rng(rd());

    const Difficulty levels[] = {
        {"Easy",   50,  10},
        {"Medium", 100, 8},
        {"Hard",   500, 9}
    };

    int highScore = loadHighScore();
    char playAgain;

    cout << "=== NUMBER GUESSING CHALLENGE ===\n";
    cout << "Current high score: " << highScore << "\n";

    do {
        cout << "\nChoose difficulty:\n"
             << "1. Easy   (1-50, 10 attempts)\n"
             << "2. Medium (1-100, 8 attempts)\n"
             << "3. Hard   (1-500, 9 attempts)\n"
             << "Choice: ";

        int choice;
        while (!(cin >> choice) || choice < 1 || choice > 3) {
            cout << "Enter 1, 2, or 3: ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        Difficulty level = levels[choice - 1];
        uniform_int_distribution<int> distribution(1, level.maxNumber);
        int secret = distribution(rng);

        int attemptsLeft = level.attempts;
        int previousDistance = level.maxNumber + 1;
        bool won = false;

        cout << "\nI'm thinking of a number from 1 to "
             << level.maxNumber << ".\n";

        while (attemptsLeft > 0) {
            cout << "\nAttempts left: " << attemptsLeft
                 << "\nYour guess: ";

            int guess;
            while (!(cin >> guess) || guess < 1 || guess > level.maxNumber) {
                cout << "Enter a whole number from 1 to "
                     << level.maxNumber << ": ";
                cin.clear();
                cin.ignore(10000, '\n');
            }

            --attemptsLeft;

            if (guess == secret) {
                won = true;
                int score = attemptsLeft * 10 + level.maxNumber / 10;

                cout << "Correct! The number was " << secret << ".\n"
                     << "Your score: " << score << "\n";

                if (score > highScore) {
                    highScore = score;
                    saveHighScore(highScore);
                    cout << "New high score!\n";
                }
                break;
            }

            int distance = abs(secret - guess);
            cout << (guess < secret ? "Too low. " : "Too high. ");

            if (distance <= 5)
                cout << "You're very hot!";
            else if (distance <= 15)
                cout << "You're warm.";
            else
                cout << "You're cold.";

            if (distance < previousDistance)
                cout << " Getting warmer!";
            else if (distance > previousDistance)
                cout << " Getting colder!";

            cout << '\n';
            previousDistance = distance;
        }

        if (!won)
            cout << "\nOut of attempts! The number was " << secret << ".\n";

        cout << "\nPlay again? (y/n): ";
        cin >> playAgain;

    } while (playAgain == 'y' || playAgain == 'Y');

    cout << "\nThanks for playing!\n";
    return 0;
}

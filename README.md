#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int readInt(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) return value;

        std::cout << "Please enter a whole number.\n";
        std::cin.clear();
        std::cin.ignore(10000, '\n');
    }
}

int main() {
    std::random_device rd;
    std::mt19937 rng(rd());

    std::cout << "=== Number Guessing Challenge ===\n";

    do {
        int low = 1, high = 100, attempts = 8;
        std::cout << "\nDifficulty: 1) Easy  2) Medium  3) Hard  4) Custom\n";
        int choice = readInt("> ");

        if (choice == 1) {
            high = 50; attempts = 10;
        } else if (choice == 3) {
            high = 500; attempts = 10;
        } else if (choice == 4) {
            low = readInt("Lowest possible number: ");
            do {
                high = readInt("Highest possible number: ");
                if (high <= low) std::cout << "Must be greater than the lowest number.\n";
            } while (high <= low);
            do {
                attempts = readInt("Number of guesses (1-100): ");
            } while (attempts < 1 || attempts > 100);
        }

        std::uniform_int_distribution<int> dist(low, high);
        int secret = dist(rng);
        std::vector<int> history;
        bool won = false;

        std::cout << "Guess from " << low << " to " << high
                  << ". You have " << attempts << " guesses.\n";

        for (int turn = 1; turn <= attempts; ++turn) {
            int guess = readInt("Guess " + std::to_string(turn) + "/" +
                                std::to_string(attempts) + " (-9999 to quit): ");

            if (guess == -9999) break;
            if (guess < low || guess > high) {
                std::cout << "Stay within the range.\n";
                --turn;
                continue;
            }
            if (std::find(history.begin(), history.end(), guess) != history.end()) {
                std::cout << "You already tried that number.\n";
                --turn;
                continue;
            }

            history.push_back(guess);

            if (guess == secret) {
                std::cout << "Correct! You won in " << turn << " guesses.\n";
                won = true;
                break;
            }

            int distance = std::abs(secret - guess);
            std::cout << (guess < secret ? "Too low" : "Too high") << " and "
                      << (distance <= 3 ? "very hot" : distance <= 10 ? "hot" : "cold")
                      << ".\n";
        }

        if (!won) std::cout << "The number was " << secret << ".\n";

        std::cout << "Play again? (y/n): ";
        std::string answer;
        std::cin >> answer;
        if (answer != "y" && answer != "Y") break;
    } while (true);

    std::cout << "Thanks for playing!\n";
    return 0;
}

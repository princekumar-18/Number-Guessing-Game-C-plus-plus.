📌 Project Overview
Mastermind Number Guesser is a Python-based game in which the computer generates a random secret number, and the player must guess it within a limited number of attempts.

The game provides:

Four difficulty levels.
Limited attempts based on difficulty.
Smart mathematical hints.
Temperature-based proximity feedback.
Dynamic score calculation.
Persistent high-score tracking using JSON.
Guess history and narrowed number ranges.
Time-based scoring system. 

✨ Features

1. 🎯 Multiple Difficulty Levels
Easy: Guess between 1 and 50.
Medium: Guess between 1 and 100.
Hard: Guess between 1 and 500.
Insane: Guess between 1 and 1000.

2. ❤️ Limited Attempts

Each difficulty has a predefined number of attempts.
Choose your guesses wisely to win.

3.💡 Smart Hint System

Type hint to receive a mathematical clue.
Hints may reveal:
Whether the number is even or odd.
The sum of its digits.
Divisibility information.
Whether the number lies in the lower or upper half of the current range.
Each hint costs 150 points.

4.🌡️ Temperature-Based Feedback

🔥 Boiling Hot – Within 2% of the range.
♨️ Very Warm – Within 5% of the range.
☀️ Warm – Within 15% of the range.
🌤️ Lukewarm – Within 30% of the range.
❄️ Cold – Within 50% of the range.
🧊 Freezing – More than 50% away.
📊 Dynamic Range Tracking
Displays the narrowed guessing range after every guess.
Updates the minimum and maximum possible values.
Helps players make smarter decisions.

5. 📝 Guess History

Keeps track of previously entered guesses.
Prevents duplicate guesses.
Displays previous guesses during gameplay.

6. 🏆 Scoring System

Earn points based on difficulty.
Get bonus points for remaining attempts.
Lose points based on time taken.
Lose 150 points for every hint used.

7. 💾 Persistent High Scores

Stores high scores in a local highscores.json file.
Maintains separate records for each difficulty.
Automatically updates when a new high score is achieved.

8. ⏱️ Time Tracking

Records the time taken to guess the secret number.
Includes time-based penalties in score calculation.

9. 🔄 Replay Option

Play multiple rounds.
Switch difficulty levels between rounds.
View final high scores when exiting the game.

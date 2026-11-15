import json
import os
import random
import time
from typing import Dict, Optional


class NumberGuessingGame:
    HIGHSCORE_FILE = "highscores.json"

    DIFFICULTIES = {
        "1": {"name": "Easy", "max_num": 50, "attempts": 10, "multiplier": 1},
        "2": {"name": "Medium", "max_num": 100, "attempts": 7, "multiplier": 2},
        "3": {"name": "Hard", "max_num": 500, "attempts": 9, "multiplier": 4},
        "4": {"name": "Insane", "max_num": 1000, "attempts": 10, "multiplier": 10},
    }

    def __init__(self):
        self.high_scores: Dict[str, int] = self._load_high_scores()

    def _load_high_scores(self) -> Dict[str, int]:
        """Load persistent high scores from a local JSON file."""
        if os.path.exists(self.HIGHSCORE_FILE):
            try:
                with open(self.HIGHSCORE_FILE, "r") as file:
                    return json.load(file)
            except (json.JSONDecodeError, IOError):
                pass
        return {diff["name"]: 0 for diff in self.DIFFICULTIES.values()}

    def _save_high_scores(self) -> None:
        """Save current high scores to a local JSON file."""
        try:
            with open(self.HIGHSCORE_FILE, "w") as file:
                json.dump(self.high_scores, file, indent=4)
        except IOError as e:
            print(f"⚠️ Warning: Could not save high scores ({e}).")

    def _get_proximity_feedback(self, guess: int, secret: int, max_num: int) -> str:
        """Return temperature-based proximity feedback relative to the range."""
        distance = abs(secret - guess)
        ratio = distance / max_num

        if ratio <= 0.02:
            return "🔥 BOILING HOT! (Within 2% of the range)"
        elif ratio <= 0.05:
            return "♨️ Very Warm! (Within 5% of the range)"
        elif ratio <= 0.15:
            return "☀️ Warm (Getting closer)"
        elif ratio <= 0.30:
            return "🌤️ Lukewarm"
        elif ratio <= 0.50:
            return "❄️ Cold"
        else:
            return "🧊 FREEZING! (Way off)"

    def _generate_clue(self, secret: int, low_bound: int, high_bound: int) -> str:
        """Generate a mathematical clue to help the player at a point cost."""
        clues = [
            f"The secret number is {'EVEN' if secret % 2 == 0 else 'ODD'}.",
            f"The sum of its digits is {sum(int(d) for d in str(secret))}.",
        ]

        # Check divisibility for common factors
        for factor in [3, 5, 7, 10]:
            if secret % factor == 0:
                clues.append(f"The secret number is a multiple of {factor}.")
                break
        else:
            clues.append("The secret number is NOT divisible by 3, 5, 7, or 10.")

        # Narrowed range clue if bounds are still wide
        if high_bound - low_bound > 20:
            mid = (low_bound + high_bound) // 2
            if secret <= mid:
                clues.append(f"The number is in the lower half of your current range ({low_bound} to {mid}).")
            else:
                clues.append(f"The number is in the upper half of your current range ({mid + 1} to {high_bound}).")

        return random.choice(clues)

    def select_difficulty(self) -> Optional[Dict]:
        """Prompt the user to select a difficulty level."""
        print("\n" + "=" * 45)
        print("🎯 SELECT DIFFICULTY LEVEL")
        print("=" * 45)
        for key, info in self.DIFFICULTIES.items():
            best = self.high_scores.get(info["name"], 0)
            print(
                f"[{key}] {info['name']:<8} | Range: 1-{info['max_num']:<4} "
                f"| Attempts: {info['attempts']:<2} | High Score: {best}"
            )
        print("[Q] Quit Game")
        print("=" * 45)

        while True:
            choice = input("Choose an option (1-4 or Q): ").strip().upper()
            if choice == "Q":
                return None
            if choice in self.DIFFICULTIES:
                return self.DIFFICULTIES[choice]
            print("❌ Invalid selection. Please enter 1, 2, 3, 4, or Q.")

    def play_round(self, config: Dict) -> None:
        """Execute a single round of the guessing game."""
        max_num = config["max_num"]
        max_attempts = config["attempts"]
        multiplier = config["multiplier"]
        diff_name = config["name"]

        secret_number = random.randint(1, max_num)
        attempts_left = max_attempts
        low_bound = 1
        high_bound = max_num
        hints_used = 0
        guess_history = []
        start_time = time.time()

        print(f"\n🚀 Starting {diff_name} Mode! Guess the number between 1 and {max_num}.")
        print("💡 Tip: Type 'hint' during your turn for a clue (costs 150 points).\n")

        while attempts_left > 0:
            print("-" * 45)
            print(f"📊 Narrowed Range: [{low_bound} - {high_bound}] | Attempts Left: {attempts_left}")
            if guess_history:
                print(f"📝 Previous Guesses: {', '.join(map(str, guess_history))}")

            user_input = input("\n👉 Enter your guess (or 'hint'): ").strip().lower()

            # Handle hint request
            if user_input == "hint":
                hints_used += 1
                clue = self._generate_clue(secret_number, low_bound, high_bound)
                print(f"🕵️ CLUE #{hints_used}: {clue}")
                continue

            # Validate numeric input
            if not user_input.isdigit():
                print("⚠️ Invalid input! Please enter a whole number or type 'hint'.")
                continue

            guess = int(user_input)
            if not (1 <= guess <= max_num):
                print(f"⚠️ Out of bounds! Please guess between 1 and {max_num}.")
                continue

            if guess in guess_history:
                print("⚠️ You already guessed that number! Try a different one.")
                continue

            # Record valid guess
            guess_history.append(guess)
            attempts_left -= 1

            # Check win condition
            if guess == secret_number:
                elapsed_time = round(time.time() - start_time, 2)
                self._handle_victory(
                    config, attempts_left, hints_used, elapsed_time, len(guess_history)
                )
                return

            # Provide directional and proximity feedback
            direction = "📈 TOO LOW!" if guess < secret_number else "📉 TOO HIGH!"
            if guess < secret_number:
                low_bound = max(low_bound, guess + 1)
            else:
                high_bound = min(high_bound, guess - 1)

            proximity = self._get_proximity_feedback(guess, secret_number, max_num)
            print(f"{direction} | {proximity}")

        # Handle loss condition
        print("\n" + "=" * 45)
        print(f"💀 GAME OVER! You ran out of attempts.")
        print(f"🔓 The secret number was: {secret_number}")
        print("=" * 45)

    def _handle_victory(
        self, config: Dict, attempts_left: int, hints_used: int, elapsed_time: float, total_guesses: int
    ) -> None:
        """Calculate final score, display stats, and update high scores."""
        base_score = 1000 * config["multiplier"]
        attempt_bonus = attempts_left * 150 * config["multiplier"]
        time_penalty = int(elapsed_time * 2)
        hint_penalty = hints_used * 150

        final_score = max(100, base_score + attempt_bonus - time_penalty - hint_penalty)
        diff_name = config["name"]

        print("\n" + "🎉" * 18)
        print(f"🏆 CONGRATULATIONS! You nailed it in {total_guesses} guesses!")
        print(f"⏱️ Time Taken:     {elapsed_time} seconds")
        print(f"🕵️ Hints Used:     {hints_used} (-{hint_penalty} pts)")
        print(f"⭐ Final Score:    {final_score} points")

        if final_score > self.high_scores.get(diff_name, 0):
            print(f"🔥 NEW HIGH SCORE FOR {diff_name.upper()} MODE! 🔥")
            self.high_scores[diff_name] = final_score
            self._save_high_scores()

        print("🎉" * 18)

    def run(self) -> None:
        """Main application loop."""
        print("=============================================")
        print("   🎲 WELCOME TO MASTERMIND NUMBER GUESSER   ")
        print("=============================================")

        while True:
            config = self.select_difficulty()
            if not config:
                print("\n👋 Thanks for playing! Final High Scores:")
                for mode, score in self.high_scores.items():
                    print(f"   • {mode:<8}: {score} pts")
                break

            self.play_round(config)

            again = input("\n🔄 Play another round? (Y/N): ").strip().upper()
            if again != "Y":
                print("\n👋 Thanks for playing! See you next time.")
                break


if __name__ == "__main__":
    game = NumberGuessingGame()
    game.run()
    

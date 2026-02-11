#include <iostream>
#include <chrono>
#include <thread>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <unordered_set>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

const int height = 27;
const int width = 27;
const char WALL = '#';
const char PATH = ' ';
const auto minimum_distance = 12; // Minimum distance that must be maintained between player and other entities


enum Direction { HORIZONTAL, VERTICAL }; // 0 for Horizontal, 1 for Vertical

// Struct for entity coordinates
struct Position { // Defining the position struct for entity coordinates.
	int x;
	int y;

	// Overloading the == operator for comparison
	bool operator==(const Position& pos) const {
		return x == pos.x && y == pos.y;
	}

};

// Defining the hash function for the Position struct
namespace std {
	template<>
	struct hash<Position> {
		size_t operator()(const Position& pos) const {
			size_t h1 = std::hash<int>()(pos.x); // Hash value for x
			size_t h2 = std::hash<int>()(pos.y); // Hash value for y
			return h1 ^ (h2 << 1); // Combine hash values with left shift
		}
	};
}

// Function for collecting password from keycoordinates
void PasswordGetter(int maze[height][width], Position& player, Position& key, string password) {
	int guess_num = rand() % 10 + 1;
	int player_guess;
	if (maze[player.y][player.x] == maze[key.y][key.x]) {
		std::cout << "To receive the password, guess a number between 1 and a 10" << endl;
		std::cout << "Enter your guess: ";
		std::cin >> player_guess;
		if (player_guess == guess_num) {
			std::cout << " The password is" << password << endl;
		}
		else {
			std::cout << "Unfortunately, that is incorrect." << endl;
			std::cout << "Try again another time." << endl;
		}
	}
}

// Function that manages the player and exit interactions
void ExitandPlayerInteraction(int maze[height][width], Position& player, Position& enemy, Position& exit, string password, string user_answer, bool& PasswordCorrect) {
	if (player.x == exit.x && player.y == exit.y) {
		std::cout << "You've made it this far agent P" << endl;
		std::cout << "But to escape, you need the special code!" << endl;
		std::cout << "What is the special code? : ";
		std::cin >> user_answer;
		if (user_answer != password) {
			std::cout << "That's wrong!" << endl;
			std::cout << "Hurry agent P before it's too late!: " << endl;

		}
		else {
			std::cout << "Congratulations! You have escaped the maze." << endl;
			PasswordCorrect = true;
		}
	}

}

// Function to delay typing
void TypeEffect(const string& text, int delay_ms = 20) {
	for (char c : text) {
		cout << c << flush;
		this_thread::sleep_for(chrono::milliseconds(delay_ms));
	}
	std::cout << endl;
}

// Function to describe what goes on when player and information point interact
void PlayerInfo(Position& player, Position& info, int maze[height][width], int& num_M) {
	int hint_choice;
	if (player.x == info.x && player.y == info.y) {
		num_M++;
		TypeEffect("Welcome Agent P! I see you made it to our secret information point.");
		TypeEffect("I can give you a hint for each of the four methods that you can use to disable the alarm system ");
		TypeEffect("Choose wisely: ");
		std::cin >> hint_choice;

		switch (hint_choice) {
		case 1:
			TypeEffect("The first method requires you to solve a math puzzle.");
			TypeEffect("Sounds easy, but there's limited time so be sharp!");
			TypeEffect("Good luck, agent P! - M");
			break;
		case 2:
			TypeEffect("The second method requires you to identify and name the capital of a country.");
			TypeEffect("Make sure your geography is up to scratch!");
			TypeEffect(" These two countries are as far from each other as they are in size.");
			TypeEffect(" But one thing they have in common is snow-or lack thereof.");
			TypeEffect("Good luck, agent P! - M");
			break;
		case 3:
			TypeEffect("The third method requires you to identify a poet by a line from their work.");
			TypeEffect(" E is for Effort, remember that.");
			TypeEffect("Good luck, agent P! - M");
			break;
		case 4:
			TypeEffect("The fourth method requires you to enter the enemy agency code.");
			TypeEffect("This one is the hardest of all.");
			TypeEffect("Even after months of decryption, we still haven't cracked it.");
			TypeEffect("But the guys back at HQ working on it provided us with a hint.");
			TypeEffect("It's the last two digits of the birthyears of three of the world's most famous singers.");
			TypeEffect("So, six digits in total");
			TypeEffect("But the order isn't clear. One said it's in order of their births, the other said the order of their deaths.");
			TypeEffect("Good luck, agent P! - M");
			break;
		default:
			TypeEffect(" I'm sorry, what was that?");
			break;
		}
	}

}

bool ManhattanDistance(Position& pos1, Position& pos2, int minimum_distance) {
	return abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y) >= minimum_distance;
}

// Function to initliaze maze with cells (with all being 0)
void InitializeMaze(int maze[height][width], int height, int width) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			maze[y][x] = 0;
		}
	}
}

// Function to initialize maze with boundaries
void AddBoundaries(int maze[height][width], int height, int width) {

	// First check to see if Recursive Division Algorithm has added boundaries
	if (maze[0][0] == 1 && maze[height - 1][width - 1] == 1) {
		return;
	}

	// Top and bottom boundaries
	for (int x = 0; x < width; x++) {
		maze[0][x] = 1;
		maze[height - 1][x] = 1;
	}

	// Left and right boundaries
	for (int y = 0; y < height; y++) {
		maze[y][0] = 1;
		maze[y][width - 1] = 1;
	}
}

// Function to display the maze
void DisplayMaze(int maze[height][width], int height, int width, Position& player, Position& enemy, Position& exit, Position& key, Position& info) {
	AddBoundaries(maze, height, width); // Add boundaries to the maze
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (player.x == x && player.y == y) {
				std::cout << 'P';
			}
			else if (enemy.x == x && enemy.y == y) {
				std::cout << 'E';
			}
			else if (exit.x == x && exit.y == y) {
				std::cout << 'X';
			}
			else if (key.x == x && key.y == y) {
				std::cout << 'K';
			}
			else if (info.x == x && info.y == y) {
				std::cout << 'M';
			}
			else {
				std::cout << (maze[y][x] == 1 ? WALL : PATH);
			}
		}
		std::cout << endl; // Creating a new line for each row
	}
}

// Introdcutory message from handler
void OpeningMessage() {
	TypeEffect("Greetings, Agent P!");
	TypeEffect(" Congratulations on getting the encoded information.");
	TypeEffect("Unfortunately, the mission has been compromised and the enemy agency has sensed your presence.");
	TypeEffect("As such, they have deployed one of their agents to capture you.");
	TypeEffect("To escape, you must decrypt the password embeded within the keystand.");
	TypeEffect("But beware of the top-level alarm system.");
	TypeEffect("I cannot disable it for you, but I can provide with hints on how to disable it.");
	TypeEffect("If you want to know more, meet me by our secret information point - M.");
}

// Function that checks and generates coordinates for exit, enemy and key entities relative to distance from player
bool CoordinateGenerationAndChecking(int maze[height][width], Position& player, Position& pos, unordered_set<Position>& OccupiedPositions, int max_attempts) {
	int attempts = 0;

	// Generate coordinate for moveable
	do {

		if (attempts >= max_attempts) {
			return false;
		}

		// Generate random coordinates
		pos.x = rand() % (width - 2) + 1;
		pos.y = rand() % (height - 2) + 1;


		Position given_position = { pos.x, pos.y };

		// Check if the position is already occupied
		bool isPostionValid =
			maze[pos.y][pos.x] == 0 && // Ensuring position is not a wall
			ManhattanDistance(player, pos, minimum_distance) && // Ensuring position is not too close to player
			OccupiedPositions.find(given_position) == OccupiedPositions.end(); // Ensuring position is not already occupied

		if (isPostionValid) {
			OccupiedPositions.insert(given_position);
			return true;
		}
	} while (true);
}

// Function that generates coordinates for player, enemy, exit and key
bool GeneratingCoordinates(int maze[height][width], Position& player, Position& enemy, Position& exit, Position& key, Position& info, unordered_set<Position>& OccupiedPositions) {
	// Generate coordinates for player first
	do {
		player.x = rand() % (width - 2) + 1;
		player.y = rand() % (height - 2) + 1;

	} while (maze[player.y][player.x] == 1);

	OccupiedPositions.insert(player); // Inserting player into OccupiedPositions set so that it can be checked when generating coordinates for other entities

	// Generate coordinate for enemy for enemy, exit and key

	if (!CoordinateGenerationAndChecking(maze, player, enemy, OccupiedPositions, 1000)) {
		return false;
	}
	else if (!CoordinateGenerationAndChecking(maze, player, exit, OccupiedPositions, 1000)) {
		return false;
	}
	else if (!CoordinateGenerationAndChecking(maze, player, key, OccupiedPositions, 1000)) {
		return false;
	}
	else if (!CoordinateGenerationAndChecking(maze, player, info, OccupiedPositions, 1000)) {
		return false;
	}
	else {
		return true;
	}
}
// Function that determines the direction to be choosen
Direction choose_orientation(int height, int width) {
	if (height < width) {
		return VERTICAL;
	}
	else if (width < height) {
		return HORIZONTAL;
	}
	else {
		return rand() % 2 ? VERTICAL : HORIZONTAL;
	}
}

// Function for recursive division
void Divide(int maze[height][width], int x, int y, int h, int w, Position& player, Position& enemy, Position& exit, Position& key, Position& info, int depth = 0, int max_depth = 8) {
	// Stop dividing if the section is too small or we've reached maximum depth
	if (h <= 3 || w <= 3 || depth >= max_depth) {
		return;
	}

	Direction orientation = choose_orientation(h, w);
	int wall_x, wall_y;
	// Minimum passages for connectivity
	const int minimum_passages = 3;

	// Display maze for visualization 
	DisplayMaze(maze, height, width, player, enemy, exit, key, info);
	this_thread::sleep_for(chrono::milliseconds(2));

	if (orientation == HORIZONTAL) {
		if (h <= 4) return;

		// Randomly choosing a wall position (x will not change)
		wall_y = y + 2 + (rand() % (h - 4));
		vector<int> possible_passages;

		// Collect possible passage points, ensuring better distribution
		for (int i = 0; i < w; i++) {
			int current_x = x + i;
			// Ensured regular spacing
			if (current_x > 0 && current_x < width - 1 &&
				(i == 0 || i == w - 1 || i % 3 == 0)) {
				possible_passages.push_back(current_x);
			}
		}

		// Create evenly distributed passages
		int num_passages = minimum_passages + (rand() % 3);
		vector<int> passages;

		// Ensure at least one passage near each end of the wall
		if (!possible_passages.empty()) {
			// Add passage near start
			int start_idx = rand() % min(3, (int)possible_passages.size());
			passages.push_back(possible_passages[start_idx]);
			possible_passages.erase(possible_passages.begin() + start_idx);

			// Add passage near end if possible
			if (!possible_passages.empty()) {
				int end_idx = max(0, (int)possible_passages.size() - 3) +
					(rand() % min(3, (int)possible_passages.size()));
				passages.push_back(possible_passages[end_idx]);
				possible_passages.erase(possible_passages.begin() + end_idx);
			}
		}

		// Add remaining passages
		while (passages.size() < num_passages && !possible_passages.empty()) {
			int idx = rand() % possible_passages.size();
			passages.push_back(possible_passages[idx]);
			possible_passages.erase(possible_passages.begin() + idx);
		}

		// Drawing the wall
		for (int i = 0; i < w; i++) {
			int current_x = x + i;
			bool is_passage = false;

			for (int passage : passages) {
				if (current_x == passage) {
					is_passage = true;
					break;
				}
			}

			// Wall placement logic
			if (!is_passage && current_x > 0 && current_x < width - 1 &&
				wall_y > 0 && wall_y < height - 1) {

				// Checking for entities before placing wall
				if ((current_x != player.x || wall_y != player.y) &&
					(current_x != enemy.x || wall_y != enemy.y) &&
					(current_x != exit.x || wall_y != exit.y) &&
					(current_x != key.x || wall_y != key.y) &&
					(current_x != info.x || wall_y != info.y)) {

					// Ensure we're not creating a dead end
					int nearby_walls = 0;
					if (nearby_walls >= 2) continue; // Skip wall placement  to not create too many adjacent walls
					if (wall_y > 0 && maze[wall_y - 1][current_x] == 1) nearby_walls++;
					if (wall_y < height - 1 && maze[wall_y + 1][current_x] == 1) nearby_walls++;
					if (current_x > 0 && maze[wall_y][current_x - 1] == 1) nearby_walls++;
					if (current_x < width - 1 && maze[wall_y][current_x + 1] == 1) nearby_walls++;

					if (nearby_walls <= 1) {  // Only place wall if it won't create a dead end
						maze[wall_y][current_x] = 1;
					}
				}
			}
		}

		// Create dimensions for subfields
		int top_height = wall_y - y;
		int bottom_height = h - (wall_y - y + 1);

		// Recursively divide the remaining subfields
		if (top_height > 3)
			Divide(maze, x, y, top_height, w, player, enemy, exit, key, info, depth + 1, max_depth);
		if (bottom_height > 3)
			Divide(maze, x, wall_y + 1, bottom_height, w, player, enemy, exit, key, info, depth + 1, max_depth);
	}
	else {  // VERTICAL division
		if (w <= 4) return;

		// Randomly choosing a wall position (y will not change)
		wall_x = x + 2 + (rand() % (w - 4));
		vector<int> possible_passages;

		// Similar improvements for vertical walls
		for (int i = 0; i < h; i++) {
			int current_y = y + i;
			if (current_y > 0 && current_y < height - 1 &&
				(i == 0 || i == h - 1 || i % 3 == 0)) {
				possible_passages.push_back(current_y);
			}
		}
		// Create evenly distributed passages
		int num_passages = minimum_passages + (rand() % 2);
		vector<int> passages;

		// Ensure passages near top and bottom
		if (!possible_passages.empty()) {
			int top_idx = rand() % min(3, (int)possible_passages.size());
			passages.push_back(possible_passages[top_idx]);
			possible_passages.erase(possible_passages.begin() + top_idx);

			if (!possible_passages.empty()) {
				int bottom_idx = max(0, (int)possible_passages.size() - 3) +
					(rand() % min(3, (int)possible_passages.size()));
				passages.push_back(possible_passages[bottom_idx]);
				possible_passages.erase(possible_passages.begin() + bottom_idx);
			}
		}
		// Add remaining passages 
		while (passages.size() < num_passages && !possible_passages.empty()) {
			int idx = rand() % possible_passages.size();
			passages.push_back(possible_passages[idx]);
			possible_passages.erase(possible_passages.begin() + idx);
		}

		for (int i = 0; i < h; i++) {
			int current_y = y + i;
			bool is_passage = false;

			for (int passage : passages) {
				if (current_y == passage) {
					is_passage = true;
					break;
				}
			}
			// Drawing the wall
			if (!is_passage && wall_x > 0 && wall_x < width - 1 &&
				current_y > 0 && current_y < height - 1) {

				// Making sure entities are not covered by walls
				if ((wall_x != player.x || current_y != player.y) &&
					(wall_x != enemy.x || current_y != enemy.y) &&
					(wall_x != exit.x || current_y != exit.y) &&
					(wall_x != key.x || current_y != key.y) &&
					(wall_x != info.x || current_y != info.y)) {


					int nearby_walls = 0;
					if (current_y > 0 && maze[current_y - 1][wall_x] == 1) nearby_walls++;
					if (current_y < height - 1 && maze[current_y + 1][wall_x] == 1) nearby_walls++;
					if (wall_x > 0 && maze[current_y][wall_x - 1] == 1) nearby_walls++;
					if (wall_x < width - 1 && maze[current_y][wall_x + 1] == 1) nearby_walls++;

					if (nearby_walls <= 1) {
						maze[current_y][wall_x] = 1;
					}
				}
			}
		}
		// Creating dimensions for subfields
		int left_width = wall_x - x;
		int right_width = w - (wall_x - x + 1);

		// Recursively divide the remaining subfields
		if (left_width > 3)
			Divide(maze, x, y, h, left_width, player, enemy, exit, key, info, depth + 1, max_depth);
		if (right_width > 3)
			Divide(maze, wall_x + 1, y, h, right_width, player, enemy, exit, key, info, depth + 1, max_depth);
	}
}

// Function for MathPuzzle
void MathPuzzle(int num_of_pmoves, Position& player, Position& enemy, bool& alarmgameover, int& score) {
	int player_answer = 0;
	int num1, num2, num3;
	int correct_answer;

	// Generating two random numbers between 1 and 100 and storing them in num1 and num2
	num1 = rand() % 100 + 1;
	num2 = rand() % 100 + 1;
	num3 = rand() % 100 + 1;

	// Storing the sum between num1 and num2 in correct_answer
	correct_answer = (num1 + num2) * num3;

	// Setting the time limit to five seconds
	int time_limit = 10;


	if (num_of_pmoves % 10 == 0) {

		// Asks the player the math question
		std::cout << "What is " << num1 << " + " << num2 << " * " << num3 << endl;
		std::cout << "You have " << time_limit << " seconds to answer." << endl;

		// Starts the timer
		auto start = std::chrono::high_resolution_clock::now();

		// Loop for continuously checking timer

		while (true) {
			// Calculate the current and elapsed time since the start
			auto current_time = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(current_time - start).count();

			if (duration >= time_limit) {
				std::cout << "Time's up, you've been caught" << endl;
				alarmgameover = true;
				return;
			}

			// User can try repeatedly until time limit has been reached
			std::cout << "You have " << time_limit - duration << " seconds left. Enter your answer: " << endl;
			std::cin >> player_answer;

			// Check if the player's answer is correct
			if (player_answer == correct_answer) {
				std::cout << "System disabled, proceed as usual." << endl;
				alarmgameover = false;
				score++;
				return;
			}
			else {
				std::cout << "Incorrect! Try again before it's too late!" << endl;
			}

			this_thread::sleep_for(std::chrono::milliseconds(100));
		}
	}

}

// Function for CapitalsQuiz
void CapitalsQuiz(int num_of_pmoves, Position& player, Position& enemy, bool& alarmgameover, int& score) {
	string player_answer;
	string capital1, capital2;
	string country1, country2;
	string correct_answer;

	// Define the countries and their capitals
	country1 = "Vietnam";
	country2 = "Saint Kitts and Nevis";
	capital1 = "Hanoi";
	capital2 = "Basseterre";

	// Randomly decide what the correct answer is
	string country_question = rand() % 2 ? country1 : country2;
	correct_answer = country_question == country1 ? capital1 : capital2;



	// Setting the time limit to 10 seconds
	int time_limit = 10;

	if (num_of_pmoves % 10 == 0) {

		// Asks the player the  question
		std::cout << "You have " << time_limit << " seconds to answer." << endl;
		std::cout << "What is the capital of " << country_question << " ?: " << endl;


		// Starts the timer
		auto start = std::chrono::high_resolution_clock::now();

		bool answered_correctly = false; // Flag to check if the player answered correctly

		// Loop for continuously checking timer

		while (true) {
			// Calculate the current and elapsed time since the start
			auto current_time = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(current_time - start).count();

			if (duration >= time_limit) {
				std::cout << "Time's up! The enemy agents are on their way..." << endl;
				alarmgameover = true;
				return;
			}

			// User can try repeatedly until time limit has been reached
			std::cout << "You have " << time_limit - duration << " seconds left. Enter your answer: " << endl;
			std::cin.ignore();
			std::getline(std::cin, player_answer);

			// Check if the player's answer is correct

			if (player_answer == correct_answer) {
				std::cout << "System has been disabled, proceed as usual." << endl;
				alarmgameover = false;
				score++;
				return;
			}
			else {
				std::cout << "Incorrect! Try again before it's too late" << endl;
			}

			this_thread::sleep_for(std::chrono::milliseconds(100));
		}

	}
}

// Function for PoetVerse
void PoetVerse(int num_of_pmoves, Position& player, Position& enemy, bool& alarmgameover, int& score) {
	// Initialize variables
	string player_answer;
	string quote1, quote2;
	string poet1, poet2;
	string correct_answer;
	string question;

	// Define the quotes and their poets
	poet1 = "Emily Dickinson";
	poet2 = "Edgar Allen Poe";
	quote1 = "Unable are the loved to die, for love is immortality.";
	quote2 = "All that we see or seem is but a dream within a dream.";

	// Randomly decide what the correct answer and question is
	question = rand() % 2 ? quote1 : quote2;
	correct_answer = question == quote1 ? poet1 : poet2;

	// Setting the time limit to 10 seconds
	int time_limit = 10;

	if (num_of_pmoves % 10 == 0) {

		// Asks the player the  question
		std::cout << "You have " << time_limit << " seconds to answer." << endl;
		std::cout << "Which famous poet wrote the following line: " << question << " ?: " << endl;


		// Starts the timer
		auto start = std::chrono::high_resolution_clock::now();

		// Loop for continuously checking timer

		while (true) {
			// Calculate the current and elapsed time since the start
			auto current_time = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(current_time - start).count();

			if (duration >= time_limit) {
				std::cout << "Time's up! The enemy agents are on their way" << endl;
				alarmgameover = true;
				return;
			}

			// User can try repeatedly until time limit has been reached
			std::cout << "You have " << time_limit - duration << " seconds left. Enter your answer: " << endl;
			std::cin.ignore();
			std::getline(std::cin, player_answer);

			// Check if the player's answer is correct

			if (player_answer == correct_answer) {
				std::cout << "Alarm system disabled. Proceed as usual..." << endl;
				alarmgameover = false;
				score++;
				return;
			}
			else {
				std::cout << "Incorrect! Try again" << endl;
			}

			this_thread::sleep_for(std::chrono::milliseconds(100));
		}

	}
}

// Function for EnemyAlarmCode
void EnemyAlarmCode(int num_of_pmoves, Position& player, Position& enemy, bool& alarmgameover, int& score) {
	// Initialize variables to be used
	int player_answer;
	int enemy_agencycode = 354658;
	int correct_answer = enemy_agencycode;

	// Setting the time limit to 10 seconds
	int time_limit = 20;

	if (num_of_pmoves % 10 == 0) {

		// Asks the player to input the enemy agency code
		std::cout << "You have " << time_limit << " seconds to answer." << endl;
		std::cout << "What is the code to disable the alarm system? " << endl;
		// Starts the timer
		auto start = std::chrono::high_resolution_clock::now();

		// Use of seperate thread to check for time limit

		// Loop for continuously checking timer

		while (true) {
			// Calculate the current and elapsed time since the start
			auto current_time = std::chrono::high_resolution_clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(current_time - start).count();

			if (duration >= time_limit) {
				std::cout << "Time's up! The enemy agents are on their way..." << endl;
				alarmgameover = true;
				return;
			}

			// Stop loop if user answered correctly

			// User can try repeatedly until time limit has been reached
			std::cout << "You have " << time_limit - duration << " seconds left. Enter your answer: " << endl;
			std::cin >> player_answer;

			// Check if the player's answer is correct
			if (player_answer == correct_answer) {
				std::cout << "System has been disabled. Proceed as usual." << endl;
				alarmgameover = false;
				score++;
				return;
			}
			else {
				std::cout << "Incorrect! Try again!" << endl;
			}

			this_thread::sleep_for(std::chrono::milliseconds(100));
		}

	}




}

// Function that manages player movement
void PlayerMovement(int maze[height][width], Position& player, Position& enemy, Position& key, Position& exit, Position& info, string password, string user_answer, int& num_of_pmoves, bool& PasswordCorrect, bool& alarmgameover, int& num_M, bool& EnemyandPlayer, int& score) {
	num_of_pmoves;
	int num_system_disabler;
	int i = 1;
	char move;
	std::cout << "Enter your move (w, a, s, d): ";
	std::cin >> move;
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	bool moved = false;

	if (player.x >= 0 && player.x < width - 1 && player.y >= 0 && player.y < height - 1) {
		switch (move) {
		case 'w':
			if (maze[player.y - i][player.x] != 1) {
				player.y -= i;
				moved = true;
			}
			break;
		case 'a':
			if (maze[player.y][player.x - i] != 1) {
				player.x -= i;
				moved = true;
			}
			break;
		case 's':
			if (maze[player.y + i][player.x] != 1) {
				player.y += i;
				moved = true;
			}
			break;
		case 'd':
			if (maze[player.y][player.x + i] != 1) {
				player.x += i;
				moved = true;
			}
			break;
		default:
			std::cout << "Invalid move. Please try again: " << endl;
		}
	}

	if (moved) {
		num_of_pmoves++;
		if (enemy.y == player.y && enemy.x == player.x) {
			EnemyandPlayer = true;

		}
	}

	if (player.x == key.x && player.y == key.y) {
		PasswordGetter(maze, player, key, password);
	}
	if (player.x == exit.x && player.y == exit.y) {
		ExitandPlayerInteraction(maze, player, enemy, exit, password, user_answer, PasswordCorrect);
	}
	if (player.x == info.x && player.y == info.y) {
		PlayerInfo(player, info, maze, num_M);
	}
	if (num_M > 0) {
		if (num_of_pmoves % 10 == 0) {
			std::cout << "Intruder Alert! Disable the system to proceed." << endl;
			std::cout << "Pick a number between 1 and 4 to pick which method to use: " << endl;
			std::cin >> num_system_disabler;
			switch (num_system_disabler) {
			case 1:
				MathPuzzle(num_of_pmoves, player, enemy, alarmgameover, score);
				break;
			case 2:
				CapitalsQuiz(num_of_pmoves, player, enemy, alarmgameover, score);
				break;
			case 3:
				PoetVerse(num_of_pmoves, player, enemy, alarmgameover, score);
				break;
			case 4:
				EnemyAlarmCode(num_of_pmoves, player, enemy, alarmgameover, score);
				break;
			}
		}
	}
}

// Function that ends the game
bool GameOver(int maze[height][width], Position& player, Position& enemy, Position& exit, string password, string user_answer, bool& PasswordCorrect, bool& alarmgameover, bool& EnemyandPlayer) {
	if (alarmgameover || EnemyandPlayer) {
		std::cout << "\nIt's over Agent P! You were caught by the enemy agents." << endl;
		return true;
	}
	else if ((player.x == exit.x && player.y == exit.y) && PasswordCorrect == true) {
		std::cout << "\nCongratulations! You have escaped the maze." << endl;
		return true;
	}
	else {
		return false;
	}
}

// Function to get the name of the player and the data played
void GetNameandDate(string& player_name, string& date) {
	std::cout << "Enter your name: ";
	std::cin >> player_name;
	std::cout << "Enter the date: ";
	std::cin >> date;
}

// Function to calculate the player score and save it to a file
void saveScore(int score, string player_name, string date) {
	ofstream scoreFile("score.txt", ios::app); // Create a file to save the player score
	if (scoreFile.fail()) {
		std::cerr << "Unable to open file" << endl; // print error message if the game is unable to save the player score
		return;
	}
	else {
		scoreFile << player_name << endl; // Save the player name to the file
		scoreFile << score << endl; // Save the player score to the file
		scoreFile << date << endl; // Save the date to the file
		scoreFile.close(); // Close the file to prevent memory leaks
	}

}

// Function to load the player score from a file
void loadScore(int score, string player_name, string date) {
	ifstream inputFile("score.txt"); // Open the file to load the player score

	if (!inputFile.is_open()) {
		std::cerr << "File could not be found" << endl; // print error message if the game is unable to load the player score
		return;
	}
	while (inputFile >> player_name >> score >> date) {
		if (inputFile.fail()) {
			cerr << "Invalid file data found" << endl;
			inputFile.clear();
			inputFile.ignore(100, '\n');
		}
		else {
			std::cout << "Player Name: " << player_name << "," << " Player Score: " << score << "," << " Date: " << date << endl; // Display the player name
		}
	}
	inputFile.close(); // Close the file to prevent memory leaks
}

// Function to calculate the Manhattan distance between two positions
int HeuristicDistance(Position& pos1, Position& pos2) {
	return abs(pos1.x - pos2.x) + abs(pos1.y - pos2.y);
}

// Function to check if player has moved - for enemy movement
bool HasPlayerMoved(int maze[height][width], int& num_of_pmoves) {
	if (num_of_pmoves > 0) {
		return true;
	}
	return false;
}

// Function for enemy movement
bool EnemyMovement(int maze[height][width], Position& enemy, Position& player, Position& exit, bool& PasswordCorrect, string user_answer, string password, int num_of_pmoves, bool& EnemyandPlayer) {
	// Define the directions that enemy can move in
	const int directions[4][2] = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

	if (!HasPlayerMoved(maze, num_of_pmoves)) {
		return false;
	}
	// Check for collision before movement
	if (enemy.y == player.y && enemy.x == player.x) {
		EnemyandPlayer = true;
		return true;
	}


	int min_distance = INT_MAX; // Store the minimum distance between enemy and player
	int min_index = -1; // Store the index of the minimum distance

	for (int i = 0; i < 4; i++) {
		int newx = enemy.x + directions[i][0];
		int newy = enemy.y + directions[i][1];

		// Check to make sure positions are not out of bounds and are not walls
		if (newx >= 0 && newx < width - 1 && newy >= 0 && newy < height - 1
			&& maze[newy][newx] != 1) {
			Position newpos = { newx, newy };

			// Calculate the Manhattan distance between the enemy and player
			int distance = HeuristicDistance(newpos, player);

			if (distance < min_distance) {
				min_distance = distance;
				min_index = i;
			}
		}
	}
	// Move if a valid direciton was found
	if (min_index != -1) {
		enemy.x += directions[min_index][0];
		enemy.y += directions[min_index][1];
		// Check for collision after movement
		if (enemy.x == player.x && enemy.y == player.y) {
			EnemyandPlayer = true;
		}

		return true;
	}

	return false;
}




int main() {
	srand(static_cast<unsigned int>(time(0))); // Seed the random number generator

	int choice;
	// Main menu
	std::cout << "Welcome to Mystery Maze: Agent Escape!" << endl;
	std::cout << "Please select one of the following options" << endl;
	std::cout << "1. Start New Game" << endl;
	std::cout << "2. Score Sheet" << endl;
	std::cout << "3. Exit Game" << endl;
	std::cout << "Enter your choice: ";
	std::cin >> choice;

	// Immediately exit if player chooses 3
	if (choice == 3) {
		std::cout << "Exiting Game..." << endl;
		return 0;
	}

	while (choice != 3) {
		unordered_set<Position> OccupiedPositions;
		string password = "Basic82000";
		string user_answer;
		bool PasswordCorrect = false;
		int num_of_pmoves = 0;
		bool alarmgameover = false;
		int num_M = 1;
		bool EnemyandPlayer = false;
		int score;
		string player_name;
		string date;

		Position player, enemy, exit, key, info;
		switch (choice) {


		case 1:

			// Get name and date from player
			GetNameandDate(player_name, date);

			int maze[height][width]; // Create a 2D array for the maze
			InitializeMaze(maze, height, width); // Initialize the maze with all cells being 0

			std::cout << "Starting New Game..." << endl;
			OpeningMessage(); // Display the opening message

			// Create positions for player, enemy, exit, and key
			GeneratingCoordinates(maze, player, enemy, exit, key, info, OccupiedPositions);


			// Call Recursive Division algorithm
			Divide(maze, 0, 0, height - 2, width - 2, player, enemy, exit, key, info);

			// Display the maze
			DisplayMaze(maze, height, width, player, enemy, exit, key, info);

			// Game Logic Movement
			while (!GameOver(maze, player, enemy, exit, password, user_answer, PasswordCorrect, alarmgameover, EnemyandPlayer)) {
				PlayerMovement(maze, player, enemy, key, exit, info, password, user_answer, num_of_pmoves, PasswordCorrect, alarmgameover, num_M, EnemyandPlayer, score);
				EnemyMovement(maze, enemy, player, exit, PasswordCorrect, password, user_answer, num_of_pmoves, EnemyandPlayer);
				DisplayMaze(maze, height, width, player, enemy, exit, key, info);
			}
			saveScore(score, player_name, date); // Save the player score
			break;
		case 2:
			std::cout << "Loading Score Sheet..." << endl;
			loadScore(score, player_name, date); // Load the player score
			break;

		case 3:
			std::cout << "Exiting Game..." << endl;
			break;

		default:
			std::cout << "Invalid choice. Please try again." << endl;
			break;

		}

		std::cout << "Please select one of the following options" << endl;
		std::cout << "1. Start New Game" << endl;
		std::cout << "2. Load Score Sheet" << endl;
		std::cout << "3. Exit Game" << endl;
		std::cout << "Enter your choice: ";
		std::cin >> choice;
		if (choice == 3) {
			std::cout << "Exiting Game..." << endl;
			break;
		}
	}
	return 0;
}


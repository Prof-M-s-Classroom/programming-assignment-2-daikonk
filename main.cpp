#include "GameDecisionTree.h"
#include "Llm.h"
#include <iostream>
#include <string>
using namespace std;
int main() {
  GameDecisionTree<Story> myGame;
  string choice;
  string theme;
  string apiKey;
  string res;
  std::vector<string> llms = {"OpenAI", "Gemini", "Claude"};

  string filename = "story.txt";
  char delimiter = '|'; // '|' is the delimiter, change if needed

  while (1) {
    cout << "Choose an LLM to use to generate your story: " << endl;
    cout << "1: GPT (4o)" << endl;
    cout << "2: Gemini (flash-2.0-flash)" << endl;
    cout << "3: Claude (claude-3-7-sonnet-20250219)" << endl;
    cin >> choice;

    if (choice == "1" || choice == "2" || choice == "3") {
      cout << "Selected : " << choice << endl;
      break;
    } else {
      cout << "Invalid choice" << endl;
    }
  }

  while (1) {

    cout << "Enter an API key for " << llms[stoi(choice) - 1] << endl;
    cin >> apiKey;

    cout << "Now, write a short description of the theme of the story" << endl;
    std::getline(std::cin >> std::ws, theme);

    cout << "Thinking..." << endl;
    LLM llm(apiKey, theme, choice);
    res = llm.generate();

    if (res[0] != '{') {
      cout << "Successfully Generated!" << endl;
      break;
    } else {
      cout << res << endl;
    }
  }

  myGame.writeToFile(filename, res);

  // Load the story data from the file
  myGame.loadStoryFromFile(filename, delimiter);

  // Start the game
  myGame.playGame();

  return 0;
}

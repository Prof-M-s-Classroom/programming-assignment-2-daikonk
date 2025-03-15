#include "GameDecisionTree.h"
#include <iostream>
#include <string>
using namespace std;
int main() {
  GameDecisionTree<Story> myGame;

  string filename =
      "../story.txt"; // This is necessary for me, I'm using CMAKE from cli and
                      // run cmake --build . from build folder
  //
  char delimiter = '|'; // '|' is the delimiter, change if needed

  // Load the story data from the file
  myGame.loadStoryFromFile(filename, delimiter);

  // Start the game
  myGame.playGame();

  return 0;
}

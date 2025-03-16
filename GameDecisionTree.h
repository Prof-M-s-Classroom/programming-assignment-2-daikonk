#ifndef GAMEDECISIONTREE_H
#define GAMEDECISIONTREE_H

#include "Node.h"
#include "Story.h"
#include <exception>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <unordered_map>
#include <vector>

template <typename T> class GameDecisionTree {
private:
  Node<T> *root;
  std::unordered_map<int, Node<T> *> seen;

public:
  // TODO: Constructor
  GameDecisionTree() : root(nullptr) {}

  // TODO: Function to load story data from a text file and build the binary
  // tree
  void loadStoryFromFile(const std::string &filename, char delimiter) {

    std::ifstream file(filename);
    std::string line;
    std::string temp;
    std::map<int, Story> stories;
    std::unordered_map<int, Node<T> *> seen;

    if (!file.is_open()) {
      std::cerr << "Error: could not open file: " << filename << endl;
      return;
    }

    while (std::getline(file, line)) {
      std::istringstream stream(line);

      try {

        std::getline(stream, temp, delimiter);
        int eventNum = std::stoi(temp);

        std::getline(stream, temp, delimiter);
        std::string desc = temp;

        std::getline(stream, temp, delimiter);
        int leftChildNum = std::stoi(temp);

        std::getline(stream, temp, delimiter);
        int rightChildNum = std::stoi(temp);

        stories[eventNum] = Story(desc, eventNum, leftChildNum, rightChildNum);

      } catch (const std::invalid_argument &e) {
        std::cerr << "\nERROR: Failed to parse, invalid arguments\n"
                  << line << endl;
        ;
        return;
      }
    }

    file.close();

    // I decided to go with a recursive approach here because it would be
    // simpler, I also plan to do an itterative bfs for print debugging on my
    // tree
    this->root = buildTree(stories, stories.begin()->first);
    printTreeBfs(this->root);
  }

  // TODO: Function to start the game and traverse the tree based on user input
  void playGame() {
    Node<Story> *node = root;
    string input;
    while (node) {
      cout << node->data.description << endl;
      if (node->data.leftEventNumber != -1 &&
          node->data.rightEventNumber != -1) {
        cout << "Enter 1 or 2 to make your decision" << endl;
        cin >> input;
        cout << endl;
      }

      if (input == "1") {
        node = node->left;
      } else if (input == "2") {
        node = node->right;
      } else {
        cout << "invalid input, " << endl;
      }
    };

    cout << "finished game" << endl;
  }

private:
  Node<Story> *buildTree(const std::map<int, Story> &stories, int rootNum) {
    if (rootNum == -1) {
      return nullptr;
    }

    // seen.find will return seen.end if the search fails, we want this search
    // to fail to make sure it is a new node.
    if (seen.find(rootNum) != seen.end()) {
      return seen[rootNum];
    }

    Story currStory = stories.at(rootNum);
    Node<Story> *root = new Node<Story>(currStory);

    seen[rootNum] = root;

    root->left = buildTree(stories, currStory.leftEventNumber);
    root->right = buildTree(stories, currStory.rightEventNumber);

    return root;
  }

  void printTreeBfs(Node<Story> *root) {

    std::vector<Node<Story> *> q;
    std::unordered_map<Node<Story> *, bool> seen;
    q.push_back(root);

    while (!q.empty()) {

      Node<Story> *curr = q.front();
      q.erase(q.begin());

      std::cout << "Story " << curr->data.eventNumber << ": "
                << curr->data.description << endl;

      if (curr->left && !seen[curr->left]) {
        q.push_back(curr->left);
        seen[curr->left] = true;
      }

      if (curr->right && !seen[curr->right]) {
        q.push_back(curr->right);
        seen[curr->right] = true;
      }
    }
  }
};

#endif // GAMEDECISIONTREE_H

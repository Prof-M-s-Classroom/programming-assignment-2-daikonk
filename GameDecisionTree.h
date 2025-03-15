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
#include <vector>

template <typename T> class GameDecisionTree {
private:
  Node<T> *root;

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
    Node<Story> *root = buildTree(stories, stories.begin()->first);
    printTreeBfs(root);

    return;
  }

  // TODO: Function to start the game and traverse the tree based on user input
  void playGame() {}

private:
  Node<Story> *buildTree(const std::map<int, Story> &stories, int rootNum) {
    if (rootNum == -1) {
      return nullptr;
    }

    Story currStory = stories.at(rootNum);
    cout << "found " << rootNum << endl;
    Node<Story> *root = new Node<Story>(currStory);

    root->left = buildTree(stories, currStory.leftEventNumber);
    root->right = buildTree(stories, currStory.rightEventNumber);

    return root;
  }

  void printTreeBfs(Node<Story> *root) {

    std::vector<Node<Story> *> q;
    q.push_back(root);

    while (!q.empty()) {

      Node<Story> *curr = q.front();
      q.erase(q.begin());

      std::cout << "Story " << curr->data.eventNumber << ": "
                << curr->data.description << endl;

      if (curr->left) {
        q.push_back(curr->left);
      }

      if (curr->right) {
        q.push_back(curr->right);
      }
    }
  }
};

#endif // GAMEDECISIONTREE_H

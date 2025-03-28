[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/jIKk4bke)
# CS210 Data Structures SP25
## Programming Assignment 2: Text-Based RPG Using Decision Trees

### **Student Name:** `Nicholas Stoneking`  
### **Student ID:** `825228346`  

---

## **1. Project Overview**
> This project implements a text-based RPG using a binary decision tree. This tree, is generated a file which in turn is generated by an llm with its own unique story based on a user input on the theme. The story is loaded from the text to create the tree. The game ends when a node is reached with -1 as the left and right childen.

---

## **2. Folder & File Structure**
(Explain the role of each file in your project.)

- **`main.cpp`** → `Creates GameDecisionTree, sets the file name and delimiter, calls the loadStory function from theGameDecisionTree class, and starts the game.`
- **`GameDecisionTree.h`** → `Contains the decision tree constructor, a load story function which parses the file and creates a map of Stories, a buildTree function that recursively builds the BST, and lastly a printTree function that uses a BFS algo to for debugging purposes`
- **`Node.h`** → `Contructor for Node class`  
- **`Story.h`** → `Constructor for Story`
- **`story.txt`** → `Contains the story`
- **`llm.h`** → `Has construtor and relevant methods for our llm class. This class has to be created with a model choice, theme, and apiKey. We then can call llm.generate() to create a story.txt based on the user chosen model and theme. There are a few break downs in this file to make it read better, namely the POST request function is reusable across model requests where only the curl arguments like headers, body, and url are needed. Similarly, my http code validation is also in its own function.`
- **`prompt.txt`** → `Contains the prompt to be used by the pipeline`

---

## **3. Decision Tree Construction**
(Explain how the decision tree is built from the `story.txt` file.)

- How are `Story` objects created from the text file?
  - Story objects are created by parsing each line from the file, then parsing each value delimited by our delimiter character. Each line creates 1 Story
- How are nodes linked based on left and right event numbers?
  - Nodes are linked via their eventNumber. I used a map to store each Event by their eventNumber to give a constant look-up time when needing to connect the relevant Nodes.
- What happens if an event has `-1` as left or right children?
  - It means it is terminal, or a leaf node.

---

## **4. Game Traversal**
(Describe how the game moves through the decision tree.)

- How does user input determine the path taken?
  - The use input determines whether or not i set the current node to node->left or node->right
- What happens when the player reaches a leaf node?
  - The game ends
- Explain the special case where multiple events lead to the same outcome.
  - The special case where there is more than one out come didnt need to be handled any differently, some of the nodes will lead to a single node, no issue was encountered when using my recursive construction OR my bfs iterative printTree function

---

## **5. File Parsing & Data Loading**
(Explain the process of reading `story.txt` and constructing the decision tree.)

- How is the file read and parsed using a delimiter?
  - I create a while loop that continues while there is another line to be loaded into my "line" variable. From there, we parse each line 4 times using getline(*string stream*, *var*, *delim*). Doing it this way keeps the code a bit cleaner, and might help find errors a little easier when constructing from the story.txt file
- How do you ensure nodes are properly linked?
  - One way that I did, was create a utility that printed the tree after its creation. I used a BFS search to print everything, which made it really easy because it printed in-order.
- Any challenges faced while handling file input?
  - I had one issue initially, when trying to break it up into more readable sections, i tried passing line directly into getline, but getline doesn't have a function that accepts strings as an input, so instead I had to create a string stream like the one returned from `ifstream file(filename)`

---

## **6. Debugging Process (Errors & Fixes)**
> Initially, I grabbed line by line, and was planning on parsing char by char, manually extracting what I needed. This created a more ugly debugging process, as well as ugly looking code. I then checked out some of the library docs that were already included, and found that the getline function I was already using took a delimiter as an input, so I used that instead then wrapped the whole thing in a try catch

Example:
> Initially, my program was crashing when trying to access an uninitialized node. I realized it was because I was not properly checking for `nullptr` before accessing child nodes, so I added a check to prevent accessing uninitialized memory.

---

## **7. Sample Output & Walkthrough**
(Provide an example of how the game runs, including player input.)

---

## **8. Big-O Analysis of Core Methods**
(Analyze the complexity of key methods.)

- **Loading the tree from the file** → `O(nlog(n))`
- **Searching for an event in the tree** → `O(logn)`
- **Game traversal efficiency** → `O(n)`

---

## **9. Edge Cases & Testing**
(Describe at least one edge case you tested and its outcome.)

> My biggest edge case is a failed tree construction since it is generated. This is mitigated by doing proper error handling in the tree construction if it fails.
> Another edge case is jailbreaking my app, I mitigated this the best I could by making sure to isolate the variable in the prompt.
> One more related to Nodes would be an issue with recursive construction, since I don't have it specifically at the bottom everytime, I needed to add a dictionary of seen Nodes on construction AND traversal, to make sure that we do not attemp to reconnect them. If we do, this results in an infinite loop.

Example:
> I tested a scenario where the player reaches a dead-end (leaf node) and ensured the program correctly ends the game.

---

## **10. Justification of Additional Features (If Any)**
(If you added extra features, describe them and explain why.)

> I wanted make this story more customizable and unique, so I made a small pipeline that allows the user to choose the model, and theme of their story by which it gets generated by a prompt. This gives flexibility for the user to customize their story a bit more and incorperates some more modern interests in software engineering.

Example:
> I added a **save/load feature** so players can resume the game later. This was done by writing the current node’s event number to a file and reading it when restarting.

---

## **Final Notes**
- **README is mandatory** → `(-10 points deduction if missing).`  
- **Code must compile and run** → `(Code that does not run will receive a 0).`  
- **Do not modify provided skeleton code** → `(You may only add to it).`  

---

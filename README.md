# B-Tree Implementation

This project implements a B-tree structure in C++ with three primary files:

- `main.cpp`: Contains the main function to interact with the B-tree.
- `Btree.h`: Header file defining the `Btree` class.
- `Btree.cpp`: Implementation file with method definitions for the `Btree` class.

This project was developed as an assignment for **File Management and Processing - Assignment 2**.

## Project Structure

```plaintext
├── Btree.h      # Header file with B-tree class definition
├── Btree.cpp    # Implementation of the Btree class methods
├── main.cpp     # Main file to interact with the B-tree through a simple menu
```
# Class: - `Btree`
The Btree class is responsible for managing B-tree operations on data stored in a file. 
This B-tree supports key operations such as insertion, deletion, and display, with nodes represented as records in a file to support persistent storage.

# Key Features
* File-based Structure: Nodes are stored as file records, enabling scalable storage and retrieval.
* Dynamic Node Splitting: Nodes automatically split when they exceed the allowed size.
* Insert, Delete, and Display Operations: Supports essential B-tree operations for data manipulation.
* Persistent Storage: Data is saved in a file, allowing for recovery and storage between sessions.

# Methods in - `Btree.h`
* Constructor: Initializes the B-tree with specified record, child, and cell sizes.
* openFile: Opens the file used for storing the B-tree data.
* buildTree: Constructs the B-tree from scratch.
* insert: Inserts a new record (key-value pair) into the B-tree.
* fillNodeWithValues: Fills a node with specified key-value pairs.
* changeRecordStatus: Updates a record's status as leaf or non-leaf.
* updateAfterInsert: Updates pointers after insertion when nodes split.
* split: Splits a node if it exceeds the allowed size.
* removeRecord: Removes a record from the B-tree.
* display: Displays the structure of the B-tree.
* Destructor: Ensures all resources are properly closed upon deletion of the Btree object.

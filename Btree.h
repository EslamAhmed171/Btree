//
// Created by Eslam Ahmed on 8/31/2024.
//

#ifndef BTREE_BTREE_H
#define BTREE_BTREE_H

#include <bits/stdc++.h>
using namespace std;
class Btree{
private:
    // file name
    fstream file;
    // The number of values one node can hold.
    int descendants;
    // number of records the b-tree file can hold.
    int numberOfRecords;
    // size of each cell in Btree
    int charSize;
public:
    // constructor
    Btree(int records, int children, int size);
    // open file for change
    void openFile();
    // build tree
    void buildTree();
    // display
    void display();
    // destructor
    ~Btree();

    // insert
    int insert(int recordId, int reference);
    // fill node with values
    void fillNodeWithValues(const vector<pair<int, int>> &newNodeValues, int recordNumber);
    //   Change Record status to leaf or not
    void changeRecordStatus(int recordNumber, int leafStatus);
    // update after insert
    int updateAfterInsert(int parentRecordNumber, int newChildNumber);
    // split if size overflow
    int split(int recordNumber, vector<std::pair<int, int>> originalNode);
    // remove record
    void removeRecord(int recordNumber);
    // spilt if size overflow
    bool split(vector<pair<int, int>> root);
    int readValue(int rowIdx, int columnIdx) ;
    // write value
    void writeCell(int value, int rowIndex, int columnIndex);
    // read from record as pairs first is key and second is reference
    pair<int, int> readPair(int recordNumber, int pairNumber);
    //store all keys inside record
    std::vector<std::pair<int, int>> readNodeValues(int recordNumber);
    // mark node is non-leaf
    void markIsNonLeaf(int recordNumber);
    // Node status
    int nodeStatus(int recordNumber);
    // record size or row size
    int recordSize() const;
    // to string
    string toString(int value) const;
    int nextEmpty();
    int getCharSize() const;
    bool isValidRecord(int recordNumber) const;
    // is valid pair?
    bool isValidPair(int pairNumber) const;
    // is leaf?
    bool isLeaf(int recordNumber);
    // is empty?
    bool isEmpty(int recordNumber);

};


#endif //BTREE_BTREE_H

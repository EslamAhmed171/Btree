#include "Btree.h"
Btree::Btree(int records, int children, int size){
    numberOfRecords = records;
    descendants = children;
    charSize = size;
    openFile();
    buildTree();
}
// open file for change
void Btree::openFile() {
    file.open("index", std::ios::trunc | std::ios::in | std::ios::out);
}
// build tree
void Btree::buildTree() {
    // For each record
    for (int recordIdx = 0; recordIdx < numberOfRecords + 1; ++recordIdx) {
        // Write -1 in the first cell to indicate it is
        // an empty record (available for allocation)
        writeCell(-1, recordIdx, 0);

        // Write the number of the next empty record
        // in the available list
        if (recordIdx == numberOfRecords - 1)
            writeCell(-1, recordIdx, 1);
        else
            writeCell(recordIdx + 1, recordIdx, 1);

        // Fill the rest of the record with -1s
        for (int cellIdx = 2; cellIdx <= descendants * 2; ++cellIdx)
            writeCell(-1, recordIdx, cellIdx);
    }
}
// display
void Btree::display() {
    for (int i = 0; i < numberOfRecords; ++i) {

        char record[recordSize()];

        file.seekg(i * recordSize(), std::ios::beg);

        file.read(record, recordSize());

        for (int j = 0; j < recordSize(); ++j)
            cout << record[j];

        cout <<endl;
    }

}
// destructor
Btree::~Btree() {
    file.close();
}
// insert
int Btree::insert(int recordId, int reference) {
    vector<pair<int, int>> current; //vector represent node

    // If the root is empty
    if (isEmpty(1)) {
        // insert in root

        // The next empty for the record at next empty
        int nextEmptyNode = readValue(nextEmpty(), 1);

        // Update the next empty
        writeCell(nextEmptyNode, 0, 1);

        // Create the readNodeValues
        current = readNodeValues(1);

        // insert the new pair
        current.emplace_back(recordId, reference);

        // Sort the node
        sort(current.begin(), current.end());

        // Write the node in root
        fillNodeWithValues(current, 1);

        // Mark the root as leaf
        changeRecordStatus(1, 0);

        // Return the index of the record in which the insertion happened
        // i.e. the root in this case
        return 1;
    }
    // Keep track of visited records to updateAfterInsert them after insertion
    std::stack<int> visited;

    // Search for recordId in every node in the b-tree
    // starting with the root
    int i = 1;
    bool found;
    while (!isLeaf(i)) {
        visited.push(i);
        current = readNodeValues(i);
        found = false;
        for (auto p: current) {
            // If a greater value is found
            if (p.first >= recordId) {
                // B-Tree traversal
                i = p.second;
                found = true;
                break;
            }
        }

        // B-Tree traversal
        if (!found) i = current.back().second;
    }
    current = readNodeValues(i);
    // insert the new pair
    current.emplace_back(recordId, reference);
    // Sort the node
    sort(current.begin(), current.end());
    int newFromSplitIndex = -1;
    // If record overflowed after insertion
    if (current.size() > descendants)
        newFromSplitIndex = split(i, current);
    else
        // Write the node in root
        fillNodeWithValues(current, i);

    // If the insertion happened in root
    // Then there are no parents to updateAfterInsert
    if (i == 1) return i;

    // Otherwise, updateAfterInsert parents
    while (!visited.empty()) {
        int lastVisitedIndex = visited.top();
        visited.pop();
        newFromSplitIndex = updateAfterInsert(lastVisitedIndex, newFromSplitIndex);
    }
    // Return the index of the inserted record
    // or -1 if insertion failed
    return i;
}
// fill node with values
void Btree::fillNodeWithValues(const vector<pair<int, int>> &newNodeValues, int recordNumber) {
    removeRecord(recordNumber);
    file.seekg(recordNumber * recordSize() + charSize, std::ios::beg);
    for (auto p: newNodeValues)
        file << toString(p.first) << toString(p.second);
}
//   Change Record status to leaf or not
void Btree::changeRecordStatus(int recordNumber, int leafStatus) {
    file.seekg(recordNumber * recordSize(), std::ios::beg);
    file << toString(leafStatus);
}
// update after insert
int Btree::updateAfterInsert(int parentRecordNumber, int newChildNumber) {
    vector<pair<int, int>> parent = readNodeValues(parentRecordNumber);
    vector<pair<int, int>> newParent;
    // For each value in parent
    for (auto p: parent) {
        auto childNode = readNodeValues(p.second);
        // Add the maximum of the value's child
        newParent.emplace_back(childNode.back().first, p.second);
    }
    // If there was a new child from previous split
    if (newChildNumber != -1)
        //  Add the maximum of the new value's child
        newParent.emplace_back(readNodeValues(newChildNumber).back().first, newChildNumber);

    sort(newParent.begin(), newParent.end());

    int newFromSplitIndex = -1;

    // If record overflowed after insertion
    if (newParent.size() > descendants)
        newFromSplitIndex = split(parentRecordNumber, newParent);
    else
        // Write new parent
        fillNodeWithValues(newParent, parentRecordNumber);

    return newFromSplitIndex;
}
// split if size overflow
int Btree::split(int recordNumber, vector<std::pair<int, int>> originalNode) {
    if (recordNumber == 1)
        return split(originalNode);

    // Get the index of the new record created after split
    int newNodeIdx = nextEmpty();

    // If there are no empty records, then splitting fails

    if (newNodeIdx == -1) return -1;

    // Update the next empty cell with the next in available list
    writeCell(readValue(newNodeIdx, 1), 0, 1);

    // Distribute originalNode on two new nodes
    vector<pair<int, int>> firstNodeValues, secondNodeValues;

    // Fill first and second nodes from originalNode
    auto middle(originalNode.begin() + (int) (originalNode.size()) / 2);
    for (auto it = originalNode.begin(); it != originalNode.end(); ++it) {
        if (distance(it, middle) > 0)
            firstNodeValues.push_back(*it);
        else
            secondNodeValues.push_back(*it);
    }
    // Clear originalNodeIndex and newNodeIndex
    removeRecord(recordNumber);
    removeRecord(newNodeIdx);

    changeRecordStatus(recordNumber, nodeStatus(recordNumber));
    fillNodeWithValues(firstNodeValues, recordNumber);

    changeRecordStatus(newNodeIdx, nodeStatus(recordNumber));
    fillNodeWithValues(secondNodeValues, newNodeIdx);

    return newNodeIdx;
}
// remove record
void Btree::removeRecord(int recordNumber) {
    for (int i = 1; i <= descendants * 2; ++i) {
        int offset = recordNumber * recordSize() + i * getCharSize();
        file.seekg(offset, std::ios::beg);
        file << toString(-1);
    }
}
// spilt if size overflow
bool Btree::split(vector<pair<int, int>> root) {
    // Find 2 empty nodes for the new nodes
    int firstNodeIndex = nextEmpty();
    if (firstNodeIndex == -1) return false;

    // Get next empty node in available list
    int secondNodeIndex = readValue(firstNodeIndex, 1);
    if (secondNodeIndex == -1) return false;

    // Update the next empty cell with the next in available list
    writeCell(readValue(secondNodeIndex, 1), 0, 1);

    vector<pair<int, int>> firstNodeValues, secondNodeValues;

    // Fill first and second nodes from root
    auto middle=  (root.begin() + (int) (root.size()) / 2);
    for (auto it = root.begin(); it != root.end(); ++it) {
        if (distance(it, middle) > 0)
            firstNodeValues.push_back(*it);
        else
            secondNodeValues.push_back(*it);
    }

    changeRecordStatus(firstNodeIndex, nodeStatus(1));
    fillNodeWithValues(firstNodeValues, firstNodeIndex);

    changeRecordStatus(secondNodeIndex, nodeStatus(1));
    fillNodeWithValues(secondNodeValues, secondNodeIndex);

    removeRecord(1);

    // Create new root with max values from the 2 new nodes
    vector<std::pair<int, int>> newRoot;
    newRoot.emplace_back(firstNodeValues.back().first, firstNodeIndex);
    newRoot.emplace_back(secondNodeValues.back().first, secondNodeIndex);
    markIsNonLeaf(1);
    fillNodeWithValues(newRoot, 1);

    return true;
}
int Btree::readValue(int rowIdx, int columnIdx) {
    // Go to the specified cell
    file.seekg(rowIdx * recordSize() + columnIdx * getCharSize(), std::ios::beg);

    // Read and return the integer value in the cell
    char val[charSize];

    file.read(val, charSize);

    return stoi(string(val));
}
// write value
void Btree::writeCell(int value, int rowIndex, int columnIndex) {
    file.seekg(rowIndex * recordSize() + columnIndex * charSize, std::ios::beg);
    file << toString(value);
}
// read from record as pairs first is key and second is reference
pair<int, int> Btree::readPair(int recordNumber, int pairNumber) {
    if(isValidRecord(recordNumber) && isValidPair(pairNumber)){
        // read the pair value
        std::pair<int, int> p;
        p.first = readValue(recordNumber, 2 * pairNumber - 1);
        p.second = readValue(recordNumber, 2 * pairNumber);

        return p;

    }
}
//store all keys inside record
std::vector<std::pair<int, int>> Btree::readNodeValues(int recordNumber) {
    // Validate input
    if(isValidRecord(recordNumber)){

        // Go to the specified record on the b-tree file
        file.seekg(recordNumber * recordSize() + charSize, std::ios::beg);

        // Create and return the node
        vector<pair<int, int>> theNode;

        // Read every pair in the node
        for (int i = 1; i <= descendants; ++i) {

            std::pair<int, int> p ;

            p = readPair(recordNumber, i);

            // If it is empty then the rest is empty, so return
            if (p.second == -1) return theNode;

            // Otherwise, continue reading the node
            theNode.push_back(p);


        }

        return theNode;
    }
}
// mark node is non-leaf
void Btree::markIsNonLeaf(int recordNumber) {
    file.seekg(recordNumber * recordSize(), std::ios::beg);
    file << toString(1);
}
// Node status
int Btree::nodeStatus(int recordNumber) {
    return readValue(recordNumber, 0);
}
// record size or row size
int Btree::recordSize() const {
    return charSize + 2 * descendants * charSize;
}
// to string
string Btree::toString(int value) const {
    std::stringstream result;

    // Convert the integer value into a string
    std::string stringValue = std::to_string(value);

    // Write the string
    result << stringValue;

    // Write spaces until the final result's size becomes the cell size
    for (int i = 0; i < charSize - stringValue.size(); ++i) result << ' ';
    return result.str();
}
int Btree::nextEmpty() {
    return readValue(0, 1);
}
int Btree::getCharSize() const{
    return this->charSize;
}
bool Btree::isValidRecord(int recordNumber) const {
    // If the record number is not between 1 and numberOfRecords
    if (recordNumber <= 0 || recordNumber > numberOfRecords)
        return false;

    return true;
}
// is valid pair?
bool Btree::isValidPair(int pairNumber) const {
    // If the pair number is not between 1 and descendants
    if (pairNumber <= 0 || pairNumber > descendants)
        return false;

    return true;
}
// is leaf?
bool Btree::isLeaf(int recordNumber) {
    return readValue(recordNumber, 0) == 0;
}
// is empty?
bool Btree::isEmpty(int recordNumber) {
    return readValue(recordNumber, 0) == -1;
}

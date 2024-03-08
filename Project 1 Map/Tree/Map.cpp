#include "Map.h"
using namespace std;

Map::Map() : root(nullptr), n(0){}

Map::~Map() { deleteList(root); }
bool Map::empty() const { return n == 0; }
int Map::size() const { return n; }
void Map::dump() { print(root); cerr << endl; }
bool Map::contains(const KeyType& key) const {     return nullptr != find(root, key); }

void Map::clearLeaf(Node* target) {
    if (target == root) {
        delete target;
        root = nullptr;
    }
    else {
        Node* parent = target->parent;
        if (parent->left == target) { parent->left = nullptr; }
        else { parent->right = nullptr; }
        delete target;
    }
}
// Used only when the target node to delete has no children
// If it's root, delete target and set nullptr to avoid memory access errors
// Else get the parent of the node and see if the target node is of the right or left
// If it's the right of the parent then set the right to nullptr to avoid memory access issues
// Else set the left of the parent to nullptr
// Delete target node

void Map::deleteNodeSingle(Node* target) {
    Node* parent = target->parent;
    if (target->left == nullptr && target->right == nullptr) { clearLeaf(target); }
    else if (parent->left == target) {
        if (target->right != nullptr) {
            parent->left = target->right;
            target->right->parent = target->parent;
        }
        else {
            parent->left = target->left;
            target->left->parent = target->parent;
        }
    }
    else {
        if (target->right != nullptr) {
            parent->right = target->right;
            target->right->parent = target->parent;
        }
        else {
            parent->right = target->left;
            target->left->parent = target->parent;
        }
    }
    
}
// Called when deleting a node with one child or when deleting the replacement of a node with two children
// If no children then call the function that takes care of that case
// else Find the parent and see if the target is of the left or right
// For both, see if the target has one right node or one left node
// Depending on both, rewire the target parent and target child

bool Map::insert(const KeyType& key, const ValueType& value) {
    if (contains(key)) { return false; }
    n += 1;
    if (n == 1) {
        root = new Node(key, value);
        return true;
    }
    
    else {
        
        Node* current = root;
        while (current != nullptr) {
            if (current->key > key) {
                
                if (current->left == nullptr) {
                    Node* newNode = new Node(key, value);
                    current->left = newNode;
                    newNode->parent = current;
                    return true;
                }
                current = current->left;
            }
            else {
                if (current->right == nullptr) {
                    
                    Node* newNode = new Node(key, value);
                    current->right = newNode;
                    newNode->parent = current;
                    return true;
                }
                current = current->right;
            }
        }
        
    }
    return true;
}
// Check if key is already in and return false if so
// Otherwise check if the map is empty and set the added node to root
// If not then use while loops and comparisons to the key and other node keys to find the proper position to add the new node

bool Map::update(const KeyType& key, const ValueType& value) {
    Node* toUpdate = find(root, key);
    if (toUpdate != nullptr) {
        toUpdate->value = value;
        return true;
    }
    return false;
}
// Finds adress of Node with key and updates to the given value if it exists

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
    if (contains(key)) {return update(key, value); }
    return insert(key, value);
}
// Calls insert function or update function depending if the key is in the list or not

bool Map::erase(const KeyType& key) {
    Node* target = find(root, key);
    if (target == nullptr) { return false; }
    n -= 1;
    if (target->left == nullptr && target->right == nullptr) { clearLeaf(target); }
    
    else if (target->left != nullptr && target->right != nullptr) {
        Node* replacement = target->right;
        while (replacement->left != nullptr) { replacement = replacement->left; }
        target->key = replacement->key;
        target->value = replacement->value;
        deleteNodeSingle(replacement);
    }
    else {
        if (target == root) {
            if (target->left != nullptr) {
                root = target->left;
                target->left->parent = nullptr;
            }
            else {
                root = target->right;
                target->right->parent = nullptr;
            }
        }
        else { deleteNodeSingle(target); }
        delete target;
    }
    return true;
}
// If the target key is not in the map then return false
// Otherwise check children status
// If 0 children, call the function to rewire and delete it
// Else if 2 children, get a replacement node from the right subtree, copy contents to target, and call the function that deletes rewires with 1 child to target the replacement
// Else 1 child
//        If root, find the parent and see if it is of right or left to replace as nullptr
//        Else call the function that rewires nodes with 1 child


void Map::recCopy(Node* current) {
    if (current == nullptr) { return; }
    insert(current->key, current->value);
    recCopy(current->left);
    recCopy(current->right);
}
// Pre-order traversal to insert node contents to another

Map::Map(const Map& other) {
    n = 0;
    root = nullptr;
    recCopy(other.root);
}
// Sets counter to 0 which will later increase with recursive copy function

void Map::operator=(const Map& other) {
    deleteList(root);
    n = 0;
    root = nullptr;
    recCopy(other.root);
}
// Clear list, set counter to 0, recursive copy

void Map::deleteList(Node* current) {
    if (current == nullptr) { return; }
    deleteList(current->left);
    deleteList(current->right);
    delete current;
}
// Post-order traversal to delete nodes

void Map::swap(Map& other) {
    Node* temp = root;
    int number = n;
    root = other.root;
    n = other.n;
    other.root = temp;
    other.n = number;
}
// Set aside temporary variables (root and size counter) to hold stuff of m1
// Set m1 stuff to m2
// Set m2 stuff to temp variables


bool Map::get(const KeyType& key, ValueType& value) const {
    Node* target = find(root, key);
    if (target == nullptr) { return false; }
    value = target->value;
    return true;
}
// Get's address of node with key and if not nullptr, save the value

int Map::getByIndex(Node* current, int i, int target, KeyType& key, ValueType& value) const {
    if (current == nullptr) { return i + 1; }
    i = getByIndex(current->left, i, target, key, value);
    
    if (i == target + 1) { // cout << current->data;
        key = current->key;
        value = current->value;
        return size();
    }
    if (i >= size()) {return size();}
    return getByIndex(current->right, i, target, key, value);
}
// Gonna be honest, not sure how it works exactly; I was playing around with pre-order traversal to get nodes by index and it worked
// If nullptr increments index by 1 I think
// If index = target index - 1, copy contents and return
// Return whatever number though on XCODE and other test trees I made, I had to return size() or it would not work properly



bool Map::get(int i, KeyType& key, ValueType& value) const {
    if (i > -1 && i < n) { getByIndex(root, 0, i, key, value); }
    return true;
}
// If i between 0 and size counter then call get by index function


bool combine(const Map& m1, const Map& m2, Map& result) {
    Map temp = m1;
    bool noDuplicate = true;
    for (int i = 0; i < m2.size(); i++) {
        KeyType key;
        ValueType value, val;
        m2.get(i, key, value);
        if (!temp.insert(key, value)) {
            temp.get(key, val);
            if (val != value) {
                temp.erase(key);
                noDuplicate = false;
            }
        }
    }
    
    for (int j = 0; j < result.size(); j++) {
        KeyType key;
        ValueType value;
        result.get(0, key, value);
        result.erase(key);
    }
    
    result = temp;
    return noDuplicate;
}

void Map::print(Node* current) {
    if (current == nullptr) { return; }
    print(current->left);
    cerr << current->key << " " << current->value << endl;
    print(current->right);
}
// Init new temp map and set to m1
// Create bool duplichecker counter
// Cycle through m2 and see what nodes can be added
// If repeat detected that is of different value, erase the key value pair from temp map
// Clear the result map and set it to temp

void subtract(const Map& m1, const Map& m2, Map& result) {
    Map temp = m1;
    for (int i = 0; i < temp.size(); i++) {
        KeyType key;
        ValueType value;
        temp.get(i, key, value);
        if (m2.contains(key)) { temp.erase(key); }
    }
    for (int j = 0; j < result.size(); j++) {
        KeyType key;
        ValueType value;
        result.get(0, key, value);
        result.erase(key);
    }
    result = temp;
}
// Init temp map equal to m1
// Cycle through temp nodes and see if any keys are in m2 to delete
// Clear result and set it to temp map

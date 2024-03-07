#pragma once
#include <iostream>

using namespace std;
using KeyType = std::string;
using ValueType = double;

class Map {
    private:
        struct Node {
            Node *right, *left, *parent;
            KeyType key;
            ValueType value;

            Node(KeyType key, ValueType value) {
                this->key = key;
                this->value = value;
                right = left = parent = nullptr;
            }
        };

        Node* root;
        int n;
        void print(Node* current);
        void deleteList(Node* current);
        void recCopy(Node* current);
        int getByIndex(Node* current, int i, int target, KeyType& key, ValueType& value) const;
        void clearLeaf(Node* target);
        void deleteNodeSingle(Node* target);

        Node* addToTree(Node* current, KeyType key, ValueType value) const {
            if (current == nullptr) { return new Node(key, value); }
            else if (current->key > key) { 
                
                current->left = addToTree(current->left, key, value); }
            else { 
                
                current->right = addToTree(current->right, key, value); }
            return current;
        }

        Node* find(Node* current, const KeyType key) const {
            if (current == nullptr) { return nullptr; }
            if (current->key == key) { return current; }
            else if ( current->key > key) { return find(current->left, key); }
            else { return find(current->right, key); }
        }


    public:
        Map();              // Create an empty map (i.e., one with no key/value pairs)

        ~Map();

        Map(const Map& other);

        void operator=(const Map& other);

        bool empty() const; // Return true if the map is empty, otherwise false.

        int size() const;   // Return the number of key/value pairs in the map.

        bool insert(const KeyType& key, const ValueType& value);
        // If key is not equal to any key currently in the map, and if the
        // key/value pair can be added to the map, then do so and return true.
        // Otherwise, make no change to the map and return false (indicating
        // that either the key is already in the map).

        bool update(const KeyType& key, const ValueType& value);
        // If key is equal to a key currently in the map, then make that key no
        // longer map to the value it currently maps to, but instead map to
        // the value of the second parameter; return true in this case.
        // Otherwise, make no change to the map and return false.

        bool insertOrUpdate(const KeyType& key, const ValueType& value);
        // If key is equal to a key currently in the map, then make that key no
        // longer map to the value it currently maps to, but instead map to
        // the value of the second parameter; return true in this case.
        // If key is not equal to any key currently in the map then add it and 
        // return true. In fact this function always returns true.

        bool erase(const KeyType& key);
        // If key is equal to a key currently in the map, remove the key/value
        // pair with that key from the map and return true.  Otherwise, make
        // no change to the map and return false.

        bool contains(const KeyType& key) const;
        // Return true if key is equal to a key currently in the map, otherwise
        // false.

        bool get(const KeyType& key, ValueType& value) const;
        // If key is equal to a key currently in the map, set value to the
        // value in the map that that key maps to, and return true.  Otherwise,
        // make no change to the value parameter of this function and return
        // false.

        bool get(int i, KeyType& key, ValueType& value) const;
        // If 0 <= i < size(), copy into the key and value parameters the
        // key and value of one of the key/value pairs in the map and return
        // true.  Otherwise, leave the key and value parameters unchanged and
        // return false.  (See below for details about this function.)

        void swap(Map& other);
        // Exchange the contents of this map with the other one.

        void dump();
};

bool combine(const Map& m1, const Map& m2, Map& result);

void subtract(const Map& m1, const Map& m2, Map& result);

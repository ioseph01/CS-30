#include "Map.h"
// #include <iostream>
/*Map() : head(nullptr), tail(nullptr), n(0) {}*/
Map::Map() { 
	head = tail = nullptr;
	n = 0;
}
Map::~Map() { deleteList(); }
bool Map::empty() const { return n == 0; }
int Map::size() const { return n; }
bool Map::contains(const KeyType& key) const { return (find(key) != nullptr); }
// Uses a find function and determines if it's a nullptr or not

void Map::deleteList() {
	Node* current = head;
	while (current != nullptr) {
		Node* toDelete = current;
		current = current->next;
		delete toDelete;
	}
}
// Uses a navigator node to cycle through the list and delete nodes

bool Map::insert(const KeyType& key, const ValueType& value) {
	if (contains(key)) { return false; }
	n += 1;
	Node* newNode = new Node(key, value, head);
	if (head == nullptr) { head = tail = newNode; }
	else if (head->key > key) {
		head->prev = newNode;
		head = newNode;
		if (n == 2) { tail = newNode->next; }
	}
	else {
		for (Node* current = head; current->next != nullptr; current = current->next) {
			if (current->next->key > key) { 
				newNode->next = current->next;
				newNode->prev = current;
				current->next = newNode;
				newNode->next->prev = newNode;
				return true;
			}
		}
		tail->next = newNode;
		newNode->prev = tail;
		newNode->next = nullptr;
		tail = newNode;
	}
	return true;
}
// If key already in, returns false
// Add to node counter
// Else if empty then init new node as head and tail
// Else if the key goes before the head node, init new node and rewire it and the old head to make the new node the new head
// Else if it goes somewhere in the middle, cycle through list until proper spot is found and rewire the prev and the navigator node to the new init node
// Else the key goes last, set rewire tail to the new init node and set the new node as tail

bool Map::update(const KeyType& key, const ValueType& value) {
	Node* toUpdate = find(key);
	if (toUpdate != nullptr) {
		toUpdate->value = value;
		return true;
	}
	return false;
}
// If key is in the list, use a temporary nav node to cycle through until the proper key is found
// Once found, change the value 

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value) {
	if (contains(key)) { return update(key, value); }
	return insert(key, value);
}
// Calls the insert function or the update function after checking the list for the key

bool Map::erase(const KeyType& key) {
	Node* toDelete = find(key);
	if (toDelete != nullptr) {
		n -= 1;
		if (toDelete == head) { head = toDelete->next; }
		else if (toDelete == tail) { 
			tail = toDelete->prev;
			tail->next = nullptr;
		}
		else {
			toDelete->prev->next = toDelete->next; 
			toDelete->next->prev = toDelete->prev;
		}
		delete toDelete;
		return true;
	}
	return false;
}
// If the key is in the list check cases
// If target node is head, set next node to head and DONT set connection to target as nullptr 
// Else if target is tail, set prev node to tail and set connection to target as nullptr
// (set connection to target as nullptr avoids weird crashes and memory access for the tail but not for the head)
// Else if it has a prev or next node, rewire the neighbors
// Delete the node and subtract from the node counter

bool Map::get(const KeyType& key, ValueType& value) const {
	Node* current = find(key);
	if (current == nullptr) { return false; }
	value = current->value;
	return true;
}
// Call private find function which cycles through the list searching through a particular key
// The find function returns the address of the node containing the key which can be checked as an actual node or nullptr
// Saves the value of the found node

bool Map::get(int i, KeyType& key, ValueType& value) const {
	if (0 <= i && i < size()) {
		Node* current = head;
		for (int j = 0; j < i; j++) { current = current->next; }
		key = current->key;
		value = current->value;
		return true;
	}
	return false;
}
// If i between 0 and number of nodes, cycle through list to index
// Save found key and value

Map::Map(const Map& other) {
	n = other.size();
	for (Node* current = other.head; current != nullptr; current = current->next) {
		Node* newNode = new Node(current->key, current->value, nullptr);
		if (current == other.head) { head = tail = newNode; }
		else {
			tail->next = newNode;
			newNode->prev = tail;
			tail = newNode;
		}
	}
}	
// Set node counter to other node counter
// Cycle throug nodes of other map and init new nodes of same keys and values
// 1st Case : if adding first node, set it to head and tail
// 2nd Case : if not first node, rewire tail and the new node

void Map::operator=(const Map& other) {
	Node* current = head;
	while (current != nullptr) {
		Node* toDelete = current;
		current = current->next;
		delete toDelete;
	}

	n = other.size();
	for (Node* current = other.head; current != nullptr; current = current->next) {
		Node* newNode = new Node(current->key, current->value, nullptr);
		if (current == other.head) { head = tail = newNode; }
		else {
			tail->next = newNode;
			newNode->prev = tail;
			tail = newNode;
		}
	}
}
// Cycles through the map and clears it of its nodes
// Set node counter to other node counter
// Cycle throug nodes of other map and init new nodes of same keys and values
// 1st Case : if adding first node, set it to head and tail
// 2nd Case : if not first node, rewire tail and the new node


// void Map::dump() const { cout << endl << "SIZE : " << size() << endl;  for (Node* current = head; current != nullptr; current = current->next) { cout << "Key: " << current->key << ", Value: " << current->value << endl; } }

void Map::swap(Map& other) {
	Node* temp = head;
	head = other.head;
	other.head = temp;
	int num = n;
	n = other.size();
	other.n = num;
}
// Set aside one map's head and node counter
// Set the same map's to the other's head and node counter
// Set the other's head and node counter to the saved data

bool combine(const Map& m1, const Map& m2, Map& result) {
	Map temp = m1;
	bool noDuplicate = true;
	for (int i = 0; i < m2.size(); i++) {
		KeyType key;
		ValueType value, val;
		m2.get(i, key, value);
		if (temp.insert(key, value) == false) {
			temp.get(key, val);
			if (value != val) {
				temp.erase(key);
				noDuplicate = false;
			}
		}
	}

	for (int i = 0; i < result.size(); i++) {
		KeyType key;
		ValueType value;
		result.get(0, key, value);
		result.erase(key);
	}
	result = temp;

	return noDuplicate;
}
// To work if m1 and result refer to same map, create new temp map
// Create bool duplicate counter for return value
// Use copy constructor on the temp map and m1
// Cycle through m2 list
//		Try to insert key value pairs from m2 into temp map
//			If fail : if the key value pair is same then pass
//					Otherwise erase the key value pair in temp and set duplicate counter to return false
// Clear given result map and use copy constructor with temp


void subtract(const Map& m1, const Map& m2, Map& result) {

	Map temp = m1;
	for (int i = 0; i < temp.size(); i++) {
		KeyType key;
		ValueType value;
		temp.get(i, key, value);
		if (m2.contains(key)) { temp.erase(key); }
	}

	for (int i = 0; i < result.size(); i++) {
		KeyType key;
		ValueType value;
		result.get(0, key, value);
		result.erase(key);
	}
	result = temp;
}
// Make new temp map copying m1, just in case m1 and result are the same
// Cycle through temp map and see if keys match with any in m2
// If match, delete the key value pair in temp
// Clear result map and set it to temp map

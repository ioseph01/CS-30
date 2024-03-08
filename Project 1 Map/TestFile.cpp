#include "Map.h"
#include <string>
#include <iostream>
#include <cassert>
#include <type_traits>
#define CHECKTYPE(f, t) { auto p = (t)(f); (void)p; }
using namespace std;


static_assert(std::is_default_constructible<Map>::value,
              "Map must be default-constructible.");
static_assert(std::is_copy_constructible<Map>::value,
              "Map must be copy-constructible.");

void ThisFunctionWillNeverBeCalled()
{
    CHECKTYPE(&Map::operator=, Map & (Map::*)(const Map&));
    CHECKTYPE(&Map::empty, bool (Map::*)() const);
    CHECKTYPE(&Map::size, int  (Map::*)() const);
    CHECKTYPE(&Map::insert, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::update, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::insertOrUpdate, bool (Map::*)(const KeyType&, const ValueType&));
    CHECKTYPE(&Map::erase, bool (Map::*)(const KeyType&));
    CHECKTYPE(&Map::contains, bool (Map::*)(const KeyType&) const);
    CHECKTYPE(&Map::get, bool (Map::*)(const KeyType&, ValueType&) const);
    CHECKTYPE(&Map::get, bool (Map::*)(int, KeyType&, ValueType&) const);
    CHECKTYPE(&Map::swap, void (Map::*)(Map&));
    
    CHECKTYPE(combine, bool (*)(const Map&, const Map&, Map&));
    CHECKTYPE(subtract, void (*)(const Map&, const Map&, Map&));
}

void test()
{
    Map m, m2;
    m.insert("B", 5);
    m.insert("F", 3);
    m.insert("T", 13);
    m2.insert("A", 5);
    m2.insert("T", 3);
    subtract(m, m2, m);
    assert(m.size() == 2 && m.contains("B") && m.contains("F") && !m.contains("A") && !m.contains("T"));
}

void dump2(Map& m) {
    for (int i = 0; i < m.size(); i++) {
        KeyType key;
        ValueType val;
        m.get(i, key, val);
        cout << i << ") " << key << endl;
    } cout << endl;
}

int main()
{
    {
        try {
            Map m;
            m.insert("A", 10);
            m.insert("B", 44);
            m.insert("C", 10);
            string all;
            double total = 0;
            for (int n = 0; n < m.size(); n++)
            {
                string k;
                double v;
                m.get(n, k, v);
                all += k;
                total += v;
            }
            cout << all << total << endl;
        }
        catch(...) {
            cout << "Failed Default Test 1" << endl;
            exit(1);
        }
        cout << "Passed Default Test 1" << endl;
    } // Basic insertion and getByIndex function
    
    {
        try {
            Map gpas;
            gpas.insert("Fred", 2.956);
            gpas.insert("Ethel", 3.538);
            double v;
            string k1;
            gpas.get(1, k1, v);
            assert(k1 == "Fred");
            assert(gpas.get(1, k1, v) && (k1 == "Fred" || k1 == "Ethel"));
            string k2;
            assert(gpas.get(1, k2, v) && k2 == k1);
        }
        catch(...) {
            cout << "Failed Default Test 2" << endl;
            exit(1);
        }
        cout << "Passed Default Test 2" << endl;
    } // Basic insertion and get function
    
    {
        try {
            Map gpas2;
            gpas2.insert("Fred", 2.956);
            assert(!gpas2.contains(""));
            gpas2.insert("Ethel", 3.538);
            gpas2.insert("", 4.000);
            gpas2.insert("Lucy", 2.956);
            assert(gpas2.contains(""));
            gpas2.erase("Fred");
            assert(gpas2.size() == 3 && gpas2.contains("Lucy") && gpas2.contains("Ethel") &&
            gpas2.contains(""));
        }
        catch(...) {
            cout << "Failed Default Test 3" << endl;
            exit(1);
        }
        cout << "Passed Default Test 3" << endl;
    } // Basic insertion, "" input, and size checking
    
    {
        try {
            Map m1;
            m1.insert("Fred", 2.956);
            Map m2;
            m2.insert("Ethel", 3.538);
            m2.insert("Lucy", 2.956);
            m1.swap(m2);
            assert(m1.size() == 2 && m1.contains("Ethel") && m1.contains("Lucy") &&
                   m2.size() == 1 && m2.contains("Fred"));
            // Default Test 4
        }
        catch(...) {
            cout << "Failed Default Test 4" << endl;
            exit(1);
        }
        cout << "Passed Default Test 4" << endl;
    } // Basic swap
    
    {
        try {
            Map mm1, mm2, mm3, mm4;
            mm1.insert("Fred", 123);
            mm1.insert("Ethel", 456);
            mm1.insert("Lucy", 789);
            mm2.insert("Lucy", 789);
            mm2.insert("Ricky", 321);
            cout << "Running TEST 5" << endl;
            assert(combine(mm1, mm2, mm3) && mm3.contains("Fred") && mm3.contains("Ricky") && mm3.contains("Lucy") && mm3.contains("Ethel") && mm3.size() == 4);
            // Combine function with copy constructor and same key/value pairs
            mm2.insertOrUpdate("Lucy", 654);
            assert(!combine(mm1, mm2, mm3) && mm3.contains("Fred") && mm3.contains("Ricky") && mm3.contains("Ethel") && mm3.size() == 3);
            // Combine function with assignment operator and different values
            
             assert(!combine(mm1, mm2, mm1) && mm1.contains("Fred") && mm1.contains("Ricky") && mm1.contains("Ethel") && mm1.size() == 3);
            // Combine function with same map as result map and different values
            
            cout << "Running TEST 6" << endl;
            mm4.insert("Fred", 123);
            mm4.insert("Ethel", 456);
            mm4.insert("Lucy", 789);
            mm2.insert("Ethel", 654);
            subtract(mm4, mm2, mm3);
            assert(mm3.size() == 1 && mm3.contains("Fred"));
            // Subtract Function
            subtract(mm2, mm1, mm2);
            assert(mm2.size() == 1 && mm2.contains("Lucy"));
            // Subtract function with same map as result map
            
            cout << "Running TEST 7" << endl;
            assert(mm3.erase("yo") == false);
            assert(mm3.erase("Fred") && mm3.size() == 0);
            mm3.insert("Tom", 123);
            mm3.insert("Ford", 456);
            assert(mm3.size() == 2 && mm3.contains("Tom") && mm3.contains("Ford"));
            // Deleting contents until empty and adding contents
        }
        catch(...) {
            cout << "Failed Default Test 5 or 6 or 7 " << endl;
            exit(1);
        }
        cout << "Passed Default Test 5, 6, 7" << endl;
    }
    
    try {
        Map m;
        m.insert("A", 3);
        m.insertOrUpdate("A", 5);
        assert(m.size() == 1 && m.contains("A"));
        m.erase("A");
        assert(m.size() == 0);
        m.insertOrUpdate("B", 4);
        KeyType key = "B";
        ValueType val;
        m.get(key, val);
        assert(m.contains("B") && m.size() == 1 && key == "B" && val == 4);
        m.get("C", val);
        assert(val == 4);
        cout << "Passed Default Test 5, 6, 7" << endl;
    }
    catch(...) {
        cout << "Failed Default Test 5 or 6 or 7 " << endl;
        exit(1);
    }// Insert, Update, Erase, and Delete functions
    
    {
        Map m;
        m.insert("C", 10);
        m.insert("B", 44);
        m.insert("A", 10);
        m.dump();
        
        Map m2;
        m2.insert("A", 10);
        m2.insert("B", 44);
        m2.insert("C", 10);
        m2.dump();
        
        Map m3;
        m3.insert("B", 10);
        m3.insert("C", 44);
        m3.insert("A", 10);
        m3.dump();
    } // Order testing
    
    {
        Map m, m2;
        m.insert("J", 10);
        m.insert("D", 440);
        m.insert("F", 100);
        m.insert("L", 210);
        m.insert("M", 424);
        m.insert("P", 102);
        m.insert("Z", 310);
        m.insert("B", 434);
        m.insert("A", 103);
        m.insert("N", 104);
        m.insert("T", 444);
        m.insert("F", 410);
        m.insert("L", 210);
        m.insert("C", 210);
        m.insert("G", 210);
        m.insert("I", 210);
        KeyType key, key2;
        ValueType val;
        dump2(m);
        m.get(0, key, val);
        m.get(m.size() - 1, key2, val);
        assert(key == "A" && m.size() == 14 && key2 == "Z");
        m.erase("A"); // Leaf deletion
        dump2(m);
        m.erase("F"); // 1 Child deletion
        dump2(m);
        m.erase("D");
        dump2(m); // 2 child deletion
        m.erase("P");
        dump2(m); // 2 child deletion
        m.erase("J");
        dump2(m); // 2 child Root deletion
        
        m2.insert("J", 3230);
        m2.insert("K", 12);
        m2.insert("Z", 23);
        m2.erase("J");
        dump2(m2); // 1 child  Root deletion
        
    } // GetByIndex, Duplication insert, Ordering, and Deletion testing
    
    
    test();
    cout << "Passed all tests" << endl;
}



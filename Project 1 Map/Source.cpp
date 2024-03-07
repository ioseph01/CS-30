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

int main()
{
    {
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
        // Default Test 1
    }

    {
        Map gpas;
        gpas.insert("Fred", 2.956);
        gpas.insert("Ethel", 3.538);
        double v;
        string k1;
        assert(gpas.get(1, k1, v) && (k1 == "Fred" || k1 == "Ethel"));
        string k2;
        assert(gpas.get(1, k2, v) && k2 == k1);
        // Default Test 2

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
        // Default Test 3
    }

    {
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

    {
        Map mm1, mm2, mm3;
        mm1.insert("Fred", 123);
        mm1.insert("Ethel", 456);
        mm1.insert("Lucy", 789);
        mm2.insert("Lucy", 789);
        mm2.insert("Ricky", 321);
        combine(mm1, mm2, mm3);
        assert(mm3.contains("Fred") && mm3.contains("Ricky") && mm3.contains("Lucy") && mm3.contains("Ethel") && mm3.size() == 4);
        mm2.insertOrUpdate("Lucy", 654);
        combine(mm1, mm2, mm3);
        assert(mm3.contains("Fred") && mm3.contains("Ricky") && mm3.contains("Ethel") && mm3.size() == 3);
        // Default Test 6

        mm2.insert("Ethel", 654);
        subtract(mm1, mm2, mm3);
        assert(mm3.size() == 1 && mm3.contains("Fred"));
        //Default Test 7

        assert(mm3.erase("yo") == false);
        assert(mm3.erase("Fred") && mm3.size() == 0);
        mm3.insert("Tom", 123);
        mm3.insert("Ford", 456);
        assert(mm3.size() == 2 && mm3.contains("Tom") && mm3.contains("Ford"));
        // Deleting contents until empty and adding contents
    }

    {
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
    } // Adding one then deleting one and get function

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
        Map m;
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
        KeyType key;
        ValueType val;
        m.get(0, key, val);
        assert(key == "A" && m.size() == 11);
        m.erase("F");
        m.dump();

    } // Duplication insert, Ordering, and Deletion testing


    test();
    cout << "Passed all tests" << endl;
}
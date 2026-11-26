#include "../solution.hpp"
#include <cassert>
#include <deque>
#include <random>
#include <string>

struct Tracked {
    static inline int alive = 0;
    int value;
    explicit Tracked(int v) : value(v) { ++alive; }
    Tracked(const Tracked& other) : value(other.value) { ++alive; }
    ~Tracked() { --alive; }
};

int main() {
    DoublyLinkedList<int> list;
    std::deque<int> reference;
    std::mt19937 random(2640);
    for (int step = 0; step < 100000; ++step) {
        int operation = random() % 6;
        int value = random() % 100000;
        if (operation == 0) {
            list.push_front(value);
            reference.push_front(value);
        } else if (operation == 1) {
            list.push_back(value);
            reference.push_back(value);
        } else {
            auto result = operation % 2 ? list.pop_front() : list.pop_back();
            assert(result.has_value() == !reference.empty());
            if (!reference.empty()) {
                assert(*result == (operation % 2 ? reference.front() : reference.back()));
                if (operation % 2) reference.pop_front();
                else reference.pop_back();
            }
        }
        assert(list.size() == reference.size());
        assert(list.empty() == reference.empty());
        auto it = list.begin();
        for (int expected : reference) {
            assert(it != list.end());
            auto old = it++;
            assert(*old == expected);
        }
        assert(it == list.end());
    }
    DoublyLinkedList<std::string> words;
    words.push_back("first");
    *words.begin() = "changed";
    assert(*words.pop_back() == "changed");
    assert(!words.pop_front());
    words.push_front("again");
    assert(*words.pop_front() == "again");
    assert(!words.pop_back());

    {
        DoublyLinkedList<Tracked>::iterator survivor(nullptr);
        {
            DoublyLinkedList<Tracked> objects;
            for (int i = 0; i < 1000; ++i) objects.push_back(Tracked(i));
            survivor = objects.begin();
            objects.pop_front();
            objects.pop_back();
            assert((*survivor).value == 0);
        }
        assert(Tracked::alive > 0);
    }
    assert(Tracked::alive == 0);
    std::cout << "All list tests passed\n";
}

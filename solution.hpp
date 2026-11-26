#include <iostream>
#include <memory>
#include <optional>

template <typename T>
class DoublyLinkedList {
private:
    // Node structure definition
    struct Node {
        T value;
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;

        Node(const T& val) : value(val), next(nullptr) {}
    };

    std::shared_ptr<Node> head;
    std::weak_ptr<Node> tail;
    size_t length;

public:
    // Iterator class
    class iterator {
    private:
        std::shared_ptr<Node> current;

    public:
        iterator(std::shared_ptr<Node> node) : current(node) {}

        // Dereference operator to get value
        T& operator*() {
            // 注：对空迭代器调用此函数是未定义行为，测试数据中不会出现
            return current->value;
        }

        // Post-increment operator
        iterator operator++(int) {
            // 注：对空迭代器调用此函数是未定义行为，测试数据中不会出现
            iterator previous = *this;
            current = current->next;
            return previous;
        }

        // Equality operators
        bool operator==(const iterator& other) const {
            // 注：比较属于不同容器的非空迭代器是未定义行为，测试数据中不会出现
            return current == other.current;
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }
    };

    // Constructor
    DoublyLinkedList() : head(nullptr), length(0) {}

    // No destructor needed, memory managed by shared_ptr

    // Check if list is empty
    bool empty() const {
        return length == 0;
    }

    // Get the size of the list
    size_t size() const {
        return length;
    }

    // Add element to the front
    void push_front(const T& value) {
        auto node = std::make_shared<Node>(value);
        node->next = head;
        if (head) {
            head->prev = node;
        } else {
            tail = node;
        }
        head = node;
        ++length;
    }

    // Add element to the back
    void push_back(const T& value) {
        auto node = std::make_shared<Node>(value);
        auto last = tail.lock();
        node->prev = last;
        if (last) {
            last->next = node;
        } else {
            head = node;
        }
        tail = node;
        ++length;
    }

    // Remove and return the first element
    std::optional<T> pop_front() {
        if (!head) {
            return std::nullopt;
        }
        std::optional<T> value(head->value);
        head = head->next;
        if (head) {
            head->prev.reset();
        } else {
            tail.reset();
        }
        --length;
        return value;
    }

    // Remove and return the last element
    std::optional<T> pop_back() {
        if (empty()) {
            return std::nullopt;
        }
        auto last = tail.lock();
        std::optional<T> value(last->value);
        auto previous = last->prev.lock();
        if (previous) {
            previous->next.reset();
        } else {
            head.reset();
        }
        tail = previous;
        --length;
        return value;
    }

    // Iterators
    iterator begin() {
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }
};

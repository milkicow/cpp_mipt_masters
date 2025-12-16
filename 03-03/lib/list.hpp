#ifndef LIST_HPP
#define LIST_HPP

#include <iostream>

namespace list {

class List {
   private:
    struct Node {
        int value = 0;
        Node* next = nullptr;
    };

    Node* m_head = nullptr;
    Node* m_tail = nullptr;

   public:
    List() = default;
    List(const List&) = delete;
    List& operator=(const List&) = delete;

    List(List&& rhs) : m_head(rhs.m_head), m_tail(rhs.m_tail) {
        rhs.m_head = nullptr;
        rhs.m_tail = nullptr;
    }

    List& operator=(List&& rhs) {
        if (this != &rhs) {
            clear();
            m_head = std::exchange(rhs.m_head, nullptr);
            m_tail = std::exchange(rhs.m_tail, nullptr);
        }
        return *this;
    }

    ~List() { clear(); }

    void clear() noexcept {
        while (m_head != nullptr) {
            Node* temp = m_head;
            m_head = m_head->next;
            delete temp;
        }
        m_tail = nullptr;
    }

    [[nodiscard]] bool empty() const noexcept { return m_head == nullptr; }

    void show() const noexcept {
        Node* current = m_head;
        while (current != nullptr) {
            std::cout << current->value << ' ';
            current = current->next;
        }
        std::cout << '\n';
    }

    void push_front(int value) {
        Node* newNode = new Node{value};

        if (empty()) {
            m_head = m_tail = newNode;
        } else {
            newNode->next = m_head;
            m_head = newNode;
        }
    }

    void push_back(int value) {
        Node* newNode = new Node{value};

        if (empty()) {
            m_head = m_tail = newNode;
        } else {
            m_tail->next = newNode;
            m_tail = newNode;
        }
    }

    void pop_front() {
        if (empty()) {
            return;
        }

        Node* temp = m_head;
        m_head = m_head->next;

        if (m_head == nullptr) {
            m_tail = nullptr;
        }

        delete temp;
    }

    void pop_back() {
        if (empty()) {
            return;
        }

        if (m_head == m_tail) {
            delete m_head;
            m_head = m_tail = nullptr;
            return;
        }

        Node* current = m_head;
        while (current->next != m_tail) {
            current = current->next;
        }

        delete m_tail;
        m_tail = current;
        m_tail->next = nullptr;
    }

    int get() const {
        if (empty()) {
            throw std::runtime_error("Cannot get middle element from empty list");
        }

        Node* slow = m_head;
        Node* fast = m_head;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;
        }
        return slow->value;
    }
};

}  // namespace list
#endif  // LIST_HPP
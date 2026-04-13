#include <iterator>
#include <memory>

template <typename T>
class List {
   private:
    struct Node {
        T x = T();
        std::shared_ptr<Node> next;
        std::weak_ptr<Node> prev;
    };

   public:
    class Iterator {
       public:
        using iterator_category = std::bidirectional_iterator_tag;
        Iterator(std::shared_ptr<Node> node = nullptr) : m_node(node) {}

        auto const operator++(int) {
            auto x = *this;
            m_node = m_node->next;
            return x;
        }

        auto& operator++() {
            m_node = m_node->next;
            return *this;
        }

        auto const operator--(int) {
            auto x = *this;
            m_node = m_node->prev.lock();
            return x;
        }

        auto& operator--() {
            m_node = m_node->prev.lock();
            return *this;
        }

        auto& operator*() const { return m_node->x; }
        auto operator->() const { return &m_node->x; }

        friend bool operator==(Iterator const& lhs, Iterator const& rhs) {
            return lhs.m_node == rhs.m_node;
        }

       private:
        std::shared_ptr<Node> m_node;
    };

    auto begin() const { return Iterator(m_head); }
    auto end() const { return Iterator(); }

    void push_back(T x) {
        auto node = std::make_shared<Node>();
        node->x = x;

        if (m_head) {
            node->prev = m_tail;
            m_tail->next = node;
            m_tail = node;
        } else {
            m_head = node;
            m_tail = node;
        }
    }

   private:
    std::shared_ptr<Node> m_head;
    std::shared_ptr<Node> m_tail;
};

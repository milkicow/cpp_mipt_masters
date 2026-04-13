#include <iostream>
#include <memory>
#include <queue>

class Tree {
   public:
    struct Node {
        int value;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;
        std::weak_ptr<Node> parent;

        Node(int val) : value(val) { std::cout << "Node ctor: " << value << std::endl; }

        ~Node() { std::cout << "Node dtor: " << value << std::endl; }
    };

    std::shared_ptr<Node> root;

    // BFS
    void traverse_v1() const {
        if (!root) return;

        std::queue<std::shared_ptr<Node>> q;
        q.push(root);

        while (!q.empty()) {
            auto current = q.front();
            q.pop();

            std::cout << current->value << " ";

            if (current->left) q.push(current->left);
            if (current->right) q.push(current->right);
        }
        std::cout << std::endl;
    }

    // DFS
    void traverse_v2() const {
        dfs(root);
        std::cout << std::endl;
    }

   private:
    void dfs(std::shared_ptr<Node> node) const {
        if (!node) return;

        std::cout << node->value << " ";
        dfs(node->left);
        dfs(node->right);
    }
};

int main() {
    Tree tree;

    tree.root = std::make_shared<Tree::Node>(1);

    auto n2 = std::make_shared<Tree::Node>(2);
    auto n3 = std::make_shared<Tree::Node>(3);

    auto n4 = std::make_shared<Tree::Node>(4);
    auto n5 = std::make_shared<Tree::Node>(5);
    auto n6 = std::make_shared<Tree::Node>(6);
    auto n7 = std::make_shared<Tree::Node>(7);

    tree.root->left = n2;
    tree.root->right = n3;

    n2->parent = tree.root;
    n3->parent = tree.root;

    n2->left = n4;
    n2->right = n5;
    n3->left = n6;
    n3->right = n7;

    n4->parent = n2;
    n5->parent = n2;
    n6->parent = n3;
    n7->parent = n3;

    std::cout << "BFS: ";
    tree.traverse_v1();

    std::cout << "DFS: ";
    tree.traverse_v2();

    return 0;
}

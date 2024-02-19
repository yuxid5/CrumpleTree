#ifndef __PROJ_FOUR_CRUMPLE_TREE_HPP
#define __PROJ_FOUR_CRUMPLE_TREE_HPP

#include <cstddef>
#include <stdexcept>
#include <string>
#include <vector>

namespace shindler::ics46::project4 {

class ElementNotFoundException : public std::runtime_error {
   public:
    explicit ElementNotFoundException(const std::string &err)
        : std::runtime_error(err) {}
};

template <typename K, typename V>
class CrumpleTree {
   private:
    // fill in private member data here
    // If you need to declare private functions, do so here too.
        struct Node{
            Node(K k, V v)
            : key{k}, value{v}, parent{nullptr}, right{nullptr}, left{nullptr}, level{0}, shape_l{1}, shape_r{1}
            {}
            K key;
            V value;
            Node * parent;
            Node * right;
            Node * left;
            unsigned level;
            size_t shape_l;
            size_t shape_r;
        };
        Node* root;
        size_t node_size;
        void insert_r(Node*& current, Node* insert){
            //insert the element
            if (current == nullptr){
                current = insert;
                current-> level = 1;
                node_size++;
                return;
            }
            if (insert->key < current -> key){
                insert_r(current->left, insert);
                current -> left -> parent = current;
            }
            else{
                insert_r(current->right, insert);
                current -> right -> parent = current;
            }
            //update level and shape
            if ((current-> right!= nullptr && current->right->level == current->level) || 
            (current-> left!= nullptr && current->left->level == current->level)){
                current->level++;
            }
            if (current->right != nullptr){
                current->shape_r = current->level - current->right->level;
                if (current->left == nullptr){
                    current->shape_l = current->level - 0;
                }
            }
            if (current->left != nullptr){
                current->shape_l = current->level - current->left->level;
                if (current->right == nullptr){
                    current->shape_r = current->level - 0;
                }
            }
            //rebalance
            if (current->shape_l > 2){
                current->shape_l -= 2;
                current->level -= 2;
                //记录当前的右边（稍后这个节点会变成当前的值）
                Node* swap_node = current->right;
                //现在的右边的是原来右侧的左边
                current->right = current->right->left;
                //当前变为当前右侧的子节点
                swap_node -> left = current;
                swap_node -> parent = current->parent;
                current->parent = swap_node;
                current = swap_node;
                //更新当前替换的shape
                current->shape_l = current->level - current->left->level;
            }
            //另一个方向
            else if (current->shape_r > 2){
                current->shape_r -= 2;
                current->level -= 2;
                Node* swap_node = current->left;
                current->left = current->left->right;
                swap_node->right = current;
                swap_node -> parent = current->parent;
                current->parent = swap_node;
                current = swap_node;
                current->shape_r = current->level - current->right->level;
            }
            
        }

   public:
    CrumpleTree();

    // In general, a copy constructor and assignment operator
    // are good things to have.
    // For this quarter, I am not requiring these.
    //	CrumpleTree(const CrumpleTree & st);
    //	CrumpleTree & operator=(const CrumpleTree & st);
    // You do not need to implement these
    CrumpleTree(const CrumpleTree &) = delete;
    CrumpleTree(CrumpleTree &&) = delete;
    CrumpleTree &operator=(const CrumpleTree &) = delete;
    CrumpleTree &operator=(CrumpleTree &&) = delete;

    // The destructor is required.
    ~CrumpleTree();

    // size() returns the number of distinct keys in the tree.
    [[nodiscard]] size_t size() const noexcept;

    // isEmpty() returns true if and only if the tree has no values in it.
    [[nodiscard]] bool empty() const noexcept;

    // contains() returns true if and only if there
    //  is a (key, value) pair in the tree
    //	that has the given key as its key.
    bool contains(const K &key) const noexcept;

    // returns the level on which the node with this key resides.
    // If !contains(k), this will throw an ElementNotFoundException
    unsigned level(const K &key) const;

    // find returns the value associated with the given key
    // If !contains(k), this will throw an ElementNotFoundException
    // There needs to be a version for const and non-const CrumpleTrees.
    V &find(const K &key);
    const V &find(const K &key) const;

    // Inserts the given key-value pair into
    // the tree and performs the balancing operation(s)
    // if needed as described in lecture.
    // If the key already exists in the tree,
    // you may do as you please (no test cases in
    // the grading script will deal with this situation)
    void insert(const K &key, const V &value);

    // Deletes the given key from the tree
    // and performs the balancing operation(s) if needed.
    // If the key does not exist in the tree,
    // do not modify the tree.
    void remove(const K &key);

    // The following three functions all return
    // the set of keys in the tree as a standard vector.
    // Each returns them in a different order.
    [[nodiscard]] std::vector<K> inOrder() const;
    [[nodiscard]] std::vector<K> preOrder() const;
    [[nodiscard]] std::vector<K> postOrder() const;
};

template <typename K, typename V>
CrumpleTree<K, V>::CrumpleTree() 
    :root{nullptr}, node_size{0}
{
    // TODO: Implement this
}

template <typename K, typename V>
CrumpleTree<K, V>::~CrumpleTree() {
    // TODO: Implement this
}

template <typename K, typename V>
size_t CrumpleTree<K, V>::size() const noexcept {
    // TODO: Implement this
    return node_size;
}

template <typename K, typename V>
bool CrumpleTree<K, V>::empty() const noexcept {
    // TODO: Implement this
    return root == nullptr;
}

template <typename K, typename V>
bool CrumpleTree<K, V>::contains(const K &key) const noexcept {
    // TODO: Implement this
    Node *temp = root;
    while (temp != nullptr){
        if (key == temp -> key){
            return true;
        }
        else if (key < temp -> key){
            temp = temp->left;
        }
        else{
            temp = temp->right;
        }
    }
    return false;
}

template <typename K, typename V>
unsigned CrumpleTree<K, V>::level(const K &key) const {
    // TODO: Implement this
    if (contains(key)){
        Node * temp = root;
        while (temp != nullptr){
            if (key == temp -> key){
                return temp->level;
            }
            else if (key < temp -> key){
                temp = temp->left;
            }
            else{
                temp = temp->right;
            }
        }
    }
    else{
        throw ElementNotFoundException("Not found");
    }
}

template <typename K, typename V>
V &CrumpleTree<K, V>::find(const K &key) {
    // TODO: Implement this
    if (contains(key)){
        Node * temp = root;
        while (temp != nullptr){
            if (key == temp->key){
                return temp->value;
            }
            else if (key < temp -> key){
                temp = temp -> left;
            }
            else{
                temp = temp->right;
            }
        }
    }
    else{
        throw ElementNotFoundException("Not found");
    }
}

template <typename K, typename V>
const V &CrumpleTree<K, V>::find(const K &key) const {
    // TODO: Implement this
    if (contains(key)){
        Node * temp = root;
        while (temp != nullptr){
            if (key == temp->key){
                return temp->value;
            }
            else if (key < temp -> key){
                temp = temp -> left;
            }
            else{
                temp = temp->right;
            }
        }
    }
    else{
        throw ElementNotFoundException("Not found");
    }
}

template <typename K, typename V>
void CrumpleTree<K, V>::insert(const K &key, const V &value) {
    // TODO: Implement this
    if (contains(key)){
        return;
    }
    Node * newNode = new Node(key, value);
    insert_r(root, newNode);
}

template <typename K, typename V>
void CrumpleTree<K, V>::remove(const K &key) {
    // TODO: Implement this
    int placeholder = key;
}

template <typename K, typename V>
std::vector<K> CrumpleTree<K, V>::inOrder() const {
    // TODO: Implement this
    return {};
}

template <typename K, typename V>
std::vector<K> CrumpleTree<K, V>::preOrder() const {
    // TODO: Implement this
    return {};
}

template <typename K, typename V>
std::vector<K> CrumpleTree<K, V>::postOrder() const {
    // TODO: Implement this
    return {};
}

}  // namespace shindler::ics46::project4

#endif

#ifndef __PROJ_FOUR_CRUMPLE_TREE_HPP
#define __PROJ_FOUR_CRUMPLE_TREE_HPP

#include <array>
#include <cstddef>
#include <stdexcept>
#include <string>
#include <type_traits>
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
            //case 3
            if (current->shape_l > 2 && current->right->shape_l == 2){
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
            else if (current->shape_r > 2 && current->left->shape_r == 2){
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
            //case4
            if (current->shape_l > 2 && current->right->shape_r == 1){
                //层数左shape保持不变
                current->shape_l -= 1;
                current->level -= 1;
                //记录当前的右边
                Node* swap_node = current->right;
                //现在的右边是原来右侧的左边
                current->right = current->right->left;
                //将右侧上升
                swap_node->level++;
                swap_node->left = current;
                swap_node -> parent = current->parent;
                current->parent = swap_node;
                current = swap_node;
                //更新当前的shape_l和shape_r
                current->shape_l = current->level - current->left->level;
                current->shape_r = current->level - current->right->level;
            }
            //case4另一个方向
            else if (current->shape_r > 2 && current->left->shape_l == 1){
                current->shape_r--;
                current->level--;
                Node* swap_node = current->left;
                current->left = current->left->right;
                swap_node->level++;
                swap_node->right = current;
                swap_node->parent = current->parent;
                current->parent = swap_node;
                current = swap_node;
                current->shape_l = current->level - current->left->level;
                current->shape_r = current->level - current->right->level;
            }
            //case5
            if (current->shape_l > 2 && current->right->shape_r == 2){
                //双下降
                current->shape_l -= 2;
                current -> level -= 2;
                current->right->shape_r--;
                current->right->level--;
                //记录当前的右侧和右侧的左边
                Node* swap_node = current->right->left;
                Node* current_right = current->right;
                //更新当前的右边，和当前右边的左边
                current->right = swap_node->left;
                current_right->left = swap_node->right;
                //上升替换的node,连接节点
                swap_node->level++;
                swap_node->left = current;
                swap_node->right = current_right;
                swap_node->parent = current->parent;
                current_right->parent = swap_node;
                current->parent = swap_node;
                current = swap_node;
                //更改shape
                current->shape_l = current->level - current->left->level;
                current->shape_r = current->level - current->right->level;
            }
            else if (current->shape_r > 2 && current->left->shape_l == 2){
                current->shape_r -= 2;
                current -> level -= 2;
                current->left->shape_l--;
                current->left->level--;
                Node* swap_node = current->left->right;
                Node* current_left = current->left;
                current->left = swap_node->right;
                current_left->right = swap_node->left;
                swap_node->level++;
                swap_node->right = current;
                swap_node->left = current_left;
                swap_node->parent = current->parent;
                current_left->parent = swap_node;
                current->parent = swap_node;
                current = swap_node;
                current->shape_l = current->level - current->left->level;
                current->shape_r = current->level - current->right->level;
            }
        }
        void remove_helper(Node*& current, const K &key){
            if (key == current->key){
                //符合条件删除
                if (current->left == nullptr && current->right == nullptr){
                    delete current;
                    current = nullptr;
                    node_size--;
                    return;
                }
                //找successor
                if (current->right != nullptr){
                    Node* succssor = current -> right;
                    while(succssor->left != nullptr){
                        succssor = succssor->left;
                    }
                    if (succssor->left == nullptr && succssor->right == nullptr){
                        std::swap(current->key, succssor->key);
                        std::swap(current->value, succssor->value);
                        remove_helper(current->right, key);
                    }
                    else if (succssor->left == nullptr && succssor->right != nullptr){
                        std::swap(current->key, succssor->key);
                        std::swap(current->value, succssor->value);
                        std::swap(succssor->key, succssor->right->key);
                        std::swap(succssor->value, succssor->right->value);
                        remove_helper(current->right, key);
                    }
                    else{
                        std::swap(current->key, succssor->key);
                        std::swap(current->value, succssor->value);
                        std::swap(succssor->key, succssor->left->key);
                        std::swap(succssor->value, succssor->left->value);
                        remove_helper(current->right, key);
                    }
                }
                else{
                    std::swap(current->key, current->left->key);
                    std::swap(current->value, current->left->value);
                    remove_helper(current->left, key);
                }
            }
            //如果key小于当前目标
            else if (key < current->key && current->left != nullptr){
                remove_helper(current->left, key);
            }
            else{
                remove_helper(current->right, key);
            }
            //update level and shape after delete
            if (current-> right == nullptr && current->left == nullptr && current->level > 1){
                current->level--;
                current->shape_l = current->level - 0;
                current->shape_r = current->level - 0;
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
            //case 2
            if ((current->shape_l == 3 && current->shape_r == 2) || (current->shape_r == 3 && current->shape_l == 2)){
                current->level--;
                current->shape_l--;
                current -> shape_r--;
            }
            // else if (current->shape_r == 3 && current->shape_l == 2){
            //     current->level--;
            //     current->shape_l--;
            //     current -> shape_r--;
            // }

            //case 3
            if (current->shape_l == 3 && current->shape_r == 1 && current->right->shape_l == 1 && current->right->shape_r == 1){
                //现在的下降
                current->level--;
                current->shape_l--;
                Node* current_right = current->right;
                //现在的右边是原来右边的左边
                current->right = current_right->left;
                //更新parent
                current_right->parent = current->parent;
                current->parent = current_right;
                //更新原来右边的左边
                current_right->left = current;
                //替换current
                current = current_right;
                //往上升,更新shape
                current->level ++;
                current->shape_l = current->level - current->left->level;
                current->shape_r = current->level - current->right->level;
            }
            else if(current->shape_r == 3 && current->shape_l == 1 && current->left->shape_r == 1 && current->left->shape_l == 1){
                current->level--;
                current->shape_r--;
                Node* current_left = current->left;
                current->left = current_left->right;
                current_left->parent = current->parent;
                current->parent = current_left;
                current_left->right = current;
                current = current_left;
                current->level ++;
                current->shape_l = current->level - current->left->level;
                current->shape_r = current->level - current->right->level;
            }
            //case_4A
            if(current->shape_l == 3 && current->shape_r == 1 && current->left != nullptr && current->right->shape_l == 2 
                && current->right->left != nullptr && current->right->shape_r == 1){
                //现在的下降
                current->level--;
                current->shape_l--;
                Node* current_right = current->right;
                //现在的右边是原来的右边的左边,并更新shape
                current->right = current->right->left;
                current->shape_r = current->level - current->right->level;
                //更新parent
                current_right->parent = current->parent;
                current->parent = current_right;
                //更新原来右边的左边
                current_right->left = current;
                //替换current
                current = current_right;
                //往上升,更新shape
                current->level ++;
                current->shape_l = current->level - current->left->level;
                current->shape_r = current->level - current->right->level;
            }
            else if(current->shape_r == 3 && current->shape_l == 1 && current->right != nullptr && current->left->shape_r == 2 
                && current->left->right != nullptr && current->left->shape_l == 1){
                current->level--;
                current->shape_r--;
                Node* current_left = current->left;
                current->left = current->left->right;
                current->shape_l = current->level - current->left->level;
                current->left->parent = current->parent;
                current->parent = current_left;
                current_left->right = current;
                current = current_left;
                current->level ++;
                current->shape_l = current->level - current->left->level;
                current->shape_r = current->level - current->right->level;
            }
            //case4b
            if(current->shape_l == 3 && current->shape_r == 1 && current->left == nullptr && current->right->shape_l == 2 
                && current->right->left == nullptr && current->right->shape_r == 1){
                //下降两格
                current -> level -= 2;
                current->shape_l -= 2;
                //记录原来的right
                Node* current_right = current->right;
                //现在的右边是原来的右边的左边也就是nullptr
                current->right = current->right->left;
                //更新parent
                current_right->parent = current->parent;
                current->parent = current_right;
                //更新原来右边的左边
                current_right->left = current;
                //替换current
                current = current_right;
                //更新shape
                current->shape_l = current->level - current->left->level;
                current->shape_r = current->level - current->right->level;
            }
            else if(current->shape_r == 3 && current->shape_l == 1 && current->right == nullptr && current->left->shape_r == 2 
                && current->left->right == nullptr && current->left->shape_l == 1){
                current->level -= 2;
                current->shape_r -= 2;
                Node* current_left = current->left;
                current->left = current->left->right;
                current_left->parent = current->parent;
                current->parent = current_left;
                current_left -> right = current;
                current = current_left;
                current->shape_l = current->level - current->left->level;
                current->shape_r = current->level - current->right->level;
            }
            //case5
            if (current->shape_l == 3 && current->shape_r == 1 && current->right->shape_l == 1 && current->right->shape_r == 2){
                current->level -= 2;
                current->shape_l -= 2;
                current->right->level -= 1;
                current->right->shape_r -= 1;
                Node* newTarget = current->right->left;
                Node* current_right = current->right;
                current->right = newTarget->left;
                current_right -> left = newTarget->right;
                newTarget->level += 2;
                newTarget->left = current;
                newTarget->right = current_right;
                newTarget->parent = current->parent;
                current_right->parent = newTarget;
                current->parent = newTarget;
                current = newTarget;
                current->shape_l = current->level - current->left->level;
                current->shape_r = current->level - current->right->level;
            }
            else if(current->shape_r == 3 && current->shape_l == 1 && current->left->shape_r == 1 && current->left->shape_l == 2){
                current->level -= 2;
                current->shape_r -= 2;
                current->left->level -= 1;
                current->left->shape_l -= 1;
                Node* newTarget = current->left->right;
                Node* current_left = current->left;
                current->left = newTarget->right;
                current_left->right = newTarget->left;
                newTarget->level += 2;
                newTarget->right = current;
                newTarget->left = current_left;
                newTarget->parent = current->parent;
                current_left->parent = newTarget;
                current->parent = newTarget;
                current = newTarget;
                current->shape_l = current->level - current->left->level;
                current->shape_r = current->level - current->right->level;
            }
            //case6
            if (current->shape_l == 3 && current->shape_r == 1 && current->right->shape_l == 2 && current->right->shape_r == 2){
                current->level--;
                current->shape_l--;
                current->right->level--;
                current->right->shape_l--;
                current->right->shape_r--;

            }
            else if (current->shape_r == 3 && current->shape_l == 1 && current->left->shape_r == 2 && current->left->shape_l == 2){
                current->level--;
                current->shape_r--;
                current->left->level--;
                current->left->shape_l--;
                current->left->shape_r--;
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
        if (key < temp -> key){
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
            if (key < temp -> key){
                temp = temp->left;
            }
            else{
                temp = temp->right;
            }
        }
    }
    throw ElementNotFoundException("Not found");
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
            if (key < temp -> key){
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
            if (key < temp -> key){
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
    if (!contains(key)){
        return;
    }
    remove_helper(root, key);
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

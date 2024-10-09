#pragma once

#include <functional>
#include <memory>
#include "ANSI_colors.hpp"

namespace avl_tree {

template <typename KeyT, typename CompT = std::less<KeyT>> class avl_tree_t final {

    struct avl_node;
    using  unique_avl_node = std::unique_ptr<avl_node>;

    enum class node_type_e {
        NOT_USED  = 1 << 0,
        USED      = 1 << 1,
        NOT_EXIST = 1 << 2,
        EXIST     = 1 << 3,
    };
    enum class node_setting_type_e {
        ADD,
        DELETE,
    };
    struct avl_node final {
        KeyT key_{};
        int node_type_ = static_cast<int>(node_type_e::NOT_USED) |
                         static_cast<int>(node_type_e::EXIST);
        int height_ = 0;
        int Nleft_  = 0;
        int Nright_ = 0;
        avl_node*       parent_ = nullptr;
        unique_avl_node left_;
        unique_avl_node right_;

        avl_node(const KeyT& key) : key_(key) {}

        avl_node(const avl_node& node) {
            if (node.is_valid()) {
                key_    = node.key_;
                height_ = node.height_;
                Nleft_  = node.Nleft_;
                Nright_ = node.Nright_;
                parent_ = node.parent_;
                node_type_ = node.node_type_;
                if (node.left_)  left_  = std::make_unique<avl_node>(*node.left_);
                if (node.right_) right_ = std::make_unique<avl_node>(*node.right_);
            }
        }

        avl_node(const avl_node* node) {
            if (node && node->is_valid())
                *this = avl_node{*node};
        }

        avl_node(const unique_avl_node& node) {
            if (node)
                *this = avl_node{node.get()};
        }

        bool is_valid() const { return ((node_type_ & static_cast<int>(node_type_e::EXIST)) > 0); }

        ~avl_node() {
            key_.~KeyT();
            left_.release();
            right_.release();
        }
    };

    enum class avl_node_it_type_e {
        ASCENDING
    };
    class avl_node_it final {
        avl_node* node_          = nullptr;
        avl_node_it_type_e type_ = avl_node_it_type_e::ASCENDING;

    public:
        avl_node*          get_node() const { return node_; }
        avl_node_it_type_e get_type() const { return type_; }

        avl_node_it(const avl_node_it& node_it,
                    avl_node_it_type_e type = avl_node_it_type_e::ASCENDING) : 
                    node_(node_it.get_node()), type_(node_it.get_type()) {}
        
        avl_node_it(const avl_node& node,
                    avl_node_it_type_e type = avl_node_it_type_e::ASCENDING) :
                    node_(const_cast<avl_node*>(&node)), type_(type) {}
        
        avl_node_it(avl_node* node,
                    avl_node_it_type_e type = avl_node_it_type_e::ASCENDING) : node_(node), type_(type) {}

        avl_node_it(const unique_avl_node& node,
                    avl_node_it_type_e type = avl_node_it_type_e::ASCENDING) : node_(node.get()), type_(type) {}

        avl_node_it& operator=(const unique_avl_node& node) {
            if (node_ == node.get())
                return *this;

            *this = avl_node_it{node};
            return *this;
        }

        bool is_valid() const { return (node_ != nullptr); }

        avl_node* operator*()             { return (node_ ? node_ : nullptr); } // return ptr for iterating with ++it;
        const avl_node* operator*() const { return (node_ ? node_ : nullptr); }

        avl_node* operator->()             { return (node_ ? node_ : nullptr); }
        const avl_node* operator->() const { return (node_ ? node_ : nullptr); }

        bool operator==(const avl_node_it& rhs) const {
            return (rhs.node_ == node_);
        }

        bool operator!=(const avl_node_it& rhs) const {
            return (rhs.node_ != node_);
        }

        avl_node_it operator++() {
            if (is_valid() && node_->is_valid()) {
                switch (type_) {
                    case avl_node_it_type_e::ASCENDING:
                        node_ = node_->parent_;
                        break;
                }
            }
            return avl_node_it{nullptr, type_};
        }

        avl_node_it begin() const {
            avl_node_it current = node_;
            if (current.is_valid())
                while (current->left_)
                    current = current->left_;
            return current;
        }

        avl_node_it end() const {
            return avl_node_it{nullptr, type_};
        }
    };

    CompT comp_func;
    unique_avl_node root_;
    KeyT max_key_;

private:
    avl_node_it ascending_range(const avl_node_it node) const {
        return avl_node_it(node, avl_node_it_type_e::ASCENDING);
    }

    int get_node_size(const avl_node_it node) const noexcept {
        if (!node.is_valid())
            return 0;

        return node->Nleft_ + node->Nright_ + 1;
    }

    void update_Nchilds(avl_node_it node) noexcept {
        for (auto node_iter : ascending_range(node)) {
            node_iter->Nleft_  = get_node_size(node_iter->left_);
            node_iter->Nright_ = get_node_size(node_iter->right_);
        }
    }

    void update_height(avl_node_it node) noexcept {
        for (auto node_iter : ascending_range(node)) {
            node_iter->height_ = 0;
            if (node_iter->left_)
                node_iter->height_ = node_iter->left_->height_;
            if (node_iter->right_)
                node_iter->height_ = std::max(node_iter->height_, node_iter->right_->height_);

            node_iter->height_++;
        }
    }

    avl_node_it balance(avl_node_it node) {
        if (!node.is_valid())
            return node;

        int balance_diff = 0;
        if (node->left_)
            balance_diff = node->left_->height_;
        if (node->right_)
            balance_diff -= node->right_->height_;

        int left_height = 0;
        int right_height = 0;
        if (balance_diff > 1) {
            avl_node_it left = *node->left_;
            if (left->left_)
                left_height = left->left_->height_;

            if (left->right_)
                right_height = left->right_->height_;

            if (left_height < right_height)
                return rotate_left(node->left_);
            return rotate_right(node);
                
        } else if (balance_diff < -1) {
            avl_node_it right = *node->right_;
            if (right->left_)
                left_height = right->left_->height_;

            if (right->right_)
                right_height = right->right_->height_;

            if (left_height > right_height)
                return rotate_right(node->right_);
            return rotate_left(node);
        }
        return node;
    }

    avl_node_it rotate_right(avl_node_it node) {
        if (!node.is_valid())
            return node;

        unique_avl_node* destination;
        if (node == root_) {
            destination = &root_;
        } else {
            if (comp_func(node->left_->key_, node->parent_->key_))
                destination = &node->parent_->left_;
            else
                destination = &node->parent_->right_;
        }
        avl_node* old_node_parent = node->parent_;

        //--------------------------------
        unique_avl_node node_lr_old = nullptr;
        if (node->left_->right_)
            node_lr_old = std::make_unique<avl_node>(node->left_->right_);
        unique_avl_node node_l_old = std::make_unique<avl_node>(node->left_);

        node_l_old->right_.reset(node.get_node());
        node_l_old->right_->left_.reset(node_lr_old.get());
        //--------------------------------

        *destination = std::make_unique<avl_node>(node_l_old);
        (*destination)->left_->parent_  = destination->get();
        (*destination)->right_->parent_ = destination->get();
        (*destination)->parent_         = old_node_parent;
        if ((*destination)->right_->left_)
            (*destination)->right_->left_->parent_ = (*destination)->right_.get();


        update_height ((*destination)->right_);
        update_Nchilds((*destination)->right_);
        return *destination;
    }

    avl_node_it rotate_left(avl_node_it node) {
        return node;
        // if (!node.is_valid())
        //     return node;

        // unique_avl_node node_right_old = std::make_unique<avl_node>(new (avl_node)(*node->right_));

        // if (node->right_->left_)
        //     node->right_ = std::make_unique<avl_node>(*node->right_->left_);
        // else
        //     node->right_ = std::make_unique<avl_node>(nullptr);
        // node_right_old->left_ = std::make_unique<avl_node>(new (avl_node)(*node));

        // if (node == root_) {
        //     root_.reset(node_right_old.get());
        // } else {
        //     if (comp_func(node->key_, node->parent_->key_))
        //         node->parent_->left_  = std::make_unique<avl_node>(*node_right_old);
        //     else
        //         node->parent_->right_ = std::make_unique<avl_node>(*node_right_old);
        // }

        // node_right_old->parent_ = node->parent_;
        // if (node->right_)
        //     node->right_->parent_ = *node;
        // node->parent_ = node_right_old.get();
        
        // update_height (node);
        // update_Nchilds(node);
        // return node_right_old;
    }

    avl_node_it get_parent_bigger(const avl_node_it node, const KeyT& key) const {
        avl_node_it ans_node = node;
        KeyT         ans_key = max_key_;
        for (auto node_iter : ascending_range(node)) {
            if (comp_func(key, node_iter->key_) && node_iter->key_ != key &&
                comp_func(node_iter->key_, ans_key)) {

                ans_node = node_iter;
                ans_key  = node_iter->key_;
            }
        }
        return ans_node;
    }

    avl_node_it lower_bound(const KeyT& key) const {
        if (!root_)
            return nullptr;

        avl_node_it current = *root_;
        while (current.is_valid()) {
            if (comp_func(key, current->key_)) {
                if (current->left_)
                    current = current->left_;
                else
                    return get_parent_bigger(current, key);
            } else if (comp_func(current->key_, key)) {
                if (current->right_)
                    current = current->right_;
                else
                    return get_parent_bigger(current, key);
            } else {
                return current;
            }
        }
        return current;
    }

    avl_node_it upper_bound(const KeyT& key) const {
        if (!root_)
            return nullptr;

        avl_node_it current = *root_;
        while (current.is_valid()) {
            if (comp_func(key, current->key_)) {
                if (current->left_)
                    current = current->left_;
                else
                    return get_parent_bigger(current, key);
            } else {
                if (current->right_)
                    current = current->right_;
                else
                    return get_parent_bigger(current, key);
            }
        }
        return current;
    }

    int size_left_side(const KeyT& key) const {
        if (!root_)
            return 0;

        int dist = 0;
        avl_node_it current = *root_;
        while (current.is_valid()) {
            if (comp_func(key, current->key_)) {
                if (current->left_)
                    current = current->left_;
                else
                    break;
            } else if (comp_func(current->key_, key)) {
                dist += current->Nleft_ + 1;
                if (current->right_)
                    current = current->right_;
                else
                    break;
            } else {
                return dist + current->Nleft_;
            }
        }
        return dist;
    }

    void print_node(const avl_node_it node) const noexcept {
        if (!node.is_valid() || !node->is_valid())
            return;

        std::cerr << print_lcyan(node->key_ << "\t(");

        if (node->left_)
            std::cerr << print_lcyan(node->left_->key_ << ",\t");
        else
            std::cerr << print_lcyan("none" << ",\t");

        if (node->right_)
            std::cerr << print_lcyan(node->right_->key_ << ",\t");
        else
            std::cerr << print_lcyan("none" << ",\t");

        if (node->parent_)
            std::cerr << print_lcyan(node->parent_->key_ << ",\t");
        else
            std::cerr << print_lcyan("none" << ",\t");

        std::cerr << print_lcyan(node->Nleft_  << ",\t" << node->Nright_ << ",\t" <<
                                 node->height_ << ",\t" << node.get_node() << ")\n");
    }

    void print_subtree(avl_node_it node) noexcept {
        if (!node.is_valid() || !node->is_valid())
            return;

        avl_node_it current = node;
        while (current.is_valid()) {

            avl_node_it left_it = current->left_;
            if (left_it.is_valid() &&
                (current->left_->node_type_ & static_cast<int>(node_type_e::USED)) == 0) {
                print_subtree(current->left_);
                continue;
            }

            if (current->node_type_ & static_cast<int>(node_type_e::NOT_USED))
                print_node(current);
            current->node_type_ &= !static_cast<int>(node_type_e::NOT_USED);
            current->node_type_ |=  static_cast<int>(node_type_e::USED);

            avl_node_it right_it = current->right_;
            if (right_it.is_valid() &&
                (current->right_->node_type_ & static_cast<int>(node_type_e::USED)) == 0) {
                print_subtree(current->right_);
                continue;
            }

            current = current->parent_;
        }
    }

    void decrement_parents_size(avl_node_it node) noexcept {
        for (avl_node_it node_iter = node; node_iter->parent_ != nullptr; node_iter = node_iter->parent_) {
            if (node_iter->parent_->left_.get() == node_iter.get_node())
                node_iter->parent_->Nleft_--;
            else
                node_iter->parent_->Nright_--;
        }
    }

    avl_node_it find(const KeyT& key) const {
        if (!root_)
            return 0;

        avl_node_it current = *root_;
        while (current.is_valid()) {
            if (comp_func(key, current->key_)) {
                if (current->left_)
                    current = current->left_;
                else
                    return *current;
            } else if (comp_func(current->key_, key)) {
                if (current->right_)
                    current = current->right_;
                else
                    return *current;
            } else {
                return *current;
            }
        }
        return *root_;
    }

    void set_type(const node_type_e& type, const node_setting_type_e* setting) noexcept {
        if (!root_)
            return;

        avl_node_it current = *root_;
        while (current.is_valid()) {
            if (current->left_) {
                current = current->left_;
            } else if (current->right_) {
                current = current->right_;
            } else {
                if (setting == node_setting_type_e::ADD)
                    current->node_->node_type_ |= static_cast<int>(type);
                if (setting == node_setting_type_e::DELETE)
                    current->node_->node_type_ -= static_cast<int>(type);

                if (current->parent_)
                    current = current->parent_;
                else
                    break;
            }
        }
    }

public:
    avl_tree_t(const KeyT& max_key) : max_key_(max_key) {}

    avl_tree_t(const avl_tree_t<KeyT, CompT>& other) {
        avl_node_it curr_other = other.root_;
        if (!curr_other.is_valid())
            return;

        root_ = std::make_unique<avl_node>((avl_node){other.root_->key_});
        avl_node_it curr_this = root_;

        while (curr_other.is_valid()) {

            if (curr_other->left_ && !curr_this->left_) {
                curr_other       = curr_other->left_;
                KeyT new_key = curr_other->key_;
                curr_this->left_ = std::make_unique<avl_node>((avl_node){new_key});
                curr_this->left_->parent_ = curr_this.get_node();
                curr_this        = curr_this->left_;

            } else if (curr_other->right_ && !curr_this->right_) {
                curr_other        = curr_other->right_;
                KeyT new_key = curr_other->key_;
                curr_this->right_ = std::make_unique<avl_node>((avl_node){new_key});
                curr_this->right_->parent_ = curr_this.get_node();
                curr_this         = curr_this->right_;

            } else {
                if (curr_other->parent_) {
                    update_height (curr_this);
                    update_Nchilds(curr_this);
                    curr_other = curr_other->parent_;
                    curr_this  = curr_this->parent_;
                } else {
                    break;
                }
            }
        }
    }

    avl_tree_t<KeyT, CompT>& operator=(const avl_tree_t<KeyT, CompT>& other) {
        if (this == &other)
            return *this;

        avl_tree_t<KeyT, CompT> new_tree{other};
        std::swap(root_, new_tree.root_);
        return *this;
    }

    avl_tree_t(avl_tree_t<KeyT, CompT>&& other) noexcept : comp_func(other.comp_func), root_(other.root_), 
                                                           max_key_(other.max_key_) {
        other.root_ = nullptr;
    }
    
    avl_tree_t& operator=(avl_tree_t<KeyT, CompT>&& other) noexcept {
        if (this == &other)
            return *this;

        std::swap(comp_func, other.comp_func);
        std::swap(root_,     other.root_);
        std::swap(max_key_,  other.max_key_);
        return *this;
    }

    void print() const noexcept {
        if (!root_)
            return;

        avl_tree_t<KeyT, CompT> print_tree{*this};

        std::cerr << print_lblue("AVL Tree with root = " << print_tree.root_->key_ <<
                                 ":\nkey(<child>, <child>, <parent>, <Nleft>, <Nright>, <height>, <ptr>):\n");

        print_tree.print_subtree(print_tree.root_);
        std::cerr << '\n';
    }

    avl_node_it insert(const KeyT& key) {
        if (!root_) {
            root_ = std::make_unique<avl_node>((avl_node){key});
            return root_;
        }

        avl_node_it current     = *root_;
        avl_node_it destination = *root_;
        while (current.is_valid()) {
            if (comp_func(key, current->key_)) {
                if (current->left_) {
                    current = current->left_;
                } else {
                    current->left_ = std::make_unique<avl_node>((avl_node){key});
                    current->left_->parent_ = current.get_node();
                    destination = current->left_;
                    break;
                }
            } else if (comp_func(current->key_, key)) {
                if (current->right_) {
                    current = current->right_;
                } else {
                    current->right_ = std::make_unique<avl_node>((avl_node){key});
                    current->right_->parent_ = current.get_node();
                    destination = current->right_;
                    break;
                }
            } else {
                return current;
            }
        }

        update_height (destination);
        update_Nchilds(destination);

        avl_node_it begin = ascending_range(destination).begin();
        avl_node_it end   = ascending_range(destination).end();
        for (avl_node_it node_iter = begin; node_iter != end; ++node_iter)
            node_iter = balance(node_iter).get_node();

        return find(destination->key_);
    }

    int check_range(const KeyT& first_key, const KeyT& second_key) const {
        if (first_key >= second_key || !root_)
            return 0;

        avl_node_it left  = lower_bound(first_key);
        avl_node_it right = upper_bound(second_key);

        int border = -1;
        if (first_key <= left->key_)
            border++;
        if (second_key >= right->key_)
            border++;

        return size_left_side(right->key_) - size_left_side(left->key_) + border;
    }

    ~avl_tree_t() {
        if (!root_)
            return;

        avl_node_it current = root_;
        while (current.is_valid()) {
            if (current->Nleft_ > 0) {
                int node_left_size = get_node_size(current->left_);
                if (node_left_size == 1) {
                    decrement_parents_size(current->left_);
                    std::cerr << "delete k: " << current->left_->key_ << "\n";
                    std::cerr << "delete p: " << current->left_.get() << "\n";
                    delete current->left_.get();
                } else if (node_left_size > 1) {
                    current = current->left_;
                }

            } else if (current->Nright_ > 0) {
                int node_right_size = get_node_size(current->right_);

                if (node_right_size == 1) {
                    decrement_parents_size(current->right_);
                    std::cerr << "delete k: " << current->right_->key_ << "\n";
                    std::cerr << "delete p: " << current->right_.get() << "\n";
                    delete current->right_.get();
                } else if (node_right_size > 1) {
                    current = current->right_;
                }

            } else {
                current = current->parent_;
            }
        }
        std::cerr << "delete root k: " << root_->key_ << "\n";
        std::cerr << "delete root p: " << root_.get() << "\n";
    }
};

};
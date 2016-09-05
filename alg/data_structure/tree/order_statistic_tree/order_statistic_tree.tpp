namespace alg {

template <class T>
OSTNode<T>::OSTNode(T element) : TreeNode<T>(element) {
    count_ = 1;
}

template <class T>
int OSTNode<T>::count() {
    return count_;
}

template <class T>
void OrderStatisticTree<T>::insert(T element) {
    OSTNode<T> *last = nullptr;
    OSTNode<T> *current = (OSTNode<T> *)this->root();
    OSTNode<T> *node = new OSTNode<T>(element);

    while (current) {
        last = current;
        last->count_++;

        if (element < current->element()) {
            current = (OSTNode<T> *)this->left(current);
        } else {
            current = (OSTNode<T> *)this->right(current);
        }
    }

    if (!last) {
        this->set_root(node);
    } else if (element < last->element()) {
        this->set_left(last, node);
    } else {
        this->set_right(last, node);
    }
}

template <class T>
void OrderStatisticTree<T>::remove(TreeNode<T> *z) {
    if (!this->left(z)) {
        this->transplant(z, this->right(z));
    } else if (!this->right(z)) {
        this->transplant(z, this->left(z));
    } else {
        TreeNode<T> *y = this->min(this->right(z));

        if (this->parent(y) != z) {
            this->transplant(y, this->right(y));
            decrement_count((OSTNode<T> *)y, (OSTNode<T> *)z);
            this->set_right(y, this->right(z));
        }

        this->transplant(z, y);
        ((OSTNode<T> *)y)->count_ = ((OSTNode<T> *)z)->count_ - 1;
        this->set_left(y, this->left(z));
    }

    this->set_left(z, nullptr);
    this->set_right(z, nullptr);

    decrement_count((OSTNode<T> *)z, nullptr);

    delete z;
}

template <class T>
void OrderStatisticTree<T>::decrement_count(
        OSTNode<T> *node, OSTNode<T> *until) {

    OSTNode<T> *parent = (OSTNode<T> *)this->parent(node);

    while (parent != until) {
        parent->count_--;
        parent = (OSTNode<T> *)this->parent(parent);
    }
}

template <class T>
T OrderStatisticTree<T>::select(int i) {
    return select(i, (OSTNode<T> *)this->root());
}

template <class T>
T OrderStatisticTree<T>::select(int i, OSTNode<T> *v) {
    return select_node(i, v)->element();
}

template <class T>
TreeNode<T> *OrderStatisticTree<T>::select_node(int i) {
    return select_node(i, (OSTNode<T> *)this->root());
}

template <class T>
TreeNode<T> *OrderStatisticTree<T>::select_node(int i, OSTNode<T> *v) {
    OSTNode<T> *left = (OSTNode<T> *)this->left(v);
    int count = left ? left->count_ : 0;

    if (i == count) {
        return v;
    } else if (i < count) {
        return select_node(i, left);
    } else {
        return select_node(i - count - 1, (OSTNode<T> *)this->right(v));
    }
}

template <class T>
int OrderStatisticTree<T>::rank(T element) {
    TreeNode<T> *node = this->search(element);

    if (!node) {
        return -1;
    }

    OSTNode<T> *left = (OSTNode<T> *)this->left(node);
    TreeNode<T> *root = this->root();

    int r = left ? left->count() : 0;

    while (node != root) {
        TreeNode<T> *parent = this->parent(node);

        if (node == this->right(parent)) {
            left = (OSTNode<T> *)this->left(parent);
            r += (left ? left->count() : 0) + 1;
        }

        node = parent;
    }

    return r;
}

}

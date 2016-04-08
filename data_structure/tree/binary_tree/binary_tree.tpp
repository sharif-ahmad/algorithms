template <class T>
Node<T>::Node(T element) {
    this->element = element;

    parent = NULL;
    left = NULL;
    right = NULL;
}

template <class T>
Node<T>::~Node() {
    delete left;
    delete right;
}

template <class T>
T Node<T>::get_element() {
    return element;
}

template <class T>
BinaryTree<T>::BinaryTree(Node<T> *root) {
   _root = root;
}

template <class T>
BinaryTree<T>::~BinaryTree() {
    delete _root;
}

template <class T>
Node<T> *BinaryTree<T>::root() {
    return _root;
}

template <class T>
Node<T> *BinaryTree<T>::parent(Node<T> *p) {
    return p->parent;
}

template <class T>
Node<T> *BinaryTree<T>::left(Node<T> *p) {
    return p->left;
}

template <class T>
Node<T> *BinaryTree<T>::right(Node<T> *p) {
    return p->right;
}

template <class T>
void BinaryTree<T>::set_left(Node<T> *p, Node<T> *child) {
    child->parent = p;
    p->left = child;
}

template <class T>
void BinaryTree<T>::set_right(Node<T> *p, Node<T> *child) {
    child->parent = p;
    p->right = child;
}

template <class T>
int BinaryTree<T>::depth(Node<T> *p) {
    int d = 0;

    while (p->parent != NULL) {
        p = p->parent;
        d++;
    }

    return d;
}

template <class T>
int BinaryTree<T>::height() {
    return height(_root);
}

template <class T>
int BinaryTree<T>::height(Node<T> *p) {
    if (!p) {
        return -1;
    }

    return 1 + MAX(height(p->left), height(p->right));
}

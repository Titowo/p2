template <typename T>
LinkedList<T>::~LinkedList() {
    clear();
}

// Implementacion de add
template <typename T>
bool LinkedList<T>::add(T value) {
    if (contains(value)) {
        return false;
    }

    Node<T>* newNode = new Node<T>(value);
    if (!head) {
        head = newNode;
    } else {
        Node<T>* current = head;
        while (current->next != nullptr) {
            current = current->next;
        }
        current->next = newNode;
    }
    size++;
    return true;
}

// Implementacion de contains
template <typename T>
bool LinkedList<T>::contains(T value) const {
    Node<T>* current = head;
    while (current != nullptr) {
        if (current->data == value) {
            return true;
        }
        current = current->next;
    } 
    return false;
}


// Implementacion de clear 
template <typename T>
void LinkedList<T>::clear() {
    Node<T>* current = head;
    Node<T>* nextNode;
    while (current != nullptr) {
        nextNode = current->next;
        delete current;
        current = nextNode;
    }
    head = nullptr;
    size = 0;
}
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

    // crea un nodo con el valor pasado y lo inserta en la lista
    Node<T>* nuevoNodo = new Node<T>(value);
    if (!head) {
        head = nuevoNodo;
    } else {
        Node<T>* actual = head;
        while (actual->next != nullptr) {
            actual = actual->next;
        }
        actual->next = nuevoNodo;
    }
    size++;
    return true;
}

// Implementacion de contains
template <typename T>
bool LinkedList<T>::contains(T value) const {
    // recorre la lista para ver si el valor existe en la lista
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
    // borra el dato y el nodo 
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
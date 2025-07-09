#include "InvertedIndex.h"

#include <iostream>

InvertedIndex::InvertedIndex() {
  // Constructor
}

InvertedIndex::~InvertedIndex() {
    for (const auto& vocab_pair : vocabulario) {
        delete vocab_pair.second;
    }
    vocabulario.clear();
}

void InvertedIndex::addDocumento(const std::string& termino, int doc_id) {
    auto it = vocabulario.find(termino);

    if (it == vocabulario.end()) {
        TermEntry* newEntry = new TermEntry();
        newEntry->listaPosteo->add(doc_id); // aniadir doc_id a la lista de posteo
        vocabulario[termino] = newEntry; // Insertar en el mapa
    } else {
        // si ya existe el termino, aniadir doc_id a la lista de posteo existentes
        // la funcion add de linkedlist se encarga de evitar duplicados
        it->second->listaPosteo->add(doc_id);
    }
}

// busca la lista de posteo de un temrino 
const LinkedList<int>* InvertedIndex::search(const std::string& termino) const {
    auto it = vocabulario.find(termino);
    if (it != vocabulario.end()) {
        return it->second->listaPosteo; // devolver la lista de posteo
    }
    return nullptr; // termino no encontrado
}

// funcion auxiliar para interseccion de dos listas de posteo
LinkedList<int>* InvertedIndex::interseccionListaPosteo(const LinkedList<int>* lista1, const LinkedList<int>* lista2) const {
    if (lista1 == nullptr || lista2 == nullptr) {
        return nullptr;
    }

    LinkedList<int>* resultado = new LinkedList<int>();
    Node<int>* p1 = lista1->getHead();
    Node<int>* p2 = lista2->getHead();

    while (p1 != nullptr && p2 != nullptr) {
        if (p1->data == p2->data) {
            resultado->add(p1->data); // aniadir el documento comun
            p1 = p1->next;
            p2 = p2->next;
    } else if (p1->data < p2->data) {
        p1 = p1->next;
    } else {
        p2 = p2->next;
        }
    }
    return resultado;
}

// busca documentos que contengan TODOS los temrinos usand AND y intersecciones
LinkedList<int>* InvertedIndex::search(const std::vector<std::string>& terminos) const {
    if (terminos.empty()) {
        return new LinkedList<int>(); // devover una lista vacia si no hay terminos
    }

    // obtener la primera lista de posteo
    LinkedList<int>* resultadoActual = nullptr;
    const LinkedList<int>* primeraLista = search(terminos[0]);
    if (primeraLista) {
        // hacer copia de la lista
        resultadoActual = new LinkedList<int>();
        Node<int>* nodoTermporal = primeraLista->getHead();
        while (nodoTermporal != nullptr) {
            resultadoActual->add(nodoTermporal->data);
            nodoTermporal = nodoTermporal->next;
        }
    } else {
        return new LinkedList<int>();
    }

    // intersecta con las listas de los demas terminoss
    for (size_t i = 1; i < terminos.size(); ++i) {
        const LinkedList<int>* nuevaLista = search(terminos[i]);
        if (!nuevaLista) {
            delete resultadoActual;
            return new LinkedList<int>();
        }

        LinkedList<int>* nuevaInterseccion = interseccionListaPosteo(resultadoActual, nuevaLista);
        delete resultadoActual;
        resultadoActual = nuevaInterseccion;

        if (resultadoActual->getSize() == 0) {
            return resultadoActual;
        }
    }
    return resultadoActual;
}

void InvertedIndex::printIndex() const {
    std::cout << "\n---Indice Invertido---" << std::endl;
    for (const auto& vocab_pair : vocabulario) {
    const std::string& termino = vocab_pair.first;
    TermEntry* termEntry = vocab_pair.second;

    std::cout << "Termino: '" << termino << "' -> Documentos: [";
    Node<int>* nodoActual = termEntry->listaPosteo->getHead();
    while (nodoActual != nullptr) {
        std::cout << nodoActual->data;
        if (nodoActual->next != nullptr) {
        std::cout << ", ";
    }
        nodoActual = nodoActual->next;
    }
    std::cout << "]" << std::endl;
    }
    std::cout << "------------------" << std::endl;
}
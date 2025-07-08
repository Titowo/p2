#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <string>
#include <map> // para el vocabulario (palabra -> TermEntry*)
#include <vector>
#include <algorithm>
#include <iostream>
#include <set>
#include <numeric>

#include "LinkedList.h"
#include "Node.h"


struct TermEntry {
    // std::string termino;
    LinkedList<int>* listaPosteo;

    TermEntry() : listaPosteo(new LinkedList<int>()) {} // Constructor
    ~TermEntry() {
        delete listaPosteo;
    }
};


class InvertedIndex {
public:
    InvertedIndex();
    ~InvertedIndex();

    void addDocumento(const std::string& termino, int doc_id);

    const LinkedList<int>* search(const std::string& termino) const;
    LinkedList<int>* search(const std::vector<std::string>& terminos) const;

    void printIndex() const;

    const std::map<std::string, TermEntry*>& getVocabulario() const { return vocabulario; } // para proyectyo 2
    LinkedList<int>* interseccionListaPosteo(const LinkedList<int>* lista1, const LinkedList<int>* lista2) const;

private:
    std::map<std::string, TermEntry*> vocabulario;
};

#endif
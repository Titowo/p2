#include "Buscador.h"
#include "LinkedList.h"
#include "ProcesadorDocumentos.h"
// #include "Utils.h"
#include <iostream>

Buscador::Buscador(InvertedIndex* index, ProcesadorDocumentos* docProcessor)
    : invertedIndex(index), docProcesador(docProcessor) {
    // Los punteros se inicializan en la lista de inicialización.
}

void Buscador::setPageRankScores(const std::map<int, double>* scores) {
    pageRankScores = scores;
}

std::vector<std::string> Buscador::procesarQueryString(const std::string& queryString) const {
    return docProcesador->getCleanWords(queryString);
}

LinkedList<int>* Buscador::query(const std::string& queryString) const {
    std::vector<std::string> terminosQuery = procesarQueryString(queryString);

    if (terminosQuery.empty()) {
        std::cout << "No se encontraron terminos validos para la consulta" << std::endl;
        return new LinkedList<int>();
    }

    if (terminosQuery.size() == 1) {
        const LinkedList<int>* resultado = invertedIndex->search(terminosQuery[0]);
        if (resultado) {
            LinkedList<int>* resultadoCopia = new LinkedList<int>();
            Node<int>* actual = resultado->getHead();
            while (actual != nullptr) {
                resultadoCopia->add(actual->data);
                actual = actual->next;
            }
            return resultadoCopia;
        } else {
            return new LinkedList<int>();
        }
    } else {
        
        const LinkedList<int>* primeraLista = invertedIndex->search(terminosQuery[0]);
        LinkedList<int>* resultadoActual;

        if (primeraLista) {
            resultadoActual = new LinkedList<int>();
            Node<int>* nodoTemporal = primeraLista->getHead();
            while(nodoTemporal != nullptr) {
                resultadoActual->add(nodoTemporal->data);
                nodoTemporal = nodoTemporal->next;
            }
        } else { 
            return new LinkedList<int>();
        }

        for (size_t i = 1; i < terminosQuery.size(); ++i) {
            const LinkedList<int>* nuevaLista = invertedIndex->search(terminosQuery[i]);
            if (!nuevaLista) {
                delete resultadoActual;
                return new LinkedList<int>();
            }

            LinkedList<int>* nuevaInterseccion = invertedIndex->interseccionListaPosteo(resultadoActual, nuevaLista);
            delete resultadoActual;
            resultadoActual = nuevaInterseccion;

            if (resultadoActual->getSize() == 0) {
                return resultadoActual;
            }
        }
        // return resultadoActual;
        // reordenamiento del pagerank
        if (resultadoActual->getSize() > 0 && pageRankScores != nullptr) {
            std::vector<std::pair<int, double>> rankedDocs;
            Node<int>* nodoResultadoActual = resultadoActual->getHead();

            while (nodoResultadoActual != nullptr) {
                int docId = nodoResultadoActual->data;
                double score = 0.0;
                auto it = pageRankScores->find(docId);
                if (it != pageRankScores->end()) {
                    score = it->second;
                } else {
                    score = 0.000000001; // valor pequenio para quedar al ultimo
                }
                rankedDocs.push_back({docId, score});
                // std::cout << "DEBUG: Doc " << docId << ", PR Score: " << score << std::endl;
                nodoResultadoActual = nodoResultadoActual->next;
            }

            std::sort(rankedDocs.begin(), rankedDocs.end(), [](const auto& a, const auto& b) {
                return a.second > b.second;
            });

            // crear una nuewva lista q este ordenada
            LinkedList<int>* resultadoFinalRankeado = new LinkedList<int>();
            for (const auto& docPair : rankedDocs) {
                resultadoFinalRankeado->add(docPair.first);
            }

            delete resultadoActual;
            return resultadoFinalRankeado;
        } else {
            return resultadoActual;
        }
    }

}

LinkedList<int>* Buscador::querySinPR(const std::string& queryString) const {
    std::vector<std::string> terminosQuery = procesarQueryString(queryString);

    if (terminosQuery.empty()) {
        return new LinkedList<int>();
    }

    LinkedList<int>* initialResults;

    if (terminosQuery.size() == 1) {
        const LinkedList<int>* resultado = invertedIndex->search(terminosQuery[0]);
        if (resultado) {
            initialResults = new LinkedList<int>();
            Node<int>* actual = resultado->getHead();
            while (actual != nullptr) {
                initialResults->add(actual->data);
                actual = actual->next;
            }
        } else {
            initialResults = new LinkedList<int>();
        }
    } else {
        const LinkedList<int>* primeraLista = invertedIndex->search(terminosQuery[0]);
        if (!primeraLista) {
            return new LinkedList<int>();
        }
        
        initialResults = new LinkedList<int>();
        Node<int>* nodoTemporal = primeraLista->getHead();
        while(nodoTemporal != nullptr) {
            initialResults->add(nodoTemporal->data);
            nodoTemporal = nodoTemporal->next;
        }

        for (size_t i = 1; i < terminosQuery.size(); ++i) {
            const LinkedList<int>* nuevaLista = invertedIndex->search(terminosQuery[i]);
            if (!nuevaLista) {
                delete initialResults;
                return new LinkedList<int>();
            }

            LinkedList<int>* nuevaInterseccion = invertedIndex->interseccionListaPosteo(initialResults, nuevaLista);
            delete initialResults;
            initialResults = nuevaInterseccion;

            if (initialResults->getSize() == 0) {
                return initialResults;
            }
        }
    }
    return initialResults;
}
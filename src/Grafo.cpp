#include "Grafo.h"

#include <iostream>
#include <ostream>

Grafo::Grafo() : numAristas(0) {
    // CONSTRUCTOR
}

Grafo::~Grafo() {
    // DESTRUCTOR
} 

std::map<int, double> Grafo::calcularPageRank(int num_interaciones, double damping_factor, double convergence_threshold) const {
    std::map<int, double> pageRank;
    std::map<int, double> anteriorPageRank;

    if (Nodos.empty()) {
        std::cout << "[PAGERANK] No hay nodos en el grafo para calcular PageRank" << std::endl;
        return pageRank;
    }

    double pr_inicial = 1.0 / Nodos.size();
    std::cout << "[PAGERANK DEBUG] Total de nodos: " << Nodos.size() << std::endl;

    for (int nodo_id : Nodos) {
        pageRank[nodo_id] = pr_inicial;
    }

    // calcular el peso de aristas salientes para cada nodo
    std::map<int, double> salidaSumaPesada;
    for (int nodo_id : Nodos) {
        double sum = 0.0;

        // revisa si el nodo tiene vertices salientes
        auto it = listaAdyacencia.find(nodo_id);
        if (it != listaAdyacencia.end()) {
            for (auto const& [vecino, peso] : it->second) {
               sum += peso; 
            }
        }

        salidaSumaPesada[nodo_id] = sum;
    }

    // 2) ITERACION DEL ALGORITMO PAGERANK
    std::cout << "[PAGERANK] Calculando PageRank con " << Nodos.size() << " nodos..." << std::endl;
    int iteracion_actual = 0;
    bool converge = false;

    while (iteracion_actual < num_interaciones && !converge) {
        anteriorPageRank = pageRank; // guarda los pr de la iteracion anterior
        converge = true; // se asume convergencia hasta que se demuestre lo contrario
        double sum_de_diff_pr = 0.0; // monitorea la convergencia

        for (int nodo_j : Nodos) {
            double sum_entrada_pr = 0.0;

            // ahora hay que iterar todos los ndoos I que tienen una arista hacia J
            // como el grafo no es dirijido, cuaklqueir nodo I que tenga a J en su lista de adyacencia
            // es un enlazador a J
            for (int nodo_i : Nodos) {
                auto it_i_adj = listaAdyacencia.find(nodo_i);
                if (it_i_adj != listaAdyacencia.end()) {
                    auto it_arista_ij = it_i_adj->second.find(nodo_j);
                    if (it_arista_ij != it_i_adj->second.end()) { // revisa si exist e una arista de I a J
                        double peso_ij = it_arista_ij->second;
                        double salida_suma_i = salidaSumaPesada[nodo_i];

                        // evitar q divida en 0 si un nodo no tiene enlaces salientes
                        if (salida_suma_i > 0) {
                            sum_entrada_pr += anteriorPageRank[nodo_i] * (peso_ij / salida_suma_i);
                        }

                    }
                }
            }

            // formula pagerank
            pageRank[nodo_j]  = (1.0 - damping_factor) + damping_factor * sum_entrada_pr;
            
            // verificar convergencia
            if (std::abs(pageRank[nodo_j] - anteriorPageRank[nodo_j]) > convergence_threshold) {
                converge = false;
            }
            sum_de_diff_pr += std::abs(pageRank[nodo_j] - anteriorPageRank[nodo_j]);

        }

        iteracion_actual++;
    }
    std::cout << "[PAGERANK] Calculo Finalizado en " << iteracion_actual << " iteraciones. Convergencia: " << (converge ? "Si" : "No") << std::endl;

    // normaliza pagerank scores para asegurar que sumen 1
    double total_pr_sum = 0.0;
    for (auto const& [nodo_id, pr_score] : pageRank) {
        total_pr_sum += pr_score;
    }
    if (total_pr_sum > 0) {
        for (auto& [nodo_id, pr_score] : pageRank) {
            pr_score /= total_pr_sum;
        }
    }

    return pageRank;
}

void Grafo::addVertice(int doc1_id, int doc2_id) {
    if (doc1_id == doc2_id) {
        return;
    } 

    Nodos.insert(doc1_id);
    Nodos.insert(doc2_id);

    listaAdyacencia[doc1_id][doc2_id]++;
    listaAdyacencia[doc2_id][doc1_id]++;

    numAristas++;
    // std::cout << "Vertice incrementado entre " << doc1_id << " y " << doc2_id << std::endl;
}

int Grafo::getNumNodes() const {
    return Nodos.size();
}

const std::map<int, std::map<int, double>>& Grafo::getListaAdyacencia() const {
    return listaAdyacencia;
}

const std::set<int>& Grafo::getNodos() const {
    return Nodos;
}

int Grafo::getNumAristas() const {
    return numAristas;
}
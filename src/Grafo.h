#ifndef GRAFO_H
#define GRAFO_H

#include <map>
#include <set>
#include <vector>

class Grafo {
public:
    Grafo();
    ~Grafo();

    void addVertice(int doc1_id, int doc2_id);
    int getNumNodes() const;
    int getNumAristas() const;

    const std::map<int, std::map<int,double>>& getListaAdyacencia() const;

    const std::set<int>& getNodos() const;

    std::map<int, double> calcularPageRank(int num_iteraciones = 50, double damping_factor = 0.85, double limite_convergencia = 1e-6) const;

private:
    std::map<int, std::map<int, double>> listaAdyacencia;
    std::set<int> Nodos;
    int numAristas;
};

#endif
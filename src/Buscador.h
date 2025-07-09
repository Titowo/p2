#ifndef BUSCADOR_H
#define BUSCADOR_H

#include <string>
#include <vector>
#include <sstream>
#include <map>

#include "InvertedIndex.h"
#include "ProcesadorDocumentos.h"
#include "LinkedList.h"


class Buscador {
public:
    Buscador(InvertedIndex* index, ProcesadorDocumentos* docProcessor);

    LinkedList<int>* query(const std::string& queryString) const;
    LinkedList<int>* querySinPR(const std::string& queryString) const;

    void setPageRankScores(const std::map<int, double>* scores);

    std::vector<std::string> procesarQueryString(const std::string& queryString) const;
private:
    InvertedIndex* invertedIndex;
    ProcesadorDocumentos* docProcesador;

    const std::map<int,double>* pageRankScores;

};

#endif
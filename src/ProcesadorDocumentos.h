#ifndef PROCESADOR_DOCUMENTOS_H
#define PROCESADOR_DOCUMENTOS_H

#include <string>
#include <vector>
#include <unordered_set> 
#include <fstream>       

// foward declaration de InvertedIndex
class InvertedIndex;

class ProcesadorDocumentos {
public:
    ProcesadorDocumentos();
    ~ProcesadorDocumentos();

    void cargarStopwords(const std::string& filename);

    int procesarContenidoDocumentos(const std::string& contenido, int documentoId, InvertedIndex& index);

    std::vector<std::string> getCleanWords(const std::string& text) const;

    void cargaYProcesadoDocumentos(const std::string& filename, InvertedIndex& index);

private:
    std::unordered_set<std::string> stopWords;
};

#endif // PROCESADOR_DOCUMENTOS_H
#ifndef DOCUMENT_PROCESSOR_H
#define DOCUMENT_PROCESSOR_H

#include <string>
#include <vector>
#include <unordered_set> // Para las stop words
#include <fstream>       // Para cargar stop words

// Forward declaration de InvertedIndex
class InvertedIndex;

class ProcesadorDocumentos {
public:
    ProcesadorDocumentos();
    ~ProcesadorDocumentos();

    void loadStopWords(const std::string& filename);

    // *** CAMBIO AQUÍ: Ahora devuelve el número de palabras añadidas ***
    int processDocumentContent(const std::string& content, int documentId, InvertedIndex& index);

    // Método para obtener palabras limpias de un texto (para la consulta, por ejemplo)
    std::vector<std::string> getCleanWords(const std::string& text) const;

    // Método para cargar y procesar documentos (este será modificado en .cpp)
    void loadAndProcessDocuments(const std::string& filename, InvertedIndex& index);

private:
    std::unordered_set<std::string> stopWords; // Usar el mismo nombre que en tu .cpp
};

#endif // DOCUMENT_PROCESSOR_H
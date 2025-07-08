#include "ProcesadorDocumentos.h"
#include "InvertedIndex.h"
#include "Utils.h" 
#include <iostream> 
#include <fstream>  
#include <sstream>  

ProcesadorDocumentos::ProcesadorDocumentos() {
    // Constructor
}

ProcesadorDocumentos::~ProcesadorDocumentos() {
    // Destructor
}

void ProcesadorDocumentos::loadStopWords(const std::string& filename) {
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cerr << "Error al abrir el archivo de stopwords: " << filename << std::endl;
        return;
    }

    std::string word;
    while (file >> word) {
        stopWords.insert(Utils::toLower(word));
    }
    file.close();
    std::cout << "Cargadas " << stopWords.size() << " stopwords." << std::endl;
}

int ProcesadorDocumentos::processDocumentContent(const std::string& linea, int doc_id, InvertedIndex& index) {
    // std::cout << "VERBOSE: Iniciando procesamiento de Doc ID: " << doc_id << std::endl; // Opcional

    size_t last_separator_pos = linea.rfind("||");
    if(last_separator_pos == std::string::npos || last_separator_pos + 2 >= linea.length()) {
        std::cerr << "Advertencia: Línea mal formada (Doc ID: " << doc_id << "): " << linea.substr(0, 50) << "..." << std::endl;
        return 0; // No se añadieron palabras si la línea está mal
    }
    std::string contenido = linea.substr(last_separator_pos + 2); // +2 para saltar "||"

    std::istringstream iss(contenido);
    std::string raw_word;
    int words_added_count_current_doc = 0; // Contador de palabras añadidas en este documento

    while (iss >> raw_word) {
        std::string cleaned_word = Utils::cleanWord(raw_word);

        if (!cleaned_word.empty()) {
            if (stopWords.find(cleaned_word) == stopWords.end()) {
                index.addDocumento(cleaned_word, doc_id);
                words_added_count_current_doc++;
            }
        }
    }
    // std::cout << "VERBOSE: Doc ID " << doc_id << " procesado. " << words_added_count_current_doc << " palabras añadidas al índice." << std::endl; // Opcional
    return words_added_count_current_doc; // Devuelve el conteo de palabras añadidas
}

// Este método es para la función de consulta, no lo modificaremos significativamente para esta tarea.
std::vector<std::string> ProcesadorDocumentos::getCleanWords(const std::string& text) const {
    std::vector<std::string> cleanWords;
    std::istringstream iss(text);
    std::string rawWord;

    while (iss >> rawWord) {
        std::string cleanedWord = Utils::cleanWord(rawWord);
        if (!cleanedWord.empty() && stopWords.find(cleanedWord) == stopWords.end()) {
            cleanWords.push_back(cleanedWord);
        }
    }
    return cleanWords;
}

void ProcesadorDocumentos::loadAndProcessDocuments(const std::string& filename, InvertedIndex& index) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de documentos: " << filename << std::endl;
        return;
    }

    std::cout << "VERBOSE: Iniciando la carga y procesamiento de documentos desde: " << filename << std::endl;
    std::string linea;
    int doc_id = 0;
    int processed_docs_count = 0;
    long long total_words_indexed = 0; // Contador de palabras totales indexadas
    const long long WORD_LIMIT = 2'000'000; // Límite de palabras a indexar

    while (std::getline(file, linea)) {
        if (total_words_indexed >= WORD_LIMIT) {
            std::cout << "VERBOSE: Limite de " << WORD_LIMIT << " palabras alcanzado. Deteniendo la indexacion inicial." << std::endl;
            break; // Salir del bucle si se alcanza el límite
        }

        // Procesa la línea y obtiene el número de palabras añadidas
        total_words_indexed += processDocumentContent(linea, doc_id, index);
        
        doc_id++; // Incrementa el ID para el siguiente documento
        processed_docs_count++;

        // Mensaje de progreso cada 100 documentos (puedes ajustar VERBOSE_DOC_STEP)
        const int VERBOSE_DOC_STEP = 100;
        if (processed_docs_count % VERBOSE_DOC_STEP == 0) {
            std::cout << "VERBOSE: " << processed_docs_count << " documentos procesados. Palabras indexadas: " << total_words_indexed << std::endl;
        }
    }
    file.close();
    std::cout << "VERBOSE: Finalizado el procesamiento de " << processed_docs_count << " documentos. Total palabras indexadas: " << total_words_indexed << std::endl;
    std::cout << "Carga y procesamiento de documentos completado." << std::endl;
}
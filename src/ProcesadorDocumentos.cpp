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

void ProcesadorDocumentos::cargarStopwords(const std::string& filename) {
    std::ifstream file(filename);
    if(!file.is_open()) {
        std::cerr << "Error al abrir el archivo de stopwords: " << filename << std::endl;
        return;
    }

    // agrega las palabras del archivo al set dentro de la clase
    std::string word;
    while (file >> word) {
        stopWords.insert(Utils::toLower(word));
    }
    file.close();
    std::cout << "Cargadas " << stopWords.size() << " stopwords." << std::endl;
}

int ProcesadorDocumentos::procesarContenidoDocumentos(const std::string& linea, int doc_id, InvertedIndex& index) {
    // std::cout << "VERBOSE: Iniciando procesamiento de Doc ID: " << doc_id << std::endl; // Opcional

    size_t separadoUltimaPos = linea.rfind("||"); // rfind retorna el primer caracter del ultimo match "https://cplusplus.com/reference/string/string/rfind/"
    if(separadoUltimaPos == std::string::npos || separadoUltimaPos + 2 >= linea.length()) { // npos = -1 size_t ( final de string )
        std::cerr << "Advertencia: Línea mal formada (Doc ID: " << doc_id << "): " 
        << linea.substr(0, 50)  // .substr retorna un string dentro de la string linea desde el 0 al 50 char 
        << "..." << std::endl;
        return 0;
    }
    std::string contenido = linea.substr(separadoUltimaPos+ 2); // +2 para saltas los ||

    std::istringstream iss(contenido);
    std::string palabraIndividial;
    int contadorPalabrasSumDocActual = 0;

    while (iss >> palabraIndividial) {
        std::string palabraLimpia = Utils::cleanWord(palabraIndividial);

        if (!palabraLimpia.empty()) {
            if (stopWords.find(palabraLimpia) == stopWords.end()) {
                index.addDocumento(palabraLimpia, doc_id);
                contadorPalabrasSumDocActual++;
            }
        }
    }
    // std::cout << "VERBOSE: Doc ID " << doc_id << " procesado. " << contadorPalabrasSumDocActual << " palabras añadidas al índice." << std::endl; // Opcional
    return contadorPalabrasSumDocActual;
}

std::vector<std::string> ProcesadorDocumentos::getCleanWords(const std::string& text) const {
    std::vector<std::string> cleanWords;
    std::istringstream iss(text);
    std::string palabraInicial;

    while (iss >> palabraInicial) {
        std::string palabraLimpia = Utils::cleanWord(palabraInicial);
        if (!palabraInicial.empty() && stopWords.find(palabraLimpia) == stopWords.end()) {
            cleanWords.push_back(palabraInicial); // se aniade la palabra al final del vector
        }
    }
    return cleanWords;
}

void ProcesadorDocumentos::cargaYProcesadoDocumentos(const std::string& filename, InvertedIndex& index) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error al abrir el archivo de documentos: " << filename << std::endl;
        return;
    }

    std::cout << "VERBOSE: Iniciando la carga y procesamiento de documentos desde: " << filename << std::endl;
    std::string linea;
    int doc_id = 0;
    int contadorPalabrasProcesadas = 0;
    long long totalPalabrasIndexadas = 0; // Contador de palabras totales indexadas
    const long long LIMITE_INDEX= 500'000; // Límite de palabras a indexar

    while (std::getline(file, linea)) {
        if (totalPalabrasIndexadas>= LIMITE_INDEX) {
            std::cout << "VERBOSE: Limite de " <<   LIMITE_INDEX << " palabras alcanzado. Deteniendo la indexacion inicial." << std::endl;
            break;
        }

        // Procesa la línea y obtiene el número de palabras añadidas
        totalPalabrasIndexadas += procesarContenidoDocumentos(linea, doc_id, index);

        doc_id++; // Incrementa el ID para el siguiente documento
        contadorPalabrasProcesadas++;

        // Mensaje de progreso cada 100 documentos (puedes ajustar VERBOSE_DOC_STEP)
        const int VERBOSE_DOC_STEP = 100;
        if (contadorPalabrasProcesadas % VERBOSE_DOC_STEP == 0) {
            std::cout << "VERBOSE: " << contadorPalabrasProcesadas 
                        << " documentos procesados. Palabras indexadas: " 
                        << totalPalabrasIndexadas << std::endl;
        }
    }
    file.close();
    std::cout << "VERBOSE: Finalizado el procesamiento de " << contadorPalabrasProcesadas 
            << " documentos. Total palabras indexadas: " << totalPalabrasIndexadas << std::endl;
    std::cout << "Carga y procesamiento de documentos completado." << std::endl;
}
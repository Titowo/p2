#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>

#include "Buscador.h"
#include "Grafo.h"
#include "InvertedIndex.h"
#include "ProcesadorDocumentos.h"
#include "LinkedList.h"

#define STOPWORDS_FILE "data/stopwords_english.dat.txt"
#define DOCUMENT_FILE "data/gov2_pages.dat"
#define QUERY_LOGS "data/Log-Queries.dat"

#define QUERY_LOG_LIMIT 5'000 
#define TOP_K_DOCUMENTOS 10

int main() {
    std::cout << "[MAIN] Iniciando motor de busqueda... " << std::endl;

    // 1) INICIAR OBJETOS
    ProcesadorDocumentos pd;
    InvertedIndex ii;
    Buscador bs(&ii, &pd);
    Grafo g;

    // 2) CARGAR STOPWORDS
    std::cout << "[MAIN] Cargando STOPWORDS..." << std::endl;
    pd.cargarStopwords(STOPWORDS_FILE);

    // 3) CARGAR DOCUMENTOS
    std::cout << "[MAIN] Cargando y procesando documentos (" << DOCUMENT_FILE
              << ")..." << std::endl;
    auto start_time = std::chrono::high_resolution_clock::now();
    pd.cargaYProcesadoDocumentos(DOCUMENT_FILE, ii);
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
        end_time - start_time);
    std::cout << "[MAIN] Tiempo de carga y procesamiento: " << duration.count()
              << " ms" << std::endl;

    // 3.5) CONSTRUCCION GRAFO OFFLINE
    std::cout
        << "[MAIN] Construyendo Grafo de co-relevancia desde logs de consulta..."
        << std::endl;
    start_time = std::chrono::high_resolution_clock::now();

    std::ifstream QUERY_LOGS_PROCESADOR(QUERY_LOGS);
    if (!QUERY_LOGS_PROCESADOR.is_open()) {
        std::cerr << "[ERROR] No se pudo abrir el archivo de consultas: "
                  << QUERY_LOGS << std::endl;
        return 1;
    }

    std::string lineaQuery;
    int queryCount = 0;
    while (std::getline(QUERY_LOGS_PROCESADOR, lineaQuery) &&
           queryCount < QUERY_LOG_LIMIT) {
        if (lineaQuery.empty()) {
            continue; // Saltar líneas vacías
        }

        LinkedList<int> *resultadoQuery = bs.querySinPR(lineaQuery);

        if (resultadoQuery && resultadoQuery->getSize() > 0) {
            std::vector<int> topKDocs; // vector para guardar los id de los docs, pero hasta K
            Node<int>* actual = resultadoQuery->getHead();
            int count = 0;
            // mientras actual no sea nullo y top_k_documentos sea mayor al contador sigue aniadiendo a topKDocs
            while (actual != nullptr && count < TOP_K_DOCUMENTOS) {
                topKDocs.push_back(actual->data);
                actual = actual->next;
                count++;
            }

            // creacion del grafico de co-relevancia; por cada elemento em topKcods se aniade una arista al grafo
            for (size_t i = 0; i < topKDocs.size(); ++i) {
                for (size_t j = i + 1; j < topKDocs.size(); ++j) {
                    g.addVertice(topKDocs[i], topKDocs[j]);
                }
            }
        }

        if (resultadoQuery) {
            delete resultadoQuery; // Liberar la memoria de la lista
            resultadoQuery = nullptr;
        }
        queryCount++;
        if (queryCount % 1000 == 0) {
            std::cout << "[MAIN] Procesadas " << queryCount << " consultas del log."
                      << std::endl;
        }
    }
    QUERY_LOGS_PROCESADOR.close();
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                                      start_time);

    std::cout << "[MAIN] Grafo construido con " << g.getNumNodes() << " nodos y " << g.getNumAristas() << " aristas en "
            << duration.count() << " ms." << std::endl;

    // 3.9) CALCULAR PAGERANK
    std::cout << "[MAIN] Calculando PageRank..." << std::endl;
    start_time = std::chrono::high_resolution_clock::now();
    std::map<int, double> pageRankScores = g.calcularPageRank();
    end_time = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                                      start_time);
    std::cout << "[MAIN] PageRank calculado en " << duration.count() << " ms."
              << std::endl;

    bs.setPageRankScores(&pageRankScores); // Pasar las puntuaciones de PageRank al buscador

    // 4) HACER CONSULTAS
    std::cout << "\n---- Busqueda de Documentos ----" << std::endl;
    std::cout << "Ingrese consulta (o 'salir' para terminar):" << std::endl;

    while (std::getline(std::cin, lineaQuery) && lineaQuery != "salir") {
        if (lineaQuery.empty()) {
            std::cout << "Por favor, ingrese una consulta..." << std::endl;
            continue;
        }

        // --- Resultados SIN PageRank ---
        std::cout << "\nBuscando (SIN PageRank) '" << lineaQuery << "'..." << std::endl;
        start_time = std::chrono::high_resolution_clock::now();
        LinkedList<int> *resultadoSinPR = bs.querySinPR(lineaQuery);
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time);

        if (resultadoSinPR && resultadoSinPR->getSize() > 0) {
            std::cout << "Documentos encontrados (SIN PageRank): ("
                      << resultadoSinPR->getSize() << ")" << std::endl;
            std::cout << "Top 10 documentos (sin PR): [";
            Node<int> *actual = resultadoSinPR->getHead();
            int count = 0;
            while (actual != nullptr && count < 10) {
                std::cout << actual->data;
                if (actual->next != nullptr && count < 9) { // no imprimir coma despues del ultimo elemento
                    std::cout << ", ";
                }
                actual = actual->next;
                count++;
            }
            std::cout << "]" << std::endl;
        } else {
            std::cout
                << "No se encontraron documentos para la consulta (SIN PageRank)."
                << std::endl;
        }
        std::cout << "Tiempo de Busqueda (SIN PageRank): " << duration.count() << " ms"
                  << std::endl;
        if (resultadoSinPR) {
            delete resultadoSinPR;
            resultadoSinPR = nullptr;
        }

        // --- Resultados CON PageRank ---
        std::cout << "\nBuscando (CON PageRank) '" << lineaQuery << "'..." << std::endl;
        start_time = std::chrono::high_resolution_clock::now();
        LinkedList<int> *resultadoConPR = bs.query(lineaQuery);
        end_time = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(
            end_time - start_time);

        if (resultadoConPR && resultadoConPR->getSize() > 0) {
            std::cout << "Documentos encontrados (CON PageRank): ("
                      << resultadoConPR->getSize()
                      << ") [Tiempo: " << duration.count() << " ms]" << std::endl;
            std::cout << "Top 10 documentos (con PR): [";
            Node<int> *actual = resultadoConPR->getHead();
            int count = 0;
            while (actual != nullptr &&
                   count < 10) { // solo los primero 10 datos
                std::cout << actual->data;
                if (actual->next != nullptr && count < 9) {
                    std::cout << ", ";
                }
                actual = actual->next;
                count++;
            }
            std::cout << "]" << std::endl;
        } else {
            std::cout
                << "No se encontraron documentos para la consulta (CON PageRank)."
                << std::endl;
        }
        std::cout << "Tiempo de Busqueda (CON PageRank): " << duration.count() << " ms"
                  << std::endl;
        if (resultadoConPR) {
            delete resultadoConPR;
            resultadoConPR = nullptr;
        }

        std::cout << "\nIngrese una consulta (o 'salir' para terminar):"
                  << std::endl;
    }
    return 0;
}

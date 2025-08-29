#include <iostream>
#include <vector>
#include <limits> // Para manejo de límites de entrada
#include <memory> // Para std::unique_ptr y std::make_unique
#include "persona.h"
#include "generador.h"
#include "monitor.h" // Nuevo header para monitoreo

void mostrarMenu() {
    std::cout << "\n\n=== MENÚ PRINCIPAL ===";
    std::cout << "\n0. Crear nuevo conjunto de datos";
    std::cout << "\n1. Mostrar resumen de todas las personas";
    std::cout << "\n2. Mostrar detalle completo por índice";
    std::cout << "\n3. Buscar persona por ID";
    std::cout << "\n4. Mostrar estadísticas de rendimiento";
    std::cout << "\n5. Exportar estadísticas a CSV";
    std::cout << "\n6. Declarantes de renta -> [Valor]";
    std::cout << "\n7. Declarantes de renta -> [Referencia]";
    std::cout << "\n8. Ranking de riqueza por agrupación -> [Valor]";
    std::cout << "\n9. Ranking de riqueza por agrupación -> [Referencia]";
    std::cout << "\n10. Ranking de riqueza por ciudad -> [Valor]";
    std::cout << "\n11. Ranking de riqueza por ciudad -> [Referencia]";
    std::cout << "\n12. Mayor patrimonio -> [Valor]";
    std::cout << "\n13. Mayor patrimonio -> [Referencia]";
    std::cout << "\n14. Persona mas longeva - [Valor]";
    std::cout << "\n15. Persona mas longeva - [Referencia]";
    std::cout << "\n16. Salir";
    std::cout << "\nSeleccione una opción: ";
}

int main() {
    srand(time(nullptr));
    
    // Usar unique_ptr para manejar la colección de personas
    std::unique_ptr<std::vector<Persona>> personas = nullptr;
    Monitor monitor;
    
    int opcion;
    do {
        mostrarMenu();
        std::cin >> opcion;
        
        size_t tam = 0;
        int indice;
        std::string idBusqueda;
        
        // Iniciar medición de tiempo y memoria para esta operación
        monitor.iniciar_tiempo();
        long memoria_inicio = monitor.obtener_memoria();
        
        switch(opcion) {
            case 0: {
                int n;
                std::cout << "\nIngrese el número de personas a generar: ";
                std::cin >> n;
                
                if (n <= 0) {
                    std::cout << "Error: Debe generar al menos 1 persona\n";
                    break;
                }
                
                // Generar el nuevo conjunto de datos
                personas = std::make_unique<std::vector<Persona>>(generarColeccion(n));
                tam = personas->size();
                
                double tiempo_gen = monitor.detener_tiempo();
                long memoria_gen = monitor.obtener_memoria() - memoria_inicio;
                
                std::cout << "Generadas " << tam << " personas en " 
                          << tiempo_gen << " ms, Memoria: " << memoria_gen << " KB\n";
                
                monitor.registrar("Crear datos", tiempo_gen, memoria_gen);
                break;
            }
                
            case 1: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                tam = personas->size();
                std::cout << "\n=== RESUMEN DE PERSONAS (" << tam << ") ===\n";
                for(size_t i = 0; i < tam; ++i) {
                    std::cout << i << ". ";
                    (*personas)[i].mostrarResumen();
                    std::cout << "\n";
                }
                
                double tiempo_mostrar = monitor.detener_tiempo();
                long memoria_mostrar = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar resumen", tiempo_mostrar, memoria_mostrar);
                break;
            }
                
            case 2: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                tam = personas->size();
                std::cout << "\nIngrese el índice (0-" << tam-1 << "): ";
                if(std::cin >> indice) {
                    if(indice >= 0 && static_cast<size_t>(indice) < tam) {
                        (*personas)[indice].mostrar();
                    } else {
                        std::cout << "Índice fuera de rango!\n";
                    }
                } else {
                    std::cout << "Entrada inválida!\n";
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
                
                double tiempo_detalle = monitor.detener_tiempo();
                long memoria_detalle = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar detalle", tiempo_detalle, memoria_detalle);
                break;
            }
                
            case 3: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                std::cout << "\nIngrese el ID a buscar: ";
                std::cin >> idBusqueda;
                
                if(const Persona* encontrada = buscarPorID(*personas, idBusqueda)) {
                    encontrada->mostrar();
                } else {
                    std::cout << "No se encontró persona con ID " << idBusqueda << "\n";
                }
                
                double tiempo_busqueda = monitor.detener_tiempo();
                long memoria_busqueda = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Buscar por ID", tiempo_busqueda, memoria_busqueda);
                break;
            }
                
            case 4:
                monitor.mostrar_resumen();
                break;
                
            case 5:
                monitor.exportar_csv();
                break;
                
            case 6:{// Declarantes de renta - Por Valor
                std::map<std::string, std::vector<Persona>> agrupacion = agruparCalendario(*personas);
                std::map<std::string, std::vector<Persona>> declarantes = declarantesRenta(agrupacion);
                std::cout << "\n--- Declarantes de Renta (Valor) ---\n";
                for (const auto& grupo : declarantes){
                    std::cout << "Grupo '" << grupo.first << "' tiene " << grupo.second.size() << " declarantes:\n";
                    for (const Persona& persona : grupo.second){
                        persona.mostrarResumen();
                        std::cout << "\n";
                    }
                    std::cout << "\n";
                }

                break;
            }
            case 7:{//Declarantes de renta - Por Referencia
                std::map<std::string, std::vector<Persona>> agrupacion;
                agruparCalendarioRef(*personas, agrupacion);
                std::map<std::string, std::vector<Persona>> declarantes;
                declarantesRentaRef(agrupacion, declarantes);
                std::cout << "\n--- Declarantes de Renta (Referencia) ---\n";
                for (const auto& grupo : declarantes){
                    std::cout << "Grupo '" << grupo.first << "' tiene " << grupo.second.size() << " declarantes:\n";
                    for (const Persona& persona : grupo.second){
                        persona.mostrarResumen();
                        std::cout << "\n";
                    }
                    std::cout << "\n";
                }
                break;
            }
            case 8:{ //Ranking de riqueza por agrupación - Por Valor
                std::map<std::string, std::vector<Persona>> agrupacion = agruparCalendario(*personas);
                std::vector<std::pair<std::string, double>> ranking = rankingRiqueza(agrupacion);
                std::cout << "\n--- Ranking de Riqueza por Agrupación (Valor) ---\n";
                for (const auto& par : ranking){
                    std::cout << "Grupo '" << par.first << "' - Ingresos Totales: " << par.second << "\n";
                }   
                break;
            }

            case 9:{// Ranking de riqueza por agrupación - Por Referencia
                std::map<std::string, std::vector<Persona>> agrupacion;
                agruparCalendarioRef(*personas, agrupacion);
                std::vector<std::pair<std::string, double>> ranking;
                rankingRiquezaRef(agrupacion, ranking);
                std::cout << "\n--- Ranking de Riqueza por Agrupación (Referencia) ---\n";
                for (const auto& par : ranking){
                    std::cout << "Grupo '" << par.first << "' - Ingresos Totales: " << par.second << "\n";
                }
                break;
            }

            case 10:{// Ranking de riqueza por ciudad - Por Valor
                std::map<std::string, std::vector<Persona>> ciudad = agruparCiudad(*personas);
                std::vector<std::pair<std::string, double>> ranking = rankingRiquezaCiudad(ciudad);
                std::cout << "\n--- Ranking de Riqueza por Ciudad (Valor) ---\n";
                for (const auto& par : ranking){
                    std::cout << "Ciudad '" << par.first << "' - Ingresos Totales: " << par.second << "\n";
                }
                break;
            }
            case 11:{// Ranking de riqueza por ciudad - Por Referencia
                std::map<std::string, std::vector<Persona>> ciudad;
                agruparCiudadRef(*personas, ciudad);
                std::vector<std::pair<std::string, double>> ranking;
                rankingRiquezaCiudadRef(ciudad, ranking);
                std::cout << "\n--- Ranking de Riqueza por Ciudad (Referencia) ---\n";
                for (const auto& par : ranking){
                    std::cout << "Ciudad '" << par.first << "' - Ingresos Totales: " << par.second << "\n";
                }   
                break;
            }

            case 12: { // Mayor patrimonio - Por Valor
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                mostrarMayorPatrimonioPorValor(*personas);
                break;
            }
            case 13: { // Mayor patrimonio - Por Referencia
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                mostrarMayorPatrimonioPorReferencia(std::make_unique<std::vector<Persona>>(*personas));
                break;
            }

            case 14: {
                Persona vieja = edadMasLongevaPais(*personas);
                vieja.mostrarResumen();
                std::cout << "\nMas longeva por ciudad:\n";
                std::vector<Persona> viejaCiudad = edadMasLongevaCiudad(*personas);
                for (const auto& persona : viejaCiudad) {
                    persona.mostrarResumen();
                    std::cout << "\n"; 
                }
                std::cout << "\nPromedio de edad en el país (valor):\n";
                double promedio = promedioEdadPais(*personas);
                std::cout << promedio << " años\n";
                break;
            }

            case 15: {
                std::cout << "\nUsando referencias:\n";
                edadMasLongevaPaisRef(*personas);
                std::cout << "\nUsando referencias por ciudad:\n";
                edadMasLongevaCiudadRef(*personas);
                std::cout << "\nPromedio de edad en el país (referencia):\n";
                promedioEdadPaisRef(*personas);
                break;
            }
                
            default:
                std::cout << "Opción inválida!\n";
        }
        
        // Mostrar estadísticas para las operaciones principales
        if (opcion >= 0 && opcion <= 11) {
            double tiempo = monitor.detener_tiempo();
            long memoria = monitor.obtener_memoria() - memoria_inicio;
            monitor.mostrar_estadistica("Opción " + std::to_string(opcion), tiempo, memoria);
        }
        
    } while(opcion != 16);
    
    return 0;
}
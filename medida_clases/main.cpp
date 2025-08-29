#include <iostream>
#include <vector>
#include <limits>
#include <memory>
#include "persona.h"
#include "generador.h"
#include "monitor.h"
#include <map>
/**
 * Muestra el menú principal de la aplicación.
 * 
 * POR QUÉ: Guiar al usuario a través de las funcionalidades disponibles.
 * CÓMO: Imprimiendo las opciones en consola.
 * PARA QUÉ: Interacción amigable con el usuario.
 */
void mostrarMenu() {
    std::cout << "\n\n=== MENÚ PRINCIPAL ===";
    std::cout << "\n0. Crear nuevo conjunto de datos";
    std::cout << "\n1. Mostrar resumen de todas las personas";
    std::cout << "\n2. Mostrar detalle completo por índice";
    std::cout << "\n3. Buscar persona por ID";
    std::cout << "\n4. Mostrar estadísticas de rendimiento";
    std::cout << "\n5. Exportar estadísticas a CSV";
    std::cout << "\n6. Mostrar edad más longeva por país -> [Valor]";
    std::cout << "\n7. Mostrar edad más longeva por país -> [Referencia]";
    std::cout << "\n8. Declarantes de renta -> [Valor]";
    std::cout << "\n9. Declarantes de renta -> [Referencia]";
    std::cout << "\n10. Ranking de riqueza por agrupación -> [Valor]";
    std::cout << "\n11. Ranking de riqueza por agrupación -> [Referencia]";
    std::cout << "\n12. Ranking de riqueza por ciudad -> [Valor]";
    std::cout << "\n13. Ranking de riqueza por ciudad -> [Referencia]";
    std::cout << "\n14. Mayor patrimonio -> [Valor]";
    std::cout << "\n15. Mayor patrimonio -> [Referencia]";
    std::cout << "\n16. Salir";
    std::cout << "\nSeleccione una opción: ";
}

/**
 * Punto de entrada principal del programa.
 * 
 * POR QUÉ: Iniciar la aplicación y manejar el flujo principal.
 * CÓMO: Mediante un bucle que muestra el menú y procesa la opción seleccionada.
 * PARA QUÉ: Ejecutar las funcionalidades del sistema.
 */
int main() {
    srand(time(nullptr)); // Semilla para generación aleatoria
    
    // Puntero inteligente para gestionar la colección de personas
    // POR QUÉ: Evitar fugas de memoria y garantizar liberación automática.
    std::unique_ptr<std::vector<Persona>> personas = nullptr;
    
    Monitor monitor; // Monitor para medir rendimiento
    
    int opcion;
    do {
        mostrarMenu();
        std::cin >> opcion;
        
        // Variables locales para uso en los casos
        size_t tam = 0;
        int indice;
        std::string idBusqueda;
        
        // Iniciar medición de tiempo y memoria para la operación actual
        monitor.iniciar_tiempo();
        long memoria_inicio = monitor.obtener_memoria();
        
        switch(opcion) {
            case 0: { // Crear nuevo conjunto de datos
                int n;
                std::cout << "\nIngrese el número de personas a generar: ";
                std::cin >> n;
                
                if (n <= 0) {
                    std::cout << "Error: Debe generar al menos 1 persona\n";
                    break;
                }
                
                // Generar el nuevo conjunto de personas
                auto nuevasPersonas = generarColeccion(n);
                tam = nuevasPersonas.size();
                
                // Mover el conjunto al puntero inteligente (propiedad única)
                personas = std::make_unique<std::vector<Persona>>(std::move(nuevasPersonas));
                
                // Medir tiempo y memoria usada
                double tiempo_gen = monitor.detener_tiempo();
                long memoria_gen = monitor.obtener_memoria() - memoria_inicio;
                
                std::cout << "Generadas " << tam << " personas en " 
                          << tiempo_gen << " ms, Memoria: " << memoria_gen << " KB\n";
                
                // Registrar la operación
                monitor.registrar("Crear datos", tiempo_gen, memoria_gen);
                break;
            }
                
            case 1: { // Mostrar resumen de todas las personas
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
                
            case 2: { // Mostrar detalle por índice
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
                
            case 3: { // Buscar por ID
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
                
            case 4: // Mostrar estadísticas de rendimiento
                monitor.mostrar_resumen();
                break;
            
            case 5: // Exportar estadísticas a CSV
                monitor.exportar_csv();
                break;

            case 6: { //mas longevo por ciudad - valor
                Persona vieja = Persona::edadMasLongevaPais(*personas);
                vieja.mostrarResumen();
                std::cout << "\nMas longeva por ciudad:\n";
                std::vector<Persona> viejaCiudad = Persona::edadMasLongevaCiudad(*personas);
                for (const auto& persona : viejaCiudad) {
                    persona.mostrarResumen();
                    std::cout << "\n"; 
                }
                std::cout << "\nPromedio de edad en el país (valor):\n";
                double promedio = Persona::promedioEdadPais(*personas);
                std::cout << promedio << " años\n";
                double tiempo_detalle = monitor.detener_tiempo();
                long memoria_detalle = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Longevo valor", tiempo_detalle, memoria_detalle);
                break;
            }

            case 7:{ //Mas longevo por ciudad - referencia
                std::cout << "\nUsando referencias:\n";
                Persona::edadMasLongevaPaisRef(*personas);
                std::cout << "\nUsando referencias por ciudad:\n";
                Persona::edadMasLongevaCiudadRef(*personas);
                std::cout << "\nPromedio de edad en el país (referencia):\n";
                Persona::promedioEdadPaisRef(*personas);
                double tiempo_detalle = monitor.detener_tiempo();
                long memoria_detalle = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Longevo referencia", tiempo_detalle, memoria_detalle);
                break;
            }

            case 8: { //Declarantes de renta - Valor
                std::map<std::string, std::vector<Persona>> agrupacion = Persona::agruparCalendario(*personas);
                auto declarantes = Persona::declarantesRenta(agrupacion);
                std::cout << "\n--- Declarantes de Renta por Calendario ---\n";
                for (const auto& grupo : declarantes) {
                    std::cout << "\n";
                    std::cout << "--------- Calendario " << grupo.first << ": " << grupo.second.size() << " declarantes---------\n";
                    std::cout << "\n";
                    for (const Persona& persona : grupo.second) {
                        persona.mostrarResumen();
                        std::cout << "\n";
                    }
                }
                break;
   
            }

            case 9:{ //Declarantes de renta - Referencia
                std::map<std::string, std::vector<Persona>> calendarioAgrupado;
                Persona::agruparCalendarioRef(*personas, calendarioAgrupado);
                std::map<std::string, std::vector<Persona>> declarantes;
                Persona::declarantesRentaRef(calendarioAgrupado, declarantes);
                std::cout << "\n--- Declarantes de Renta por Calendario (Referencia) ---\n";
                for (const auto& grupo : declarantes) {
                    std::cout << "\n";
                    std::cout << "--------- Calendario " << grupo.first << ": " << grupo.second.size() << " declarantes---------\n";
                    std::cout << "\n";
                    for (const Persona& persona : grupo.second) {
                        persona.mostrarResumen();
                        std::cout << "\n";
                    }
                }
                break;
            }

            case 10: { //Ranking de riqueza por agrupación - Valor
                std::map<std::string, std::vector<Persona>> agrupacion = Persona::agruparCalendario(*personas);
                auto ranking = Persona::rankingRiqueza(agrupacion);
                std::cout << "\n--- Ranking de Riqueza por Calendario ---\n";
                int posicion = 1;
                for (const auto& par : ranking) {
                    std::cout << posicion << ". Calendario '" << par.first << "': Suma de ingresos = " << par.second << std::endl;
                    posicion++;
                }
                double tiempo_detalle = monitor.detener_tiempo();
                long memoria_detalle = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar detalle", tiempo_detalle, memoria_detalle);
                break;
            }

            case 11:{ //Ranking de riqueza por agrupación - Referencia
                std::map<std::string, std::vector<Persona>> calendarioAgrupado;
                Persona::agruparCalendarioRef(*personas, calendarioAgrupado);
                std::vector<std::pair<std::string, double>> ranking;
                Persona::rankingRiquezaRef(calendarioAgrupado, ranking);
                std::cout << "\n--- Ranking de Riqueza por Calendario (Referencia) ---\n";
                int posicionRef = 1;
                for (const auto& par : ranking) {
                    std::cout << posicionRef << ". Calendario '" << par.first << "': Suma de ingresos = " << par.second << std::endl;
                    posicionRef++;
                }
                break;

            }

            case 12: { //Ranking de riqueza por ciudad - Valor
                std::map<std::string, std::vector<Persona>> ciudad = Persona::agruparCiudad(*personas);
                auto rankingCiudad = Persona::rankingRiquezaCiudad(ciudad);
                std::cout << "\n--- Ranking de Riqueza por Ciudad ---\n";
                int posicionCiudad = 1;
                for (const auto& par : rankingCiudad) {
                    std::cout << posicionCiudad << ". Ciudad '" << par.first << "': Suma de ingresos = " << par.second << std::endl;
                    posicionCiudad++;
                }
                break;
            }

            case 13: { //Ranking de riqueza por ciudad - Referencia
                std::map<std::string, std::vector<Persona>> ciudadAgrupada;
                Persona::agruparCiudadRef(*personas, ciudadAgrupada);
                std::vector<std::pair<std::string, double>> rankingCiudadRef;
                Persona::rankingRiquezaCiudadRef(ciudadAgrupada, rankingCiudadRef);
                std::cout << "\n--- Ranking de Riqueza por Ciudad (Referencia) ---\n";
                int posicionCiudadRef = 1;
                for (const auto& par : rankingCiudadRef) {
                    std::cout << posicionCiudadRef << ". Ciudad '" << par.first << "': Suma de ingresos = " << par.second << std::endl;
                    posicionCiudadRef++;
                }
                
                break;
            }


            case 14: { // Mayor patrimonio - Valor
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                Persona::mostrarMayorPatrimonioPorValor(*personas);
                break;
                double tiempo_detalle = monitor.detener_tiempo();
                long memoria_detalle = monitor.obtener_memoria() - memoria_inicio;
                monitor.registrar("Mostrar detalle", tiempo_detalle, memoria_detalle);
            }

            case 15: { // Mayor patrimonio - Referencia
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                Persona::mostrarMayorPatrimonioPorReferencia(std::make_unique<std::vector<Persona>>(*personas));
                break;
            }

            case 16: // Salir
                std::cout << "Saliendo...\n";
                break;

            default:

                std::cout << "Opción inválida!\n";
        }
        
        // Mostrar estadísticas de la operación (excepto para opciones 4,5,6)
        if (opcion >= 0 && opcion <= 15) {
            double tiempo = monitor.detener_tiempo();
            long memoria = monitor.obtener_memoria() - memoria_inicio;
            monitor.mostrar_estadistica("Opción " + std::to_string(opcion), tiempo, memoria);
        }
        
    } while(opcion != 16);
    
    return 0;
}

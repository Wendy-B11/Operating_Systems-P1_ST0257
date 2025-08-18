#include <iostream>
#include <vector>
#include <limits>
#include <memory>
#include "persona.h"
#include "generador.h"
#include "monitor.h"
#include <map>
#include <algorithm>

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
    std::cout << "\n6. TODAS las funcionalidades por VALOR";
    std::cout << "\n7. TODAS las funcionalidades por REFERENCIA";
    std::cout << "\n8. Preguntas adicionales por VALOR";
    std::cout << "\n8. Preguntas adicionales por REFERENCIA";
    std::cout << "\n9. Salir";
    std::cout << "\nSeleccione una opción: ";
}

void menuLongevidad(){
    std::cout << "\n\n=== MENÚ LONGEVIDAD ===";
    std::cout << "\n1. En el pais";
    std::cout << "\n2. En la ciudad\n";
}

void menuAdicionales(){
    std::cout << "\n\n=== MENÚ ADICIONALES ===";
    std::cout << "\n1. Ranking de riqueza por agrupaciones";
    std::cout << "\n2. Ranking de riqueza por ciudad";
    std::cout << "\n3. Promedio de edades por ciudad\n";
}

std::map<char, std::vector<Persona>> agruparPersonasPorCalendario(const std::vector<Persona>& personas) {
    // El mapa guarda los resultados. 
    // La clave A, B, o C accederá al vector de personas correspondiente.
    std::map<char, std::vector<Persona>> grupos;

    for (const auto& persona : personas) {
        std::string id = persona.getId();

        // Dos últimos dígitos del ID
        std::string ultimosDos = id.substr(id.length() - 2);
        int terminacionId = std::stoi(ultimosDos);

        // Añadir la persona al grupo correcto dentro del mapa
        if (terminacionId <= 39) {
            grupos['A'].push_back(persona);
        } else if (terminacionId <= 79) {
            grupos['B'].push_back(persona);
        } else {
            grupos['C'].push_back(persona);
        }

    }
    return grupos;
}

std::map<std::string, std::vector<Persona>> agruparPersonasPorCiudad(const std::vector<Persona>& personas) {
    // El mapa guarda los resultados. 
    std::map<std::string, std::vector<Persona>> gruposc;

    for (const auto& persona : personas) {

        std::string ciudad = persona.getCiudadNacimiento();
        gruposc[ciudad].push_back(persona);

    }
    return gruposc;
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
                
            case 6: {
                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opcion 0 primero.\n";
                    break;
                }
                    // Llama a la función principal, pasando el vector por valor.
                    Persona::analizarLongevidad(*personas);

                    // patrimonio
                    // Llama a la función para mostrar la persona con mayor patrimonio
                    Persona::mostrarMayorPatrimonioPorValor(*personas);


                    //----------- DECLARANTES DE RENTA --------------
                    std::cout << "\nDeclarantes de renta - Valor.\n";
            
                    auto grupos = Persona::agruparPersonasPorCalendarioValor(*personas);
                    auto declarantes = Persona::DeclarantesPorGrupoValor(grupos);

                    for (const auto par : declarantes) {
                        std::cout << "--- Grupo " << par.first << " (ID termina en ";
                        if (par.first == 'A') std::cout << "00-39";
                        if (par.first == 'B') std::cout << "40-79";
                        if (par.first == 'C') std::cout << "80-99";
                        std::cout << ") -> Declarantes:  " << par.second.size() << "\n";
                        
                        if (par.second.empty()) {
                            std::cout << "No hay personas declarantes en este grupo.\n";
                        } else {
                            for (const auto declarante : par.second) {
                                declarante.mostrarResumen();
                                std::cout << std::endl;
                            }
                        }
                    }
                    std::cout << "\n";                
                    std::cout << "------------------------------------------------------\n";
                    // ------------------------------------------------------------
                    
                    break;
        }
        break;
            
            case 7: { 
                //_________________PATRIMONIO____________________yorPatrimonioPorReferencia(std::make_unique<std::vector<Persona>>(*personas));

                if (!personas || personas->empty()) {
                    std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
                    break;
                }
                
                std::cout << "\n--- Analisis de Longevidad (version por Referencia) ---\n";
                std::cout << "1. Persona mas longeva en todo el pais\n";
                std::cout << "2. Persona mas longeva por ciudad\n";
                
                int subp;
                std::cout << "Seleccione una opcion: ";
                std::cin >> subp;

                if (subp == 1) {
                    Persona masLongeva;
                    Persona::encontrarMasLongevoPais_ref(std::move(personas), masLongeva);
                    std::cout << "\nLa persona mas longeva del pais es: ";
                    masLongeva.mostrarResumen();
                    std::cout << " con edad: " << masLongeva.mostrarEdad() << " años.\n"; // Pasando la referencia
                } else if (subp == 2) {
                    Persona masLongeva;
                    Persona::mostrarMasLongevoPorCiudad_ref(std::move(personas), masLongeva);
                } else {
                    std::cout << "Opcion no valida.\n";

                //_________________PATRIMONIO____________________
                
                Persona::mostrarMayorPatrimonioPorReferencia(std::move(personas));
    }

                //---------------       //--------------------- DECLARANTES RENTA -----------------
                std::cout << "\nDeclarantes de renta - Referencia\n";
                std::map<char, std::vector<Persona>> grupos;
                Persona::agruparPersonasPorCalendarioRef(std::move(personas), grupos);
                std::map<char, std::vector<Persona>> declarantesRef; 
                Persona::DeclarantesPorGrupoReferencia(grupos, declarantesRef);

                for (const auto& par : declarantesRef) {
                    std::cout << "--- Grupo " << par.first << " (ID termina en ";
                    if (par.first == 'A') std::cout << "00-39";
                    if (par.first == 'B') std::cout << "40-79";
                    if (par.first == 'C') std::cout << "80-99";
                    std::cout << ") -> Declarantes: " << par.second.size() << "\n";
                    
                    if (par.second.empty()) {
                        std::cout << "No hay personas declarantes en este grupo.\n";
                    } else {
                        for (const auto& declarantePtr : par.second) {
                            declarantePtr.mostrarResumen();  // Acceso por puntero
                            std::cout << std::endl;
                        }
                    }
                }
                //--------------------------------------------------------------

                break;
            }
            
            
            case 8: { // Ranking de riqueza por agrupaciones
                menuAdicionales();
                std::cout << "\nSeleccione una opción: ";
                int opcionA;
                std::cin >> opcionA;

                switch (opcionA) {
                    case 1:{ // Riqueza por Agrupaciones

                    //----------- Ranking de Riqueza por Agrupaciones - Valor ------------
                    std::cout << "\n---- Ranking de riqueza por agrupaciones - Valor. ----\n";
            
                    auto grupos = Persona::agruparPersonasPorCalendarioValor(*personas);
                    auto ranking = Persona::RankingAgrupValor(*personas);

                    int posicion = 1; // Un contador para la posición en el ranking
                    for (const auto& stats : ranking) { // Recorremos el vector 'ranking'
                        // Imprimimos los datos de cada grupo
                        std::cout << "\n--- " << posicion << ". Grupo " << stats.idGrupo << " ---";
                        std::cout << "\nPatrimonio Promedio: $" << stats.patrimonioPromedio;
                        std::cout << "\nPatrimonio Total:     $" << static_cast<double>(stats.patrimonioTotal);
                        std::cout << "\nNúmero de Personas:   " << stats.numPersonas << "\n";
                        posicion++; // Incrementamos la posición para el siguiente grupo
                    }
                    std::cout << "\n";                
                    std::cout << "------------------------------------------------------\n";
                    // ------------------------------------------------------------

                        break;
                    }


                    case 2:{// Riqueza por Ciudades
                        //----------- Ranking de Riqueza por Ciudad - Valor ------------
                        std::cout << "\n---- Ranking de riqueza por Ciudades - Valor. ----\n";

                        auto rankingCiudades = Persona::RankingCiudadValor(*personas);
                        std::cout << "\n\n--- Ranking de riqueza por ciudad (promedio) - Valor ---\n";
                        std::cout << std::fixed << std::setprecision(2); // Formato para dinero

                        int ranking = 1;
                        for (const auto& stats : rankingCiudades) {
                            std::cout << "\n--- " << ranking << ". " << stats.nombreCiudad << " ---";
                            std::cout << "\nPatrimonio Promedio: $" << stats.patrimonioPromedio;
                            std::cout << "\nPatrimonio Total:     $" << static_cast<double>(stats.patrimonioTotal);
                            std::cout << "\nNúmero de Personas:   " << stats.numPersonas << "\n";
                            ranking++;
                        }
                        break;
                    }
                    case 3:{ // Promedio de edades por ciudad 
                    auto gruposCiudad = agruparPersonasPorCiudad(*personas);
                    for (const auto& par : gruposCiudad) {

                        const std::string& ciudad = par.first;

                        const std::vector<Persona>& personasEnCiudad = par.second;

                    if (personasEnCiudad.empty()) {
                        continue;
                    }

                    int count = 0;
                    for (size_t i = 0; i < personasEnCiudad.size(); ++i) {
                        count = count + personasEnCiudad[i].mostrarEdad();
                    }

                    double promedioEdad = static_cast<double>(count) / personasEnCiudad.size();
                    std::cout << ciudad << ": el promedio de edad es " << promedioEdad << " años.\n";
                }

                    }

                break;
                }
             }

            case 9:// Salir
                std::cout << "Saliendo...\n";
                break;     

            default:
                std::cout << "Opción inválida!\n";
        }
        
        // Mostrar estadísticas de la operación (excepto para opciones 4,5,6)
        if (opcion >= 0 && opcion <= 8) {
            double tiempo = monitor.detener_tiempo();
            long memoria = monitor.obtener_memoria() - memoria_inicio;
            monitor.mostrar_estadistica("Opción " + std::to_string(opcion), tiempo, memoria);
        }
        
    } while(opcion != 10);
    
    return 0;
    }
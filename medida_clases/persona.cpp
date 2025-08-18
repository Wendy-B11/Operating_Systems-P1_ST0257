#include "persona.h"
#include <iomanip> // Para std::setprecision // Para std::unique_ptr
#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <utility> // Para std::move
#include <memory> // Para std::unique_ptr
#include <algorithm> 

/**
 * Implementación del constructor de Persona.
 * 
 * POR QUÉ: Inicializar los miembros de la clase.
 * CÓMO: Usando la lista de inicialización y moviendo los strings para evitar copias.
 * PARA QUÉ: Eficiencia y correcta construcción del objeto.
 */


Persona::Persona(std::string nom, std::string ape, std::string id, 
                 std::string ciudad, std::string fecha, double ingresos, 
                 double patri, double deud, bool declara)
    : nombre(std::move(nom)), 
      apellido(std::move(ape)), 
      id(std::move(id)), 
      ciudadNacimiento(std::move(ciudad)), 
      ciudad(ciudad),                     
      fechaNacimiento(std::move(fecha)), 
      ingresosAnuales(ingresos), 
      patrimonio(patri),
      deudas(deud), 
      declaranteRenta(declara) {}

/**
 * Implementación de mostrar.
 * 
 * POR QUÉ: Mostrar todos los datos de la persona de forma estructurada.
 * CÓMO: Usando flujos de salida y formateadores.
 * PARA QUÉ: Facilitar la lectura de los datos completos de una persona.
 */

void Persona::menuPatrimonio() {
    std::cout << "\n\n=== MENÚ PATRIMONIO ===";
    std::cout << "\n1. En el país";
    std::cout << "\n2. En la ciudad";
    std::cout << "\n3. Por grupo (A/B/C)";
}

void Persona::mostrar() const {
    std::cout << "-------------------------------------\n";
    std::cout << "[" << id << "] Nombre: " << nombre << " " << apellido << "\n";
    std::cout << "   - Ciudad de nacimiento: " << ciudadNacimiento << "\n";
    std::cout << "   - Fecha de nacimiento: " << fechaNacimiento << "\n\n";
    std::cout << std::fixed << std::setprecision(2); // Formato de números
    std::cout << "   - Ingresos anuales: $" << ingresosAnuales << "\n";
    std::cout << "   - Patrimonio: $" << patrimonio << "\n";
    std::cout << "   - Deudas: $" << deudas << "\n";
    std::cout << "   - Declarante de renta: " << (declaranteRenta ? "Sí" : "No") << "\n";
}

/**
 * Implementación de mostrarResumen.
 * 
 * POR QUÉ: Mostrar información esencial de la persona en una sola línea.
 * CÓMO: Imprime ID, nombre completo, ciudad e ingresos.
 * PARA QUÉ: Listados rápidos y eficientes.
 */
void Persona::mostrarResumen() const {
    std::cout << "[" << id << "] " << nombre << " " << apellido
              << " | " << ciudadNacimiento 
              << " | $" << std::fixed << std::setprecision(2) << ingresosAnuales
              << " | " << fechaNacimiento;
}

int Persona::mostrarEdad() const {
    // Extrae el año de la fecha de nacimiento
    size_t pos = fechaNacimiento.find_last_of("/");
    std::string anioStr = fechaNacimiento.substr(pos + 1);
    int anioNacimiento = std::stoi(anioStr);
    int anioActual = 2025; 
    return anioActual - anioNacimiento;
}

std::map<std::string, std::vector<Persona>> Persona::agruparPersonasPorCiudadValor(const std::vector<Persona> personas) {
    // El mapa guarda los resultados. 
    std::map<std::string, std::vector<Persona>> gruposc;

    for (const auto& persona : personas) {

        std::string ciudad = persona.getCiudadNacimiento();
        gruposc[ciudad].push_back(persona);

    }
    return gruposc;
}

void Persona::agruparPersonasPorCiudad_ref(std::unique_ptr<std::vector<Persona>> personas, std::map<std::string, std::vector<Persona>>& gruposc) {
    if (!personas) return;

    for (const auto& persona : *personas) {
        std::string ciudad = persona.getCiudadNacimiento();
        gruposc[ciudad].push_back(persona);
    }
}

// Recibe una copia del vector de personas
Persona Persona::encontrarMasLongevoPaisValor(const std::vector<Persona> personas) {
    if (personas.empty()) {
        std::cout << "\nNo hay datos para analizar.\n";
        return Persona(); // Retorna un objeto Persona vacío
    }

    // Como 'personas' es una copia, trabajamos directamente con ella.
    Persona masLongeva = personas.at(0); // Asumimos que la primera es la más longeva

    // Iteramos para encontrar a la persona con la edad máxima
    for (size_t i = 1; i < personas.size(); ++i) {
        if (personas.at(i).mostrarEdad() > masLongeva.mostrarEdad()) {
            masLongeva = personas.at(i);
        }
    }

    return masLongeva;
}

// Recibe una copia del vector de personas
void Persona::mostrarMasLongevoPorCiudad(std::vector<Persona> personas) {
    if (personas.empty()) {
        std::cout << "\nNo hay datos para analizar.\n";
        return;
    }
    
    // Agrupamos las personas de la copia del vector
    auto gruposCiudad = Persona::agruparPersonasPorCiudadValor(personas);

    std::cout << "\n--- Persona más longeva por ciudad ---\n";

    for (const auto& par : gruposCiudad) {
        const std::string& ciudad = par.first;
        const std::vector<Persona>& personasEnCiudad = par.second;

        if (personasEnCiudad.empty()) {
            continue;
        }

        const Persona* masLongeva = &personasEnCiudad[0];
        
        for (size_t i = 1; i < personasEnCiudad.size(); ++i) {
            if (personasEnCiudad[i].mostrarEdad() > masLongeva->mostrarEdad()) {
                masLongeva = &personasEnCiudad[i];
            }
        }
        
        std::cout << ciudad << ": la persona mas longeva es ";
        masLongeva->mostrarResumen();
        std::cout << " con " << masLongeva->mostrarEdad() << " años.\n";
    }
}

// Recibe el vector original POR VALOR, creando una copia interna.
void Persona::analizarLongevidad(std::vector<Persona> personas) {
    // El menú de longevidad
    std::cout << "\n--- Analisis de Longevidad ---\n";
    std::cout << "1. Persona mas longeva en todo el pais\n";
    std::cout << "2. Persona mas longeva por ciudad\n";
    
    int subp;
    std::cout << "Seleccione una opcion: ";
    std::cin >> subp;

    if (subp == 1) {
        // Llama a la función de análisis nacional
        Persona::encontrarMasLongevoPaisValor(personas); // Pasando la copia
    } else if (subp == 2) {
        // Llama a la función de análisis por ciudad
        Persona::mostrarMasLongevoPorCiudad(personas); // Pasando la copia
    } else {
        std::cout << "Opcion no valida.\n";
    }
}

// Recibe una REFERENCIA CONSTANTE al vector original. No hay copia.
void Persona::encontrarMasLongevoPais_ref(std::unique_ptr<std::vector<Persona>> personas, Persona& masLongeva) {
    if (personas->empty()) {
        std::cout << "\nNo hay datos para analizar.\n";
        return;
    }

    masLongeva = personas->at(0); // Asumimos que la primera es la más longeva

    for (size_t i = 1; i < personas->size(); i++) {
        if (personas->at(i).mostrarEdad() > masLongeva.mostrarEdad()) {
            masLongeva = personas->at(i);
        }
    }
}

// Recibe una REFERENCIA CONSTANTE al vector.
void Persona::mostrarMasLongevoPorCiudad_ref(std::unique_ptr<std::vector<Persona>> personas, Persona& masLongeva) {
    // 1. Verificación de seguridad: Asegúrate de que el puntero no esté vacío.
    if (!personas || personas->empty()) {
        std::cout << "\nNo hay datos para analizar.\n";
        return;
    }

    // 2. Agrupa las personas por ciudad SIN MOVER el puntero.
    //    Simplemente leemos los datos que contiene.
    std::map<std::string, std::vector<Persona>> gruposCiudad;
    for (const auto& persona : *personas) { // Desreferencia con '*' para acceder al vector
        gruposCiudad[persona.getCiudadNacimiento()].push_back(persona);
    }
    // ¡Importante! 'personas' (el unique_ptr) sigue siendo válido aquí.

    std::cout << "\n--- Persona más longeva por ciudad (version por Referencia) ---\n";

    // 3. Itera sobre el mapa para encontrar y mostrar el resultado por ciudad.
    for (const auto& par : gruposCiudad) {
        const std::string& ciudad = par.first;
        const std::vector<Persona>& personasEnCiudad = par.second;

        if (personasEnCiudad.empty()) continue;

        // Encuentra al más longevo DENTRO de esa ciudad.
        const Persona* punteroMasLongeva = &personasEnCiudad[0]; // Usamos un puntero local
        for (size_t i = 1; i < personasEnCiudad.size(); i++) {
            if (personasEnCiudad[i].mostrarEdad() > punteroMasLongeva->mostrarEdad()) {
                punteroMasLongeva = &personasEnCiudad[i];
            }
        }
        
        std::cout << ciudad << ": la persona mas longeva es ";
        punteroMasLongeva->mostrarResumen();
        std::cout << " con " << punteroMasLongeva->mostrarEdad() << " anios.\n";
    }

    // Nota: La primera búsqueda que tenías y el parámetro 'masLongeva' no se usan
    // en esta lógica por ciudad, lo cual indica una posible redundancia en el diseño.
    // La función ahora hace lo que su nombre indica.
}


// Recibe una REFERENCIA CONSTANTE.
void Persona::analizarLongevidad_ref(std::unique_ptr<std::vector<Persona>> personas) {
    std::cout << "\n--- Analisis de Longevidad (version por Referencia) ---\n";
    std::cout << "1. Persona mas longeva en todo el pais\n";
    std::cout << "2. Persona mas longeva por ciudad\n";
    
    int subp;
    std::cout << "Seleccione una opcion: ";
    std::cin >> subp;
    Persona masLongeva;
    if (subp == 1) {
        Persona::encontrarMasLongevoPais_ref(std::move(personas), masLongeva); // Pasando la referencia
    } else if (subp == 2) {
        Persona::mostrarMasLongevoPorCiudad_ref(std::move(personas), masLongeva); // Pasando la referencia
    } else {
        std::cout << "Opcion no valida.\n";
    }
}

//juan pablo 
void Persona::mostrarMayorPatrimonioPorValor(std::vector<Persona> personas) {
    if (personas.empty()) {
        std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
        return;
    }
    Persona::menuPatrimonio();
    int subop;
    std::cout << "\nSeleccione una opción: ";
    std::cin >> subop;

    if (subop == 1) {
        // Mayor patrimonio en el país
        Persona mayor = personas.at(0);
        for (const auto& p : personas) {
            if (p.mostrarPatrimonio() > mayor.mostrarPatrimonio()) {
                mayor = p;
            }
        }
        std::cout << "Persona con mayor patrimonio en el país:\n";
        mayor.mostrarResumen();
        std::cout << " Patrimonio: " << mayor.mostrarPatrimonio() << "\n";
    } else if (subop == 2) {
        // Mostrar la persona con mayor patrimonio de cada ciudad
        auto gruposCiudad = Persona::agruparPersonasPorCiudadValor(personas);

        std::cout << "\n--- Persona con mayor patrimonio por ciudad ---\n";

        for (const auto& par : gruposCiudad) {
            const std::string& ciudad = par.first;
            const std::vector<Persona>& personasEnCiudad = par.second;

            if (personasEnCiudad.empty()) {
                continue;
            }

            Persona masRica = personasEnCiudad.at(0);
            for (size_t i = 1; i < personasEnCiudad.size(); ++i) {
                if (personasEnCiudad.at(i).getPatrimonio() > masRica.getPatrimonio()) {
                    masRica = personasEnCiudad.at(i);
                }
            }
            
            std::cout << ciudad << ": la persona con mayor patrimonio es ";
            masRica.mostrarResumen();
            std::cout << " con patrimonio: $" << masRica.getPatrimonio() << "\n";
        }
    } else if (subop == 3) {
        // Mostrar la persona con mayor patrimonio de cada grupo (A/B/C)
        auto gruposPorCalendario = Persona::agruparPersonasPorCalendarioValor(personas);

        std::cout << "\n--- Persona con mayor patrimonio por grupo ---\n";

        for (const auto& par : gruposPorCalendario) {
            char grupo = par.first;
            const std::vector<Persona>& personasEnGrupo = par.second;

            if (personasEnGrupo.empty()) {
                continue;
            }

            Persona masRica = personasEnGrupo[0];
            for (size_t i = 1; i < personasEnGrupo.size(); ++i) {
                if (personasEnGrupo[i].getPatrimonio() > masRica.getPatrimonio()) {
                    masRica = personasEnGrupo[i];
                }
            } 

            std::cout << "Grupo " << grupo << ": la persona con mayor patrimonio es ";
            masRica.mostrarResumen();
            std::cout << " con patrimonio: $" << masRica.getPatrimonio() << "\n";
        }
    }
}




std::map<char, std::vector<Persona>> Persona::agruparPersonasPorCalendarioValor(const std::vector<Persona>& personas) {
    std::map<char, std::vector<Persona>> grupos;
    for (const auto& persona : personas) {
        std::string id = persona.getId();
        std::string ultimosDos = id.substr(id.length() - 2);
        int terminacionId = std::stoi(ultimosDos);
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

void Persona::mostrarMayorPatrimonioPorReferencia(std::unique_ptr<std::vector<Persona>> personas) {
    if (!personas || personas->empty()) {
        std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
        return;
    }
    Persona::menuPatrimonio();
    int subop;
    std::cout << "\nSeleccione una opción: ";
    std::cin >> subop;

    if (subop == 1) {
        // Mayor patrimonio en el país
        Persona mayor = personas->at(0);
        for (const auto& p : *personas) {
            if (p.mostrarPatrimonio() > mayor.mostrarPatrimonio()) {
                mayor = p;
            }
        }
        std::cout << "Persona con mayor patrimonio en el país:\n";
        mayor.mostrarResumen();
        std::cout << " Patrimonio: " << mayor.mostrarPatrimonio() << "\n";
    } else if (subop == 2) {
        // Mostrar la persona con mayor patrimonio de cada ciudad
        std::map<std::string, std::vector<Persona>> gruposCiudad;
        Persona::agruparPersonasPorCiudad_ref(std::move(personas), gruposCiudad);

        std::cout << "\n--- Persona con mayor patrimonio por ciudad ---\n";

        for (const auto& par : gruposCiudad) {
            const std::string& ciudad = par.first;
            const std::vector<Persona>& personasEnCiudad = par.second;

            if (personasEnCiudad.empty()) {
                continue;
            }

            Persona masRica = personasEnCiudad.at(0);
            for (size_t i = 1; i < personasEnCiudad.size(); ++i) {
                if (personasEnCiudad.at(i).getPatrimonio() > masRica.getPatrimonio()) {
                    masRica = personasEnCiudad.at(i);
                }
            }
            
            std::cout << ciudad << ": la persona con mayor patrimonio es ";
            masRica.mostrarResumen();
            std::cout << " con patrimonio: $" << masRica.getPatrimonio() << "\n";
        }
    } else if (subop == 3) {
        // Mostrar la persona con mayor patrimonio de cada grupo (A/B/C)
        std::map<char, std::vector<Persona>> gruposPorCalendario;
        Persona::agruparPersonasPorCalendarioRef(std::move(personas), gruposPorCalendario);

        std::cout << "\n--- Persona con mayor patrimonio por grupo ---\n";

        for (const auto& par : gruposPorCalendario) {
            char grupo = par.first;
            const std::vector<Persona>& personasEnGrupo = par.second;

            if (personasEnGrupo.empty()) {
                continue;
            }

            Persona masRica = personasEnGrupo[0];
            for (size_t i = 1; i < personasEnGrupo.size(); ++i) {
                if (personasEnGrupo[i].getPatrimonio() > masRica.getPatrimonio()) {
                    masRica = personasEnGrupo[i];
                }
            } 

            std::cout << "Grupo " << grupo << ": la persona con mayor patrimonio es ";
            masRica.mostrarResumen();
            std::cout << " con patrimonio: $" << masRica.getPatrimonio() << "\n";
        }
    }
}

// arriba es de jotape 

void Persona::agruparPersonasPorCalendarioRef(std::unique_ptr<std::vector<Persona>> personas, std::map<char, std::vector<Persona>>& grupos) {
    grupos.clear(); // Limpiar el mapa antes de llenarlo
    for (const auto& persona : *personas) {
        const std::string& id = persona.getId();
        std::string ultimosDos = id.substr(id.length() - 2);
        int terminacionId = std::stoi(ultimosDos);
        if (terminacionId <= 39) {
            grupos['A'].push_back(persona);
        } else if (terminacionId <= 79) {
            grupos['B'].push_back(persona);
        } else {
            grupos['C'].push_back(persona);
        }
    }
}

std::map<char, std::vector<Persona>> Persona::DeclarantesPorGrupoValor(std::map<char, std::vector<Persona>> grupos) {

    std::map<char, std::vector<Persona>> grupoDeclarantes;

    std::cout << "\n- Declarantes de renta por grupo (Procesado por VALOR) -\n";
    for (const auto par : grupos) {
        char nombreGrupo = par.first;
        const std::vector<Persona> personasDelGrupo = par.second;

        std::vector<Persona> declarantes;
        for (const auto persona : personasDelGrupo) {
            if (persona.getDeclaranteRenta()) {
                declarantes.push_back(persona);
            }
        }

        grupoDeclarantes[nombreGrupo] = declarantes;
    }

    return grupoDeclarantes;
}


void Persona::DeclarantesPorGrupoReferencia(std::map<char, std::vector<Persona>>& grupos, std::map<char, std::vector<Persona>>& grupoDeclarantes) {
    std::cout << "\n- Declarantes de renta por grupo (Procesado por Referencia) -\n";
    for (const auto &par : grupos) {
        char nombreGrupo = par.first;
        const std::vector<Persona>& personasDelGrupo = par.second;
        std::vector<Persona> declarantes;
        for (const auto &persona : personasDelGrupo) {
            if (persona.getDeclaranteRenta()) {
                declarantes.push_back(persona);
            }
        }
        grupoDeclarantes[nombreGrupo] = declarantes;
    }
}

std::vector<Persona::EstadisticasGrupo> Persona::RankingAgrupValor(std::vector<Persona> personas) {
    std::map<char, std::vector<Persona>> gruposClasificados = Persona::agruparPersonasPorCalendarioValor(personas);
    std::vector<Persona::EstadisticasGrupo> estadisticas;
    for (const auto& par : gruposClasificados) {
        Persona::EstadisticasGrupo stats;
        stats.idGrupo = par.first;
        stats.numPersonas = par.second.size();
        if (stats.numPersonas > 0) {
            for (const auto& persona : par.second) {
                stats.patrimonioTotal += persona.getPatrimonio();
            }
            stats.patrimonioPromedio = static_cast<double>(stats.patrimonioTotal) / stats.numPersonas;
        }
        estadisticas.push_back(stats);
    }
    std::sort(estadisticas.begin(), estadisticas.end(),
              [](const Persona::EstadisticasGrupo& a, const Persona::EstadisticasGrupo& b) {
                  return a.patrimonioPromedio > b.patrimonioPromedio;
              });
    return estadisticas;
}

std::vector<Persona::EstadisticasCiudad> Persona::RankingCiudadValor(std::vector<Persona> personas) {
    auto gruposPorCiudad = Persona::agruparPersonasPorCiudadValor(personas);
    std::vector<Persona::EstadisticasCiudad> estadisticas;
    for (const auto& par : gruposPorCiudad) {
        Persona::EstadisticasCiudad stats;
        stats.nombreCiudad = par.first;
        stats.numPersonas = par.second.size();

        if (stats.numPersonas > 0) {
            for (const auto& persona : par.second) {
                stats.patrimonioTotal += persona.getPatrimonio();
            }
            stats.patrimonioPromedio = static_cast<double>(stats.patrimonioTotal) / stats.numPersonas;
        }
        estadisticas.push_back(stats);
    }
    std::sort(estadisticas.begin(), estadisticas.end(),
              [](const Persona::EstadisticasCiudad& a, const Persona::EstadisticasCiudad& b) {
                  return a.patrimonioPromedio > b.patrimonioPromedio;
              });
    return estadisticas;
}

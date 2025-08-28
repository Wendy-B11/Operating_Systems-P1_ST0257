#include "persona.h"
#include <iomanip> // Para std::setprecision
#include <algorithm>
#include <vector>
#include <map>
#include <tuple> 
#include <vector>
#include <map>
#include <memory>


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
              << " | edad: " << calcularEdad();
}

//Implementacion de calcular edad
int Persona::calcularEdad() const{
    size_t pos = fechaNacimiento.find_last_of("/");
    std::string año = fechaNacimiento.substr(pos + 1);
    int añoNacimiento = std::stoi(año);
    int añoActual = 2025; 
    return añoActual - añoNacimiento;
}

//Implementacion de edad mas longeva del pais
Persona Persona::edadMasLongevaPais(const std::vector<Persona> personas){
    Persona vieja = personas[0];
    for (size_t i = 1; i < personas.size(); ++i) {
        if (personas[i].calcularEdad() > vieja.calcularEdad()) {
            vieja = personas[i];
        }
    }
    return vieja;
}

//Implementacion de edad mas longeva del pais con referencias
void Persona::edadMasLongevaPaisRef(const std::vector<Persona>& personas){
    const Persona* pPersonaMasVieja = &personas[0];

    for (const Persona& personaActual : personas) {
        if (personaActual.calcularEdad() > pPersonaMasVieja->calcularEdad()) {
                        pPersonaMasVieja = &personaActual;
        }
    }

    std::cout << "La persona más longeva es ";
    pPersonaMasVieja->mostrarResumen(); 
}

//Implementacion de edad mas longeva por ciudad con valores
std::vector<Persona> Persona::edadMasLongevaCiudad(const std::vector<Persona> personas) {
    std::map<std::string, Persona> ciudadMasVieja;
    std::vector<Persona> resultado;

    auto gruposc = Persona::agruparCiudad(personas);

    for (const auto& par : gruposc) {
        const std::string& ciudad = par.first;
        const std::vector<Persona>& personasCiudad = par.second;

        if (personasCiudad.empty()) {
            continue;
        }

        Persona masLongeva = personasCiudad[0];

        for (size_t i = 1; i < personasCiudad.size(); ++i) {
            if (personasCiudad[i].calcularEdad() > masLongeva.calcularEdad()) {
                masLongeva = personasCiudad[i];
            }
        }
        resultado.push_back(masLongeva);
    }
    return resultado;
}

//Implementacion de edad mas lonegeva por ciudad utilizando referencias
#include <vector>
#include <string>
#include <map>

// ... (definición de la clase Persona)

void Persona::edadMasLongevaCiudadRef(const std::vector<Persona>& personas) {

    std::map<std::string, std::vector<Persona>> ciudad;
    agruparCiudadRef(personas, ciudad);

    for (const auto& par : ciudad) {
        const std::string& ciudad = par.first; 
        const std::vector<Persona>& personasCiudad = par.second;

        if (personasCiudad.empty()) {
            continue;
        }

        const Persona* pMasLongeva = &personasCiudad[0];

        for (const Persona& personaActual : personasCiudad) {
            if (personaActual.calcularEdad() > pMasLongeva->calcularEdad()) {
                pMasLongeva = &personaActual;
            }
        }
        
        pMasLongeva->mostrarResumen();
        std::cout << "\n";
    }
}
    
// Agrupar por calendario - Por Valor
std::map<std::string, std::vector<Persona>> Persona::agruparCalendario(const std::vector<Persona> personas){
    std::map<std::string, std::vector<Persona>> calendario;
    for (const Persona& persona : personas){
        std::string ultimosDos = (persona.id).substr((persona.id).length() - 2, 2);
        int ultimosDosInt = std::stoi(ultimosDos);
        if (ultimosDosInt >= 0 && ultimosDosInt <= 39){
            calendario["A"].push_back(persona);
        } else if (ultimosDosInt >= 40 && ultimosDosInt <= 79){
            calendario["B"].push_back(persona);
        } else if (ultimosDosInt >= 80 && ultimosDosInt <= 99){
            calendario["C"].push_back(persona); 
        }
    }
    return calendario;
}

// Agrupar por calendario - Por Referencia
void Persona::agruparCalendarioRef(const std::vector<Persona>& personas, std::map<std::string, std::vector<Persona>>& calendario){
    for (const Persona& persona : personas){
        std::string ultimosDos = (persona.id).substr((persona.id).length() - 2, 2);
        int ultimosDosInt = std::stoi(ultimosDos);
        if (ultimosDosInt >= 0 && ultimosDosInt <= 39){
            calendario["A"].push_back(persona);
        } else if (ultimosDosInt >= 40 && ultimosDosInt <= 79){
            calendario["B"].push_back(persona);
        } else if (ultimosDosInt >= 80 && ultimosDosInt <= 99){
            calendario["C"].push_back(persona); 
        }
    }
}

// Agrupar por ciudad - Por Valor
std::map<std::string, std::vector<Persona>> Persona::agruparCiudad(const std::vector<Persona> personas){
    std::map<std::string, std::vector<Persona>> ciudad;
    for (const Persona& persona : personas){
        ciudad[persona.getCiudadNacimiento()].push_back(persona);
    }
    return ciudad;
}

// Agrupar por ciudad - Por Referencia
void Persona::agruparCiudadRef(const std::vector<Persona>& personas, std::map<std::string, std::vector<Persona>>& ciudad){
    for (const Persona& persona : personas){
        ciudad[persona.getCiudadNacimiento()].push_back(persona);
    }
} 

// --- FUNCIONES DE PATRIMONIO ---

void Persona::mostrarMayorPatrimonioPorValor(std::vector<Persona> personas) {
    if (personas.empty()) {
        std::cout << "\nNo hay datos disponibles. Use opción 0 primero.\n";
        return;
    }
    std::cout << "\n--- Menú Patrimonio ---\n";
    std::cout << "1. Mayor patrimonio en el país\n";
    std::cout << "2. Mayor patrimonio por ciudad\n";
    std::cout << "3. Mayor patrimonio por grupo (A/B/C)\n";
    int subop;
    std::cout << "\nSeleccione una opción: ";
    std::cin >> subop;

    if (subop == 1) {
        // Mayor patrimonio en el país
        Persona mayor = personas.at(0);
        for (const auto& p : personas) {
            if (p.getPatrimonio() > mayor.getPatrimonio()) {
                mayor = p;
            }
        }
        std::cout << "Persona con mayor patrimonio en el país:\n";
        mayor.mostrarResumen();
        std::cout << " Patrimonio: " << mayor.getPatrimonio() << "\n";
    } else if (subop == 2) {
        // Mostrar la persona con mayor patrimonio de cada ciudad
        auto gruposCiudad = Persona::agruparCiudad(personas);

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
    std::cout << "\n--- Menú Patrimonio ---\n";
    std::cout << "1. Mayor patrimonio en el país\n";
    std::cout << "2. Mayor patrimonio por ciudad\n";
    std::cout << "3. Mayor patrimonio por grupo (A/B/C)\n";
    int subop;
    std::cout << "\nSeleccione una opción: ";
    std::cin >> subop;

    if (subop == 1) {
        // Mayor patrimonio en el país
        Persona mayor = personas->at(0);
        for (const auto& p : *personas) {
            if (p.getPatrimonio() > mayor.getPatrimonio()) {
                mayor = p;
            }
        }
        std::cout << "Persona con mayor patrimonio en el país:\n";
        mayor.mostrarResumen();
        std::cout << " Patrimonio: " << mayor.getPatrimonio() << "\n";
    } else if (subop == 2) {
        // Mostrar la persona con mayor patrimonio de cada ciudad
        std::map<std::string, std::vector<Persona>> gruposCiudad;
        Persona::agruparCiudadRef(*personas, gruposCiudad);

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
        std::map<char, std::vector<Persona>> gruposPorCalendario = Persona::agruparPersonasPorCalendarioValor(*personas);

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


// Declarantes de renta - Por Valor
std::map<std::string, std::vector<Persona>> Persona::declarantesRenta(std::map<std::string, std::vector<Persona>> personas){
    std::map<std::string, std::vector<Persona>> declarantes;
    for (const auto& grupo : personas){
        for (const Persona& persona : grupo.second){
            if (persona.getDeclaranteRenta()){
                declarantes[grupo.first].push_back(persona);
            }
        }
    }
    return declarantes;
}

//Declarantes de renta - Por Referencia
void Persona::declarantesRentaRef(const std::map<std::string, std::vector<Persona>>& personas, std::map<std::string, std::vector<Persona>>& declarantes){
    for (const auto& grupo : personas){
        for (const Persona& persona : grupo.second){
            if (persona.getDeclaranteRenta()){
                declarantes[grupo.first].push_back(persona);
            }
        }
    }
}

// Ranking de riqueza por agrupación - Por valor
std::vector<std::pair<std::string, double>> Persona::rankingRiqueza(const std::map<std::string, std::vector<Persona>> calendario) {
    // Calculamos la suma de ingresos para cada grupo 
    std::map<std::string, double> sumasPorCalendario;
    for (const auto& par : calendario) { 
        const std::string grupo = par.first;
        const std::vector<Persona> personasDelGrupo = par.second;
        double sumaTotalGrupo = 0.0;
        for (const Persona& persona : personasDelGrupo) { 
            sumaTotalGrupo += persona.getIngresosAnuales();
        }
        sumasPorCalendario[grupo] = sumaTotalGrupo;
    }
    // Map de sumas a un vector de pares para poder ordenarlo
    std::vector<std::pair<std::string, double>> ranking(sumasPorCalendario.begin(), sumasPorCalendario.end());
    // Ordenar el vector de forma descendente basado en los ingresos
    std::sort(ranking.begin(), ranking.end(), [](const std::pair<std::string, double> a, const std::pair<std::string, double> b) {
        return a.second > b.second; 
    });
    return ranking;
}

// Ranking de riqueza por agrupación - Por referencia
void Persona::rankingRiquezaRef(const std::map<std::string, std::vector<Persona>>& calendario, std::vector<std::pair<std::string, double>>& ranking) {
    // Calculamos la suma de ingresos para cada grupo 
    std::map<std::string, double> sumasPorCalendario;
    for (const auto& par : calendario) { 
        const std::string grupo = par.first;
        const std::vector<Persona> personasDelGrupo = par.second;
        double sumaTotalGrupo = 0.0;
        for (const Persona& persona : personasDelGrupo) { 
            sumaTotalGrupo += persona.getIngresosAnuales();
        }
        sumasPorCalendario[grupo] = sumaTotalGrupo;
    }
    // Map de sumas a un vector de pares para poder ordenarlo
    ranking.assign(sumasPorCalendario.begin(), sumasPorCalendario.end());
    // Ordenar el vector de forma descendente basado en los ingresos
    std::sort(ranking.begin(), ranking.end(), [](const std::pair<std::string, double> a, const std::pair<std::string, double> b) {
        return a.second > b.second; 
    });
}

// Ranking de riqueza por ciudad - Por Valor
std::vector<std::pair<std::string, double>> Persona::rankingRiquezaCiudad(const std::map<std::string, std::vector<Persona>> ciudad) {
    // Calculamos la suma de ingresos para cada grupo 
    std::map<std::string, double> sumasPorCiudad;
    for (const auto& par : ciudad) { 
        const std::string grupo = par.first;
        const std::vector<Persona> personasDelGrupo = par.second;
        double sumaTotalGrupo = 0.0;
        for (const Persona& persona : personasDelGrupo) { 
            sumaTotalGrupo += persona.getIngresosAnuales();
        }
        sumasPorCiudad[grupo] = sumaTotalGrupo;
    }
    // Map de sumas a un vector de pares para poder ordenarlo
    std::vector<std::pair<std::string, double>> ranking(sumasPorCiudad.begin(), sumasPorCiudad.end());
    // Ordenar el vector de forma descendente basado en los ingresos
    std::sort(ranking.begin(), ranking.end(), [](const std::pair<std::string, double> a, const std::pair<std::string, double> b) {
        return a.second > b.second; 
    });
    return ranking;
}

// Ranking de riqueza por ciudad - Por Referencia
void Persona::rankingRiquezaCiudadRef(const std::map<std::string, std::vector<Persona>>& ciudad, std::vector<std::pair<std::string, double>>& ranking) {
    // Calculamos la suma de ingresos para cada grupo 
    std::map<std::string, double> sumasPorCiudad;
    for (const auto& par : ciudad) { 
        const std::string grupo = par.first;
        const std::vector<Persona> personasDelGrupo = par.second;
        double sumaTotalGrupo = 0.0;
        for (const Persona& persona : personasDelGrupo) { 
            sumaTotalGrupo += persona.getIngresosAnuales();
        }
        sumasPorCiudad[grupo] = sumaTotalGrupo;
    }
    // Map de sumas a un vector de pares para poder ordenarlo
    ranking.assign(sumasPorCiudad.begin(), sumasPorCiudad.end());
    // Ordenar el vector de forma descendente basado en los ingresos
    std::sort(ranking.begin(), ranking.end(), [](const std::pair<std::string, double> a, const std::pair<std::string, double> b) {
        return a.second > b.second; 
    });
}
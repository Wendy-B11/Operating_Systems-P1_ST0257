#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <memory>

// Estructura que representa una persona con datos personales y fiscales
struct Persona {
    // Datos básicos de identificación
    std::string nombre;           // Nombre de pila
    std::string apellido;         // Apellidos
    std::string id;               // Identificador único
    std::string ciudadNacimiento; // Ciudad de nacimiento en Colombia
    std::string fechaNacimiento;  // Fecha en formato DD/MM/AAAA
    
    // Datos fiscales y económicos
    double ingresosAnuales;       // Ingresos anuales en pesos colombianos
    double patrimonio;            // Valor total de bienes y activos
    double deudas;                // Deudas pendientes
    bool declaranteRenta;         // Si está obligado a declarar renta

    // --- Métodos de visualización ---
    void mostrar() const;         // Muestra todos los detalles completos
    void mostrarResumen() const;  // Muestra versión compacta para listados
};

// Implementación de métodos inline para mantener la estructura simple
inline void Persona::mostrar() const {
    std::cout << "-------------------------------------\n";
    std::cout << "[" << id << "] Nombre: " << nombre << " " << apellido << "\n";
    std::cout << "   - Ciudad de nacimiento: " << ciudadNacimiento << "\n";
    std::cout << "   - Fecha de nacimiento: " << fechaNacimiento << "\n\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "   - Ingresos anuales: $" << ingresosAnuales << "\n";
    std::cout << "   - Patrimonio: $" << patrimonio << "\n";
    std::cout << "   - Deudas: $" << deudas << "\n";
    std::cout << "   - Declarante de renta: " << (declaranteRenta ? "Sí" : "No") << "\n";
}

inline void Persona::mostrarResumen() const {
    std::cout << "[" << id << "] " << nombre << " " << apellido
              << " | " << ciudadNacimiento 
              << " | $" << std::fixed << std::setprecision(2) << ingresosAnuales;
}

// Agrupar por ciudad - Por Valor
inline std::map<std::string, std::vector<Persona>> agruparCiudad(const std::vector<Persona> personas){
    std::map<std::string, std::vector<Persona>> ciudad;
    for (const Persona& persona : personas){
        ciudad[persona.ciudadNacimiento].push_back(persona);
    }
    return ciudad;
}

// Agrupar por ciudad - Por Referencia
inline void agruparCiudadRef(const std::vector<Persona>& personas, std::map<std::string, std::vector<Persona>>& ciudad){
    for (const Persona& persona : personas){
        ciudad[persona.ciudadNacimiento].push_back(persona);
    }
}   

// Agrupar por calendario - Por Valor
inline std::map<std::string, std::vector<Persona>> agruparCalendario(const std::vector<Persona> personas){
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
inline void agruparCalendarioRef(const std::vector<Persona>& personas, std::map<std::string, std::vector<Persona>>& calendario){
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

// Declarantes de renta - Por Valor
inline std::map<std::string, std::vector<Persona>> declarantesRenta(std::map<std::string, std::vector<Persona>> personas){
    std::map<std::string, std::vector<Persona>> declarantes;
    for (const auto& grupo : personas){
        for (const Persona& persona : grupo.second){
            if (persona.declaranteRenta){
                declarantes[grupo.first].push_back(persona);
            }
        }
    }
    return declarantes;
}

//Declarantes de renta - Por Referencia
inline void declarantesRentaRef(const std::map<std::string, std::vector<Persona>>& personas, std::map<std::string, std::vector<Persona>>& declarantes){
    for (const auto& grupo : personas){
        for (const Persona& persona : grupo.second){
            if (persona.declaranteRenta){
                declarantes[grupo.first].push_back(persona);
            }
        }
    }
}

// Ranking de riqueza por ciudad - Por Valor
inline std::vector<std::pair<std::string, double>> rankingRiquezaCiudad(const std::map<std::string, std::vector<Persona>> ciudad) {
    // Calculamos la suma de ingresos para cada grupo 
    std::map<std::string, double> sumasPorCiudad;
    for (const auto& par : ciudad) { 
        const std::string grupo = par.first;
        const std::vector<Persona> personasDelGrupo = par.second;
        double sumaTotalGrupo = 0.0;
        for (const Persona& persona : personasDelGrupo) { 
            sumaTotalGrupo += persona.ingresosAnuales;
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
inline void rankingRiquezaCiudadRef(const std::map<std::string, std::vector<Persona>>& ciudad, std::vector<std::pair<std::string, double>>& ranking) {
    // Calculamos la suma de ingresos para cada grupo 
    std::map<std::string, double> sumasPorCiudad;
    for (const auto& par : ciudad) { 
        const std::string grupo = par.first;
        const std::vector<Persona> personasDelGrupo = par.second;
        double sumaTotalGrupo = 0.0;
        for (const Persona& persona : personasDelGrupo) { 
            sumaTotalGrupo += persona.ingresosAnuales;
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

// Ranking de riqueza por agrupación - Por valor
inline std::vector<std::pair<std::string, double>> rankingRiqueza(const std::map<std::string, std::vector<Persona>> calendario) {
    // Calculamos la suma de ingresos para cada grupo 
    std::map<std::string, double> sumasPorCalendario;
    for (const auto& par : calendario) { 
        const std::string grupo = par.first;
        const std::vector<Persona> personasDelGrupo = par.second;
        double sumaTotalGrupo = 0.0;
        for (const Persona& persona : personasDelGrupo) { 
            sumaTotalGrupo += persona.ingresosAnuales;
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
inline void rankingRiquezaRef(const std::map<std::string, std::vector<Persona>>& calendario, std::vector<std::pair<std::string, double>>& ranking) {
    // Calculamos la suma de ingresos para cada grupo 
    std::map<std::string, double> sumasPorCalendario;
    for (const auto& par : calendario) { 
        const std::string grupo = par.first;
        const std::vector<Persona> personasDelGrupo = par.second;
        double sumaTotalGrupo = 0.0;
        for (const Persona& persona : personasDelGrupo) { 
            sumaTotalGrupo += persona.ingresosAnuales;
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


// Mayor patrimonio - Por Valor
inline void mostrarMayorPatrimonioPorValor(std::vector<Persona> personas) {
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
        Persona mayor = personas.at(0);
        for (const auto& p : personas) {
            if (p.patrimonio > mayor.patrimonio) {
                mayor = p;
            }
        }
        std::cout << "Persona con mayor patrimonio en el país:\n";
        mayor.mostrarResumen();
        std::cout << " Patrimonio: " << mayor.patrimonio << "\n";
    } else if (subop == 2) {
        auto gruposCiudad = agruparCiudad(personas);
        std::cout << "\n--- Persona con mayor patrimonio por ciudad ---\n";
        for (const auto& par : gruposCiudad) {
            const std::string& ciudad = par.first;
            const std::vector<Persona>& personasEnCiudad = par.second;
            if (personasEnCiudad.empty()) continue;
            Persona masRica = personasEnCiudad.at(0);
            for (size_t i = 1; i < personasEnCiudad.size(); ++i) {
                if (personasEnCiudad.at(i).patrimonio > masRica.patrimonio) {
                    masRica = personasEnCiudad.at(i);
                }
            }
            std::cout << ciudad << ": la persona con mayor patrimonio es ";
            masRica.mostrarResumen();
            std::cout << " con patrimonio: $" << masRica.patrimonio << "\n";
        }
    } else if (subop == 3) {
        auto gruposPorCalendario = agruparCalendario(personas);
        std::cout << "\n--- Persona con mayor patrimonio por grupo ---\n";
        for (const auto& par : gruposPorCalendario) {
            const std::string& grupo = par.first;
            const std::vector<Persona>& personasEnGrupo = par.second;
            if (personasEnGrupo.empty()) continue;
            Persona masRica = personasEnGrupo[0];
            for (size_t i = 1; i < personasEnGrupo.size(); ++i) {
                if (personasEnGrupo[i].patrimonio > masRica.patrimonio) {
                    masRica = personasEnGrupo[i];
                }
            }
            std::cout << "Grupo " << grupo << ": la persona con mayor patrimonio es ";
            masRica.mostrarResumen();
            std::cout << " con patrimonio: $" << masRica.patrimonio << "\n";
        }
    }
}

// Mayor patrimonio - Por Referencia
inline void mostrarMayorPatrimonioPorReferencia(std::unique_ptr<std::vector<Persona>> personas) {
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
        Persona mayor = personas->at(0);
        for (const auto& p : *personas) {
            if (p.patrimonio > mayor.patrimonio) {
                mayor = p;
            }
        }
        std::cout << "Persona con mayor patrimonio en el país:\n";
        mayor.mostrarResumen();
        std::cout << " Patrimonio: " << mayor.patrimonio << "\n";
    } else if (subop == 2) {
        std::map<std::string, std::vector<Persona>> gruposCiudad;
        agruparCiudadRef(*personas, gruposCiudad);
        std::cout << "\n--- Persona con mayor patrimonio por ciudad ---\n";
        for (const auto& par : gruposCiudad) {
            const std::string& ciudad = par.first;
            const std::vector<Persona>& personasEnCiudad = par.second;
            if (personasEnCiudad.empty()) continue;
            Persona masRica = personasEnCiudad.at(0);
            for (size_t i = 1; i < personasEnCiudad.size(); ++i) {
                if (personasEnCiudad.at(i).patrimonio > masRica.patrimonio) {
                    masRica = personasEnCiudad.at(i);
                }
            }
            std::cout << ciudad << ": la persona con mayor patrimonio es ";
            masRica.mostrarResumen();
            std::cout << " con patrimonio: $" << masRica.patrimonio << "\n";
        }
    } else if (subop == 3) {
        std::map<std::string, std::vector<Persona>> gruposPorCalendario;
        agruparCalendarioRef(*personas, gruposPorCalendario);
        std::cout << "\n--- Persona con mayor patrimonio por grupo ---\n";
        for (const auto& par : gruposPorCalendario) {
            const std::string& grupo = par.first;
            const std::vector<Persona>& personasEnGrupo = par.second;
            if (personasEnGrupo.empty()) continue;
            Persona masRica = personasEnGrupo[0];
            for (size_t i = 1; i < personasEnGrupo.size(); ++i) {
                if (personasEnGrupo[i].patrimonio > masRica.patrimonio) {
                    masRica = personasEnGrupo[i];
                }
            }
            std::cout << "Grupo " << grupo << ": la persona con mayor patrimonio es ";
            masRica.mostrarResumen();
            std::cout << " con patrimonio: $" << masRica.patrimonio << "\n";
        }
    }
}

#endif // PERSONA_H
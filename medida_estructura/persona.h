#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>

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



#endif // PERSONA_H
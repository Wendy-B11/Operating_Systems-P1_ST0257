#include "persona.h"
#include <iomanip> // Para std::setprecision
#include <algorithm>
#include <vector>
#include <map>

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

//Implementacion de agrupar personas por ciudad
std::map<std::string, std::vector<Persona>> Persona::agruparPersonasPorCiudad(const std::vector<Persona> personas) { 
    std::map<std::string, std::vector<Persona>> gruposc;

    for (const auto& persona : personas) {

        std::string ciudad = persona.getCiudadNacimiento();
        gruposc[ciudad].push_back(persona);

    }
    return gruposc;
}

std::vector<Persona> Persona::edadMasLongevaCiudad(const std::vector<Persona> personas) {
    std::map<std::string, Persona> ciudadMasVieja;
    std::vector<Persona> resultado;

    auto gruposc = Persona::agruparPersonasPorCiudad(personas);

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


#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>

/**
 * Clase que representa una persona con datos personales y financieros.
 * 
 * POR QUÉ: Para modelar una entidad persona con atributos relevantes para el sistema.
 * CÓMO: Mediante una clase con atributos privados y métodos públicos de acceso y visualización.
 * PARA QUÉ: Centralizar y encapsular la información de una persona, garantizando integridad de datos.
 */
class Persona {
private:
    std::string nombre;           // Nombre de pila
    std::string apellido;         // Apellidos
    std::string id;               // Identificador único (cédula)
    std::string ciudadNacimiento; // Ciudad de nacimiento
    std::string fechaNacimiento;  // Fecha de nacimiento en formato DD/MM/AAAA
    double ingresosAnuales;       // Ingresos anuales en pesos colombianos
    double patrimonio;            // Patrimonio total (activos)
    double deudas;                // Deudas totales (pasivos)
    bool declaranteRenta;         // Si es declarante de renta

public:
    /**
     * Constructor para inicializar todos los atributos de la persona.
     * 
     * POR QUÉ: Necesidad de crear instancias de Persona con todos sus datos.
     * CÓMO: Recibe cada atributo por valor y los mueve a los miembros correspondientes.
     * PARA QUÉ: Construir objetos Persona completos y válidos.
     */
    Persona(); // Constructor por defecto

    Persona(std::string nom, std::string ape, std::string id, 
            std::string ciudad, std::string fecha, double ingresos, 
            double patri, double deud, bool declara);
    
    // Métodos de acceso (getters) - Implementados inline para eficiencia
    std::string getNombre() const { return nombre; }
    std::string getApellido() const { return apellido; }
    std::string getId() const { return id; }
    std::string getCiudadNacimiento() const { return ciudadNacimiento; }
    std::string getFechaNacimiento() const { return fechaNacimiento; }
    double getIngresosAnuales() const { return ingresosAnuales; }
    double getPatrimonio() const { return patrimonio; }
    double getDeudas() const { return deudas; }
    bool getDeclaranteRenta() const { return declaranteRenta; }

    /**
     * Muestra toda la información de la persona de forma detallada.
     * 
     * POR QUÉ: Para inspeccionar todos los datos de una persona en formato legible.
     * CÓMO: Imprime en la salida estándar con formato.
     * PARA QUÉ: Depuración y visualización completa de un registro.
     */
    void mostrar() const;
    
    /**
     * Muestra un resumen de la persona (ID, nombre, ciudad, ingresos).
     * 
     * POR QUÉ: Para listar personas de forma compacta en listados.
     * CÓMO: Imprime solo los datos más relevantes.
     * PARA QUÉ: Visualización eficiente en colecciones grandes.
     */
    void mostrarResumen() const;

    // Calcula la edad de una persona
    int calcularEdad() const;

    // Muestra la edad de la persona mas longeva por pais utilizando valores
    static Persona edadMasLongevaPais(const std::vector<Persona> personas);

    // Muesta la edad de la persona mas longeva por pais utilizando apuntadores
    static void edadMasLongevaPaisRef(const std::vector<Persona>& personas);

    // Agrupa personas por ciudad de nacimiento
    static std::map<std::string, std::vector<Persona>> agruparPersonasPorCiudad(const std::vector<Persona> personas);

    // Muestra la edad de la persona mas longeva por ciudad utilizando valores
    static std::vector<Persona> edadMasLongevaCiudad(const std::vector<Persona> personas);

    // Muestra la edad de la persona mas longeva por ciudad utilizando apuntadores
    static void edadMasLongevaCiudadRef(const std::vector<Persona>& personas);
};

#endif // PERSONA_H
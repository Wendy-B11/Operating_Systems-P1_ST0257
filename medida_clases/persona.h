#ifndef PERSONA_H
#define PERSONA_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <memory> // Para std::unique_ptr

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
    std::string ciudad;
    char grupo;

public:

    Persona() = default; // Constructor por defecto
    /**
     * Constructor para inicializar todos los atributos de la persona.
     * 
     * POR QUÉ: Necesidad de crear instancias de Persona con todos sus datos.
     * CÓMO: Recibe cada atributo por valor y los mueve a los miembros correspondientes.
     * PARA QUÉ: Construir objetos Persona completos y válidos.
     */
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

    int mostrarEdad() const;

    std::string mostrarCiudad() const { return ciudad; }
    char mostrarGrupo() const { return grupo; }
    double mostrarPatrimonio() const { return patrimonio; }
    struct EstadisticasGrupo {
        char idGrupo;
        long patrimonioTotal = 0; 
        double patrimonioPromedio = 0.0;
        int numPersonas = 0;
    };
    struct EstadisticasCiudad {
        std::string nombreCiudad;
        long patrimonioTotal = 0; 
        double patrimonioPromedio = 0.0;
        int numPersonas = 0;
    };

    static Persona encontrarMasLongevoPaisValor(const std::vector<Persona> personas);
    static void encontrarMasLongevoPais_ref(std::unique_ptr<std::vector<Persona>> personas, Persona& masLongeva);
    
    static void mostrarMasLongevoPorCiudad(std::vector<Persona> personas);
    static void analizarLongevidad(std::vector<Persona> personas);
    static void analizarLongevidad_ref(std::unique_ptr<std::vector<Persona>> personas);

    static void agruparPersonasPorCiudad_ref(std::unique_ptr<std::vector<Persona>> personas, std::map<std::string, std::vector<Persona>>& gruposc);
    static void mostrarMasLongevoPorCiudad_ref(std::unique_ptr<std::vector<Persona>> personas, Persona& masLongeva);
    static std::map<std::string, std::vector<Persona>> agruparPersonasPorCiudadValor(const std::vector<Persona> personas);

    static void menuPatrimonio();
    
    static void agruparPersonasPorCalendarioRef(std::unique_ptr<std::vector<Persona>> personas, std::map<char, std::vector<Persona>>& grupos); 
    static std::map<char, std::vector<Persona>> agruparPersonasPorCalendarioValor(const std::vector<Persona>& personas);

    // Declarantes de renta
    static std::map<char, std::vector<Persona>> DeclarantesPorGrupoValor(std::map<char, std::vector<Persona>> grupos);
    static void DeclarantesPorGrupoReferencia(std::map<char, std::vector<Persona>> &grupos, std::map<char, std::vector<Persona>>& grupoDeclarantes);
    
    // Ranking de riqueza por agrupación
    //static void RankingAgrupRef(std::unique_ptr<std::vector<Persona>> personas);
    static std::vector<EstadisticasGrupo> RankingAgrupValor(std::vector<Persona> personas);

    // Ranking de riqueza por ciudad
    static std::vector<EstadisticasCiudad> RankingCiudadValor(std::vector<Persona> personas);
    static void mostrarMayorPatrimonioPorValor(std::vector<Persona> personas);

        
};

#endif // PERSONA_H
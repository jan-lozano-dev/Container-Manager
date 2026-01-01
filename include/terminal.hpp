#ifndef _TERMINAL_HPP
#define _TERMINAL_HPP 
#include <list>
#include <string>
#include <esin/error>
#include <esin/util>

#include "cataleg.hpp"
#include "contenidor.hpp"
#include "ubicacio.hpp"

using std::string;
using std::list;
using util::nat;

class terminal {
public:
    static constexpr nat HMAX = 7;
  
    enum class estrategia { FIRST_FIT, LLIURE };

    // Constructor per paràmetres
    // Pre: n > 0, m > 0, 0 < h <= HMAX
    // Post: Crea una terminal amb n fileres, m places per filera i alçada h
    terminal(nat n, nat m, nat h, estrategia st);

    // Constructor per còpia
    // Pre: Existeix una terminal b vàlida.
    // Post: Crea una terminal amb els valors de b.
    terminal(const terminal& b);

    // Constructor per assignació
    // Pre: P.I i b són terminals vàlides.
    // Post: Al P.I s'assigna els valors de b.
    terminal& operator=(const terminal& b);

    // Destructor
    // Pre: Existeix P.I vàlid.
    // Post: El P.I es destruit.
    ~terminal() noexcept;

    // Inserció
    // Pre: El contenidor c no està a la terminal.
    // Post: S'ha inserit el contenidor c a la terminal segons l'estratègia
    void insereix_contenidor(const contenidor &c);

    // Retirada
    // Pre: El contenidor amb matrícula m està a la terminal.
    // Post: S'ha retirat el contenidor amb matrícula m de la terminal
    void retira_contenidor(const string &m);

    // Consultors
    // Pre: El contenidor amb matrícula m està a la terminal.
    // Post: Retorna la ubicació del contenidor amb matrícula m
    ubicacio on(const string &m) const noexcept;

    // Pre: El contenidor amb matrícula m està a la terminal.
    // Post: Retorna la longitud del contenidor amb matrícula m
    nat longitud(const string &m) const;

    // Pre: La ubicació u és dins de la terminal.
    // Post: Retorna la matrícula del contenidor que ocupa la ubicació u
    void contenidor_ocupa(const ubicacio &u, string &m) const;  

    // Pre: -
    // Post: Retorna el grau de fragmentació actual de la terminal
    nat fragmentacio() const noexcept;

    // Pre: -
    // Post: Retorna el nombre de places lliures a la grua
    nat ops_grua() const noexcept;

    // Pre: -
    // Post: Omple la llista l amb les matrícules dels contenidors a l'àrea d'espera
    void area_espera(list<string> &l) const noexcept;

    // Pre: -
    // Post: Retorna el nombre de fileres de la terminal
    nat num_fileres() const noexcept;

    // Pre: -
    // Post: Retorna el nombre de places per filera de la terminal
    nat num_places() const noexcept;

    // Pre: -
    // Post: Retorna l'alçada de la terminal
    nat num_pisos() const noexcept;

    // Pre: -
    // Post: Retorna l'estratègia de la terminal
    estrategia quina_estrategia() const noexcept; 

    static constexpr int NumFileresIncorr = 40;
    static constexpr int NumPlacesIncorr  = 41;
    static constexpr int AlcadaMaxIncorr  = 42;
    static constexpr int EstrategiaIncorr = 43;
    static constexpr int MatriculaDuplicada  = 44;
    static constexpr int MatriculaInexistent = 45;
    static constexpr int UbicacioNoMagatzem  = 46;

private:
    #include "terminal.rep"
};
#endif

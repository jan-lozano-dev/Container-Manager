#include "../include/terminal.hpp"

// Constructor per paràmetres
terminal::terminal(nat n, nat m, nat h, estrategia st) : num_fileres(n), places_fila(m), altura(h), estrategia_seleccionada(st) {
    if (n <= 0 or m <= 0 or h == 0 or h > HMAX) {
        throw error("Error en la creació de la terminal: paràmetres invàlids.");
    }
    fileres.resize(n);
}

// Constructor per còpia
terminal::terminal(const terminal& b) : num_fileres(b.num_fileres), places_fila(b.places_fila), altura(b.altura), estrategia_seleccionada(b.estrategia_seleccionada),
    fileres(b.fileres), contenidors_map(b.contenidors_map), area_espera_list(b.area_espera_list) {
}

// Constructor per assignació
terminal& terminal::operator=(const terminal& b) {
    if (this != &b) {
        num_fileres = b.num_fileres;
        places_fila = b.places_fila;
        altura = b.altura;
        estrategia_seleccionada = b.estrategia_seleccionada;
        fileres = b.fileres;
        contenidors_map = b.contenidors_map;
        area_espera_list = b.area_espera_list;
    }
    return *this;
}

// Inserció
void terminal::insereix_contenidor(const contenidor &c) {
    // Implementació de la inserció segons l'estratègia seleccionada
    
}

// Retirada
void terminal::retira_contenidor(const string &m) {
    // Implementació de la retirada del contenidor amb matrícula m
    
}

// Consultors

ubicacio terminal::on(const string &m) const noexcept {
    // Implementació per retornar la ubicació del contenidor amb matrícula m
    
}

nat terminal::longitud(const string &m) const {
    // Implementació per retornar la longitud del contenidor amb matrícula m
    
}

void terminal::contenidor_ocupa(const ubicacio &u, string &m) const {
    // Implementació per retornar la matrícula del contenidor que ocupa la ubicació u
    
}

nat terminal::fragmentacio() const noexcept {
    // Implementació per retornar el grau de fragmentació actual de la terminal
    
}

nat terminal::ops_grua() const noexcept {
    // Implementació per retornar el nombre de places lliures a la grua
    
}

void terminal::area_espera(list<string> &l) const noexcept {
    // Implementació per omplir la llista l amb les matrícules dels contenidors a l'àrea d'espera
    
}

nat terminal::num_fileres() const noexcept {
    return num_fileres;
}

nat terminal::num_places() const noexcept {
    return places_fila;
}

nat terminal::num_pisos() const noexcept {
    return altura;
}

estrategia terminal::quina_estrategia() const noexcept { 
    return estrategia_seleccionada;
}


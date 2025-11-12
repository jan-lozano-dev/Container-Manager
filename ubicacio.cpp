#include <ubicacio.hpp>

// Constructor

ubicacio(int i, int j, int k){
    _fila = i;
    _columna = j;
    _altura = k;
}

// Constructor per copia

ubicacio(const ubicacio& u){
    _fila = u._fila;
    _columna = u._columna;
    _altura = u._altura;
}

// Constructor per
ubicacio& operator=(const ubicacio& u);

// Destructor

~ubicacio() noexcept;

// Consultors

int filera() const noexcept{
    return _fila;
}

int placa() const noexcept{
    return _columna;
}

int pis() const noexcept{
    return _altura;
}

bool operator==(const ubicacio &u) const noexcept;
bool operator!=(const ubicacio &u) const noexcept;
bool operator<(const ubicacio &u) const noexcept;
bool operator<=(const ubicacio &u) const noexcept;
bool operator>(const ubicacio &u) const noexcept;
bool operator>=(const ubicacio &u) const noexcept;

static constexpr int UbicacioIncorrecta = 10;


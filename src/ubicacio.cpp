#include "../include/ubicacio.hpp"

ubicacio::ubicacio(int i, int j, int k) {
    bool ok = false;
    if (i >= 0 && j >= 0 && k >= 0) ok = true;
    if (i == -1 && j == 0 && k == 0) ok = true;
    if (i == -1 && j == -1 && k == -1) ok = true;
    if (!ok) throw error(UbicacioIncorrecta);

    _fila = i;
    _columna = j;
    _altura = k;
}

ubicacio::ubicacio(const ubicacio& orig) {
    _fila = orig._fila;
    _columna = orig._columna;
    _altura = orig._altura;
}

ubicacio& ubicacio::operator=(const ubicacio& orig) {
    _fila = orig._fila;
    _columna = orig._columna;
    _altura = orig._altura;
    return *this;
}

ubicacio::~ubicacio() noexcept { }

int ubicacio::filera() const noexcept { return _fila; }
int ubicacio::placa() const noexcept { return _columna; }
int ubicacio::pis() const noexcept { return _altura; }

bool ubicacio::operator==(const ubicacio &u) const noexcept {
    return _fila == u._fila && _columna == u._columna && _altura == u._altura;
}

bool ubicacio::operator!=(const ubicacio &u) const noexcept {
    return !(*this == u);
}

bool ubicacio::operator<(const ubicacio &u) const noexcept {
    if (_fila != u._fila) return _fila < u._fila;
    if (_columna != u._columna) return _columna < u._columna;
    return _altura < u._altura;
}

bool ubicacio::operator<=(const ubicacio &u) const noexcept {
    return *this < u || *this == u;
}

bool ubicacio::operator>(const ubicacio &u) const noexcept {
    return u < *this;
}

bool ubicacio::operator>=(const ubicacio &u) const noexcept {
    return u <= *this;
}

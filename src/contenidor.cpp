#include "../include/contenidor.hpp"

contenidor::contenidor(const string &mat, nat lon) {
    if (mat.empty()) throw error(MatriculaIncorrecta);

    for (size_t i = 0; i < mat.size(); ++i) {
        char c = mat[i];
        bool ok = (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
        if (!ok) throw error(MatriculaIncorrecta);
    }

    if (lon != 10 && lon != 20 && lon != 30) throw error(LongitudIncorrecta);

    _matricula = mat;
    _longitud = lon;
}

contenidor::contenidor(const contenidor &c) {
    _matricula = c._matricula;
    _longitud = c._longitud;
}

contenidor& contenidor::operator=(const contenidor &c) {
    _matricula = c._matricula;
    _longitud = c._longitud;
    return *this;
}

contenidor::~contenidor() noexcept { }

nat contenidor::longitud() const noexcept { return _longitud; }
string contenidor::matricula() const noexcept { return _matricula; }

bool contenidor::operator==(const contenidor &c) const noexcept {
    return _matricula == c._matricula && _longitud == c._longitud;
}

bool contenidor::operator!=(const contenidor &c) const noexcept {
    return !(*this == c);
}

bool contenidor::operator<(const contenidor &c) const noexcept {
    if (_matricula != c._matricula) return _matricula < c._matricula;
    return _longitud < c._longitud;
}

bool contenidor::operator<=(const contenidor &c) const noexcept {
    return *this < c || *this == c;
}

bool contenidor::operator>(const contenidor &c) const noexcept {
    return c < *this;
}

bool contenidor::operator>=(const contenidor &c) const noexcept {
    return c <= *this;
}

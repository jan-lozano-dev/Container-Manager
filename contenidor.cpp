#include <contenidor.hpp>

// Constructors

contenidor::contenidor(const string &m, nat l){
    _matricula = m;
    _longitud = l;
}

contenidor::contenidor(const contenidor &u){
    _matricula = u._matricula;
    _longitud = u._lonigtud;
}

contenidor::contenidor& operator=(const contenidor &u){
    if(this != &u){
        _matricula = u._matricula;
        _longitud = u._lonigtud;
    } 
    return *this;
}

contenidor::~contenidor() noexcept;

// Consultors

contenidor::nat longitud() const noexcept{
    return _longitud;
}
contenidor::string matricula() const noexcept{
    return _matricula;
}

bool contenidor::operator==(const contenidor &c) const noexcept;
bool contenidor::operator!=(const contenidor &c) const noexcept;
bool contenidor::operator<(const contenidor &c) const noexcept;
bool contenidor::operator<=(const contenidor &c) const noexcept;
bool contenidor::operator>(const contenidor &c) const noexcept;
bool contenidor::operator>=(const contenidor &c) const noexcept;

static constexpr int MatriculaIncorrecta = 20;
static constexpr int LongitudIncorrecta = 21;

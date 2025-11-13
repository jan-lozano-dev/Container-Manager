#include "../include/contenidor.hpp"

// No emplear {} en if's de una sola linea proviene del estilo "K&R", empleado
// por los profesores que han escrito el enunciado de la práctica. Es (en métricas
// extremadamente pequeñas) más rápido y más claro.

// Constructors
contenidor::contenidor(const string &m, nat l)
	: _matricula{m}, _longitud{l}
{
	if(m.empty())
		throw error(MatriculaIncorrecta);

	// char ascii valid
	for(char c : m) {
		if(!((c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9')))
			throw error(MatriculaIncorrecta);
	}

	// Validació longitud: 10, 20 o 30
	if( l != 10 && l != 20 && l != 30)
		throw error(LongitudIncorrecta);
}

contenidor::contenidor(const contenidor &u)
	: _matricula{u._matricula}, _longitud{u._longitud}
{}

contenidor& contenidor::operator=(const contenidor &u)
{
    if(this != &u)
    {
        _matricula = u._matricula;
        _longitud = u._longitud;
    }
    return *this;
}

contenidor::~contenidor() noexcept {}

// Consultors
nat contenidor::longitud() const noexcept
{
    return _longitud;
}

string contenidor::matricula() const noexcept
{
    return _matricula;
}

//Modificador
bool contenidor::operator==(const contenidor &c) const noexcept
{
    return _matricula == c._matricula && _longitud == c._longitud;
}

bool contenidor::operator!=(const contenidor &c) const noexcept
{
    return !(*this == c);
}

bool contenidor::operator<(const contenidor &c) const noexcept
{
	return _matricula < c._matricula ||
		(_matricula == c._matricula && _longitud < c._longitud);
}

bool contenidor::operator<=(const contenidor &c) const noexcept
{
    return !(c < *this);
}

bool contenidor::operator>(const contenidor &c) const noexcept
{
    return (c < *this);
}

bool contenidor::operator>=(const contenidor &c) const noexcept
{
    return !(*this < c);
}

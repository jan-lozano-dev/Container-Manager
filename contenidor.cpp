#include "../include/contenidor.hpp"

// No emplear {} en if's de una sola linea proviene del estilo "K&R", empleado
// por los profesores que han escrito el enunciado de la práctica. Es (en métricas
// extremadamente pequeñas) más rápido y más claro.

// Constructors
// Pre: -
// Post: Crea un contenidor amb els valors m i l.
//		 Altrament, llença una excepció.
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
	if(l != 10 && l != 20 && l != 30)
		throw error(LongitudIncorrecta);
}

// Constructor per còpia
// Pre: Existeix un contenidor u amb uns valors vàlids.
// Post: Crea un contenidor amb els valors d'u.
contenidor::contenidor(const contenidor &u)
	: _matricula{u._matricula}, _longitud{u._longitud}
{}

// Constructor per assignació
// Pre: P.I i u són contenidors vàlids.
// Post: Al P.I s'assignen els valors de matricula i longitud d'u.
contenidor& contenidor::operator=(const contenidor &u)
{
    if(this != &u)
    {
        _matricula = u._matricula;
        _longitud = u._longitud;
    }
    return *this;
}

// Pre: Existeix P.I vàlid.
// Post: El P.I es destruit.
contenidor::~contenidor() noexcept {}

// Consultors
// Pre: -
// Post: Retorna la longitud del P.I.
nat contenidor::longitud() const noexcept
{
    return _longitud;
}

// Pre: -
// Post: Retorna la matricula del P.I.
string contenidor::matricula() const noexcept
{
    return _matricula;
}

//Operadors
// Pre: Existeix un P.I vàlid.
// Post: Retorna si ambdós contenidors (P.I i c) són iguals o no.
bool contenidor::operator==(const contenidor &c) const noexcept
{
    return _matricula == c._matricula && _longitud == c._longitud;
}

// Pre: Existeix P.I vàlid.
// Post: Retorna si ambdós contenidors (P.I i c) són diferents o no.
bool contenidor::operator!=(const contenidor &c) const noexcept
{
    return !(*this == c);
}

// Pre: Existeix P.I vàlid.
// Post: Retorna si P.I és menor que c.
bool contenidor::operator<(const contenidor &c) const noexcept
{
	return _matricula < c._matricula || (_matricula == c._matricula && _longitud < c._longitud);
}

// Pre: Existeix P.I vàlid.
// Post: Retorna si P.I és menor o igual que c.
bool contenidor::operator<=(const contenidor &c) const noexcept
{
    return !(c < *this);
}

// Pre: Existeix P.I vàlid.
// Post: Retorna si P.I és major que c.
bool contenidor::operator>(const contenidor &c) const noexcept
{
    return (c < *this);
}

// Pre: Existeix P.I vàlid.
// Post: Retorna si P.I és major o igual que c.
bool contenidor::operator>=(const contenidor &c) const noexcept
{
    return !(*this < c);
}

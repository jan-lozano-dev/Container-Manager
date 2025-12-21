#include "../include/ubicacio.hpp"

//Inicialització de membres amb ":" per millorar performance
//(inicialització directa, evitem inicialitzacions dobles
//accidentals.

// Constructor
// Pre: -
// Post: Crea una ubicació <i, j, k>.
//		 De forma alterna, llença una excepció.
ubicacio::ubicacio(int i, int j, int k)
	: _fila{i}, _columna{j}, _altura{k}
{
    bool cas_especial = (i == -1 && j == 0 && k == 0) || (i == -1 && j == -1 && k == -1);

    bool cas_normal = (i >= 0 || j >= 0 || k >= 0);

    if(!cas_especial && !cas_normal)
        throw error(UbicacioIncorrecta);

}

// Constructor per copia
// Pre: Existeix una ubicació u amb uns valors <i, j, k> vàlids.
// Post: Crea una ubicació <i, j, k> amb els valors d'u.
ubicacio::ubicacio(const ubicacio& u)
	: _fila{u._fila}, _columna{u._columna}, _altura{u._altura}
{}

// Constructor per assignació
// Pre: P.I i u són una ubicacions vàlides.
// Post: Al P.I s'assigna els valors <i, j, k> d'u.
ubicacio& ubicacio::operator=(const ubicacio& u)
{
	if(this != &u)
	{
		_fila = u._fila;
		_columna= u._columna;
		_altura = u._altura;
	}

	return *this;
}

// Destructor
// Pre: Existeix P.I vàlid.
// Post: El P.I es destruit.
ubicacio::~ubicacio() noexcept {}

// Consultors
// Pre: -
// Post: Retorna la filera on es troba el P.I.
int ubicacio::filera() const noexcept
{
    return _fila;
}

// Pre: -
// Post: Retorna la columna on es troba el P.I.
int ubicacio::placa() const noexcept
{
    return _columna;
}

// Pre: - 
// Post: Retorna l'altura on es troba el P.I.
int ubicacio::pis() const noexcept
{
    return _altura;
}

// Operators
// Pre: Existeix P.I vàlid. 
// Post: Retorna si ambdós ubicacions (P.I i u) són iguals o no.
bool ubicacio::operator==(const ubicacio &u) const noexcept
{
	return _fila == u._fila && _columna == u._columna && _altura == u._altura;
}

// Pre: Existeix P.I vàlid.
// Post: Retorna si ambdós ubicacions (P.I i u) són diferents o no.
bool ubicacio::operator!=(const ubicacio &u) const noexcept
{
	return !(*this == u);
}

//Implementació prioritzant performance (sense comparacions
//redundants) i eficient a nivell de cache (Accessos a
//memoria sequencials)
// Pre: Existeix P.I vàlid.
// Post: Retorna si P.I és menor que u. 
bool ubicacio::operator<(const ubicacio &u) const noexcept
{
	return _fila < u._fila || (_fila == u._fila && (_columna < u._columna || (_columna == u._columna && _altura < u._altura)));
}

// Pre: Existeix P.I vàlid.
// Post: Retorna si P.I és menor o igual que u. 
bool ubicacio::operator<=(const ubicacio &u) const noexcept
{
	return !(u < *this);
}

// Pre: Existeix P.I vàlid.
// Post: Retorna si P.I és major que u. 
bool ubicacio::operator>(const ubicacio &u) const noexcept
{
	return (u < *this);
}

// Pre: Existeix P.I vàlid.
// Post: Retorna si P.I és major o igual que u. 
bool ubicacio::operator>=(const ubicacio &u) const noexcept
{
	return !(*this < u);
}

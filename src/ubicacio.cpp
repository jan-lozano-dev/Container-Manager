#include "../include/ubicacio.hpp"

//Inicialització de membres amb ":" per millorar performance
//(inicialització directa, evitem inicialitzacions dobles
//accidentals.

// Constructor
ubicacio::ubicacio(int i, int j, int k)
	: _fila{i}, _columna{j}, _altura{k}
{
    bool cas_especial = (i == -1 && j == 0 && k == 0) ||
            (i == -1 && j == -1 && k == -1);

    bool cas_normal = (i >= 0 || j >= 0 || k >= 0);

    if(!cas_especial && !cas_normal)
        throw error(UbicacioIncorrecta);

}

// Constructor per copia
ubicacio::ubicacio(const ubicacio& u)
	: _fila{u._fila}, _columna{u._columna}, _altura{u._altura}
{}

// Constructor per assignació
ubicacio& ubicacio::operator=(const ubicacio& u)
{
	if(this != &u)
	{
		_fila = u._fila;
		_columna=u._columna;
		_altura = u._altura;
	}

	return *this;
}

// Destructor
ubicacio::~ubicacio() noexcept {}

// Consultors
int ubicacio::filera() const noexcept
{
    return _fila;
}

int ubicacio::placa() const noexcept
{
    return _columna;
}

int ubicacio::pis() const noexcept
{
    return _altura;
}

// Operators
bool ubicacio::operator==(const ubicacio &u) const noexcept
{
	return _fila == u._fila &&
		_columna == u._columna &&
		_altura == u._altura;
}

bool ubicacio::operator!=(const ubicacio &u) const noexcept
{
	return !(*this == u);
}

//Implementació prioritzant performance (sense comparacions
//redundants) i eficient a nivell de cache (Accessos a
//memoria sequencials)
bool ubicacio::operator<(const ubicacio &u) const noexcept
{
	return _fila < u._fila ||
		(_fila == u._fila && (_columna < u._columna ||
				      (_columna == u._columna &&
				       _altura < u._altura)));
}

bool ubicacio::operator<=(const ubicacio &u) const noexcept
{
	return !(u < *this);
}

bool ubicacio::operator>(const ubicacio &u) const noexcept
{
	return (u < *this);
}

bool ubicacio::operator>=(const ubicacio &u) const noexcept
{
	return !(*this < u);
}

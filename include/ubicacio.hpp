#ifndef _UBICACIO_HPP
#define _UBICACIO_HPP 

#include <esin/error>
#include <esin/util>

class ubicacio {
public:
    // Constructor per parametres
    // Pre: cert
    // Post: Crea una ubicacio amb filera i placa i pis donats
    ubicacio(int i, int j, int k);

    // Constructor per copia
    // Pre: Existeix una ubicació u amb uns valors <i, j, k> vàlids.
    // Post: Crea una ubicació <i, j, k> amb els valors d'u.
    ubicacio(const ubicacio& u);

    // Constructor per assignació
    // Pre: P.I i u són una ubicacions vàlides.
    // Post: Al P.I s'assigna els valors <i, j, k> d'u.
    ubicacio& operator=(const ubicacio& u);

    // Destructor
    // Pre: Existeix P.I vàlid.
    // Post: El P.I es destruit.
    ~ubicacio() noexcept;

    // Consultors

    // Pre: -
    // Post: Retorna la filera on es troba el P.I.
    int filera() const noexcept;

    // Pre: -
    // Post: Retorna la columna on es troba el P.I.
    int placa() const noexcept;

    // Pre: -
    // Post: Retorna el pis on es troba el P.I.
    int pis() const noexcept;

    // Operadors de comparació

    // Pre: Existeixen P.I i u vàlids.
    // Post: Retorna cert si P.I i u són iguals, fals en cas contrari.
    bool operator==(const ubicacio &u) const noexcept;

    // Pre: Existeixen P.I i u vàlids.
    // Post: Retorna cert si P.I i u no són iguals, fals en cas contrari.
    bool operator!=(const ubicacio &u) const noexcept;

    // Pre: Existeixen P.I i u vàlids.
    // Post: Retorna cert si P.I és menor que u, fals en cas contrari.
    bool operator<(const ubicacio &u) const noexcept;

    // Pre: Existeixen P.I i u vàlids.
    // Post: Retorna cert si P.I és menor o igual que u, fals en cas contrari.
    bool operator<=(const ubicacio &u) const noexcept;

    // Pre: Existeixen P.I i u vàlids.
    // Post: Retorna cert si P.I és major que u, fals en cas contrari.
    bool operator>(const ubicacio &u) const noexcept;

    // Pre: Existeixen P.I i u vàlids.
    // Post: Retorna cert si P.I és major o igual que u, fals en cas contrari.
    bool operator>=(const ubicacio &u) const noexcept;

    static constexpr int UbicacioIncorrecta = 10;

private:
    #include "ubicacio.rep"
};
#endif

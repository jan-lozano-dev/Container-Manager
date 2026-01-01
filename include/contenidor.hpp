#ifndef _CONTENIDOR_HPP
#define _CONTENIDOR_HPP 

#include <string>
#include <esin/error>
#include <esin/util>

using std::string;
using util::nat;

class contenidor {
public:
    // Constructor per paràmetres
    // Pre: cert
    // Post: Crea un contenidor amb matrícula m i longitud l.
    contenidor(const string &m, nat l);

    // Constructor per còpia
    // Pre: Existeix un contenidor u vàlid.
    // Post: Crea un contenidor amb els valors d'u.
    contenidor(const contenidor &u);

    // Constructor per assignació
    // Pre: P.I i u són contenidors vàlids.
    // Post: Al P.I s'assigna els valors d'u.
    contenidor& operator=(const contenidor &u);

    // Destructor
    // Pre: Existeix P.I vàlid.
    // Post: El P.I es destruit.
    ~contenidor() noexcept;

    // Consultors

    // Pre: -
    // Post: Retorna la longitud del contenidor.
    nat longitud() const noexcept;

    // Pre: -
    // Post: Retorna la matrícula del contenidor.
    string matricula() const noexcept;

    // Operadors de comparació

    // Pre: Existeixen P.I i c vàlids.
    // Post: Retorna cert si P.I i c són iguals, fals en cas contrari.
    bool operator==(const contenidor &c) const noexcept;

    // Pre: Existeixen P.I i c vàlids.
    // Post: Retorna cert si P.I i c no són iguals, fals en cas contrari.
    bool operator!=(const contenidor &c) const noexcept;

    // Pre: Existeixen P.I i c vàlids.
    // Post: Retorna cert si P.I és menor que c, fals en cas contrari.
    bool operator<(const contenidor &c) const noexcept;

    // Pre: Existeixen P.I i c vàlids.
    // Post: Retorna cert si P.I és menor o igual que c, fals en cas contrari.
    bool operator<=(const contenidor &c) const noexcept;

    // Pre: Existeixen P.I i c vàlids.
    // Post: Retorna cert si P.I és major que c, fals en cas contrari.
    bool operator>(const contenidor &c) const noexcept;

    // Pre: Existeixen P.I i c vàlids.
    // Post: Retorna cert si P.I és major o igual que c, fals en cas contrari.
    bool operator>=(const contenidor &c) const noexcept;

    static constexpr int MatriculaIncorrecta = 20;
    static constexpr int LongitudIncorrecta = 21;

private:
    #include "contenidor.rep"
};
#endif

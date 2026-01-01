#ifndef _CATALEG_HPP
#define _CATALEG_HPP 
#include <string>
#include <esin/error>
#include <esin/util>

using std::string;
using util::nat;

template <typename Valor>
class cataleg {
public:

    // Constructora per defecte
    // Pre: numelems > 0
    // Post: crea un catàleg buit amb capacitat per a numelems elements
    explicit cataleg(nat numelems);

    // Constructora de còpia
    // Pre: cert
    // Post: crea un catàleg que és còpia de c
    cataleg(const cataleg& c);

    // Operador d'assignació
    // Pre: cert
    // Post: el catàleg actual és una còpia de c
    cataleg& operator=(const cataleg& c);

    // Destructora
    // Pre: Existeix un P.I vàlid.
    // Post: El P.I es destruit.
    ~cataleg() noexcept;

    // Assignació
    // Pre: -
    // Post: assigna el valor v a la clau k
    void assig(const string &k, const Valor &v);

    // Eliminació
    // Pre: -
    // Post: elimina l'entrada amb clau k
    void elimina(const string &k);

    // Consultors
    // Pre: -
    // Post: Retorna si existeix una entrada a la taula que correspon amb k.
    bool existeix(const string &k) const noexcept;

    // Pre: -
    // Post: Retorna el valor associat a la clau k.
    Valor operator[](const string &k) const;

    // Pre: Existeix un P.I vàlid.
    // Post: Retorna quantes claus té.
    nat quants() const noexcept;

    static constexpr int ClauStringBuit = 30;
    static constexpr int ClauInexistent = 31;

private:
    #include "cataleg.rep"
};

#include "cataleg.t"

#endif

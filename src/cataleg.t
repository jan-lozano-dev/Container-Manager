#include "../include/cataleg.hpp"

// Constructor
template <typename Valor>
cataleg<Valor>::cataleg(nat numelems)
    : _M{numelems}, _quants{0}
{
    _taula = new node_hash*[_M];
    for(nat i = 0; i < _M; ++i) _taula[i] = nullptr;
}

// Constructor per còpia
template <typename Valor>
cataleg<Valor>::cataleg(const cataleg& c)
    : _M{c._M}, _quants{c._quants}
{
    _taula = new node_hash*[_M];
    for(nat i = 0; i < _M; ++i)
    {
        node_hash *act = c._taula[i];
        node_hash **dest = &_taula[i];
        while(act)
        {
            *dest = new node_hash(act->_k, act->_v);
            dest = &((*dest)->_seg);
            act = act->_seg;
        }
    }
}

// Operador asignació
template <typename Valor>
cataleg<Valor>& cataleg<Valor>::operator=(const cataleg& c)
{
    if(this != &c)
    {
        // Alliberar diccionari actual
        for(nat i = 0; i < _M; ++i)
        {
            node_hash *act = _taula[i];
            while(act)
            {
                node_hash *tmp = act;
                act = act->_seg;
                delete tmp;
            }
        }
        delete[] _taula;

        // Copiem cataleg& c
        _M = c._M;
        _quants = c._quants;
        _taula = new node_hash*[_M];
        for(nat i = 0; i < _M; ++i)
        {
            node_hash *act = c._taula[i];
            node_hash **dest = &_taula[i];
            while(act)
            {
                *dest = new node_hash(act->_k, act->_v);
                dest = &((*dest)->_seg);
                act = act->_seg;
            }
        }
    }
    return *this;
}

// Destructor (al tractar amb memòria dinàmica,
// no és un destructor senzill
template <typename Valor>
cataleg<Valor>::~cataleg() noexcept
{
    for(nat i = 0; i < _M; ++i)
    {
        node_hash *act = _taula[i];
        while(act)
        {
            node_hash *tmp = act;
            act = act->_seg;
            delete tmp;
        }
    }
    delete[] _taula;
}

// Assignació
template <typename Valor>
void cataleg<Valor>::assig(const string &k, const Valor &v)
{
    int i = hash(k);
    node_hash *act = _taula[i];
    bool trobat = false;
    while(act && !trobat)
    {
        if(act->_k == k) act->_v = v, trobat = true;
        else act = act->_seg;
    }
    if(!trobat) { _taula[i] = new node_hash(k, v, _taula[i]); ++_quants; }
}

// Eliminació
template <typename Valor>
void cataleg<Valor>::elimina(const string &k)
{
    int i = hash(k);
    node_hash *act = _taula[i], *ant = nullptr;
    bool trobat = false;
    while(act && !trobat)
    {
        if(act->_k == k) {
            trobat = true;
            if(ant) ant->_seg = act->_seg;
            else _taula[i] = act->_seg;
            delete act;
            --_quants;
        } else{
            ant = act;
            act = act->_seg;
        }
    }
    if(!trobat) throw error(ClauInexistent);
}

// Consultor: Existeix
template<typename Valor>
bool cataleg<Valor>::existeix(const string &k) const noexcept
{
    int i = hash(k);
    node_hash *act = _taula[i];
    bool trobat = false;

    while(act && !trobat)
    {
        if(act->_k == k) trobat = true;
        else act = act->_seg;
    }

    return trobat;
}

template <typename Valor>
Valor cataleg<Valor>::operator[](const string &k) const
{
    int i = hash(k);
    node_hash *act = _taula[i];
    bool trobat = false;
    Valor v{};

    while(act && !trobat)
    {
        if(act->_k == k) { v = act->_v; trobat = true; }
        else act = act->_seg;
    }

    if(!trobat) throw error(ClauInexistent);
    return v;
}

template <typename Valor>
nat cataleg<Valor>::quants() const noexcept
{
    return _quants;
}

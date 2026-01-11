#include "../include/terminal.hpp"

using util::nat;

static const ubicacio POS_ESPERA(-1, 0, 0);
static const ubicacio POS_BUIDA(-1, -1, -1);

terminal::terminal(nat nfil, nat nplc, nat alt, estrategia est)
    : _n(nfil), _m(nplc), _h(alt), _est(est),
      _celes(nullptr), _altesa(nullptr), _espera(), _cat(1), _ops(0) {

    if (_n == 0) throw error(NumFileresIncorr);
    if (_m == 0) throw error(NumPlacesIncorr);
    if (_h == 0 || _h > HMAX) throw error(AlcadaMaxIncorr);
    if (_est != estrategia::FIRST_FIT && _est != estrategia::LLIURE)
        throw error(EstrategiaIncorr);

    _celes = new string[_n * _m * _h];
    _altesa = new nat[_n * _m];
    for (nat i = 0; i < _n * _m; ++i) _altesa[i] = 0;
}

terminal::terminal(const terminal& t)
    : _n(t._n), _m(t._m), _h(t._h), _est(t._est),
      _celes(nullptr), _altesa(nullptr),
      _espera(t._espera), _cat(t._cat), _ops(t._ops) {

    _celes = new string[_n * _m * _h];
    _altesa = new nat[_n * _m];
    for (nat i = 0; i < _n * _m * _h; ++i) _celes[i] = t._celes[i];
    for (nat i = 0; i < _n * _m; ++i) _altesa[i] = t._altesa[i];
}

terminal& terminal::operator=(const terminal& t) {
    if (this == &t) return *this;

    string* nc = new string[t._n * t._m * t._h];
    nat* na = new nat[t._n * t._m];
    for (nat i = 0; i < t._n * t._m * t._h; ++i) nc[i] = t._celes[i];
    for (nat i = 0; i < t._n * t._m; ++i) na[i] = t._altesa[i];

    delete[] _celes;
    delete[] _altesa;
    _celes = nc;
    _altesa = na;
    _n = t._n; _m = t._m; _h = t._h; _est = t._est;
    _espera = t._espera; _cat = t._cat; _ops = t._ops;
    return *this;
}

terminal::~terminal() noexcept {
    delete[] _celes;
    delete[] _altesa;
}

nat terminal::num_fileres() const noexcept { return _n; }
nat terminal::num_places() const noexcept { return _m; }
nat terminal::num_pisos() const noexcept { return _h; }
terminal::estrategia terminal::quina_estrategia() const noexcept { return _est; }
nat terminal::ops_grua() const noexcept { return _ops; }

ubicacio terminal::on(const string &mat) const noexcept {
    if (!_cat.existeix(mat)) return POS_BUIDA;
    return _cat[mat].loc;
}

nat terminal::longitud(const string &mat) const {
    if (!_cat.existeix(mat)) throw error(MatriculaInexistent);
    return _cat[mat].cont.longitud();
}

void terminal::contenidor_ocupa(const ubicacio &pos, string &mat) const {
    int f = pos.filera(), c = pos.placa(), p = pos.pis();
    bool fora = (f < 0 || c < 0 || p < 0 || f >= (int)_n || c >= (int)_m || p >= (int)_h);
    if (fora) throw error(UbicacioNoMagatzem);
    mat = _celes[p * _n * _m + f * _m + c];
}

nat terminal::fragmentacio() const noexcept {
    nat total = 0;
    for (nat fil = 0; fil < _n; ++fil) {
        for (nat col = 0; col < _m; ) {
            nat alt = _altesa[fil * _m + col];
            if (alt >= _h) { ++col; continue; }

            nat amplada = 1;
            while (col + amplada < _m && _altesa[fil * _m + col + amplada] == alt && alt < _h)
                ++amplada;

            if (amplada == 1) ++total;
            col += amplada;
        }
    }
    return total;
}

void terminal::area_espera(list<string> &res) const noexcept {
    res.clear();
    for (auto it = _espera.begin(); it != _espera.end(); ++it)
        res.push_back(it->matricula());
    res.sort();
}

void terminal::col_loca_contenidor_magatzem(const string& mat, const ubicacio& pos, nat pl) {
    nat f = pos.filera(), c = pos.placa(), p = pos.pis();
    for (nat d = 0; d < pl; ++d) {
        _celes[p * _n * _m + f * _m + c + d] = mat;
        ++_altesa[f * _m + c + d];
    }
    ++_ops;
}

void terminal::retira_contenidor_magatzem(const ubicacio& pos, nat pl) {
    nat f = pos.filera(), c = pos.placa(), p = pos.pis();
    for (nat d = 0; d < pl; ++d) {
        _celes[p * _n * _m + f * _m + c + d] = "";
        --_altesa[f * _m + c + d];
    }
    ++_ops;
}

bool terminal::cerca_forat_ff(nat pl, nat &rf, nat &rc, nat &rp) {
    for (nat fil = 0; fil < _n; ++fil) {
        for (nat col = 0; col + pl <= _m; ++col) {
            nat niv = _altesa[fil * _m + col];
            if (niv >= _h) continue;

            bool ok = true;
            for (nat d = 1; d < pl && ok; ++d) {
                nat a = _altesa[fil * _m + col + d];
                if (a != niv || a >= _h) ok = false;
            }
            if (ok) { rf = fil; rc = col; rp = niv; return true; }
        }
    }
    return false;
}

bool terminal::cerca_forat_bf(nat pl, nat &rf, nat &rc, nat &rp) {
    bool trobat = false;
    nat best_sobra = _m + 1, best_alt = _h + 1;

    for (nat fil = 0; fil < _n; ++fil) {
        for (nat col = 0; col < _m; ) {
            nat niv = _altesa[fil * _m + col];
            if (niv >= _h) { ++col; continue; }

            nat mida = 0, c2 = col;
            while (c2 < _m && _altesa[fil * _m + c2] == niv && niv < _h) { ++mida; ++c2; }

            if (mida >= pl) {
                nat sobra = mida - pl;
                bool millor = !trobat || sobra < best_sobra ||
                    (sobra == best_sobra && niv < best_alt) ||
                    (sobra == best_sobra && niv == best_alt && (fil < rf || (fil == rf && col < rc)));

                if (millor) {
                    trobat = true; best_sobra = sobra; best_alt = niv;
                    rf = fil; rc = col; rp = niv;
                }
            }
            col = c2;
        }
    }
    return trobat;
}

bool terminal::intenta_reubicar_espera() {
    if (_est == estrategia::FIRST_FIT) {
        for (auto it = _espera.end(); it != _espera.begin(); ) {
            --it;
            nat pl = it->longitud() / 10, f, c, p;
            if (cerca_forat_ff(pl, f, c, p)) {
                ubicacio pos(f, c, p);
                col_loca_contenidor_magatzem(it->matricula(), pos, pl);
                _cat.assig(it->matricula(), InfoCont{*it, pos});
                _espera.erase(it);
                return true;
            }
        }
    } else {
        for (auto it = _espera.begin(); it != _espera.end(); ++it) {
            nat pl = it->longitud() / 10, f, c, p;
            if (cerca_forat_bf(pl, f, c, p)) {
                ubicacio pos(f, c, p);
                col_loca_contenidor_magatzem(it->matricula(), pos, pl);
                _cat.assig(it->matricula(), InfoCont{*it, pos});
                _espera.erase(it);
                return true;
            }
        }
    }
    return false;
}

void terminal::insereix_contenidor(const contenidor &cont) {
    string mat = cont.matricula();
    if (_cat.existeix(mat)) throw error(MatriculaDuplicada);

    nat pl = cont.longitud() / 10, f, c, p;
    bool ok = (_est == estrategia::FIRST_FIT) ? cerca_forat_ff(pl, f, c, p) : cerca_forat_bf(pl, f, c, p);

    if (ok) {
        ubicacio pos(f, c, p);
        col_loca_contenidor_magatzem(mat, pos, pl);
        _cat.assig(mat, InfoCont{cont, pos});
        while (intenta_reubicar_espera()) { }
    } else {
        _espera.push_back(cont);
        _cat.assig(mat, InfoCont{cont, POS_ESPERA});
    }
}

terminal::InfoCont terminal::troba_contenidor_superior_marcat(nat fil, nat niv, const bool* marca) {
    InfoCont res; res.loc = POS_BUIDA;
    bool trobat = false;

    for (nat col = 0; col < _m; ++col) {
        nat alt = _altesa[fil * _m + col];
        if (alt <= niv) continue;

        string mat = _celes[(alt - 1) * _n * _m + fil * _m + col];
        if (mat.empty()) continue;

        InfoCont info = _cat[mat];
        if (info.loc.pis() <= (int)niv) continue;

        nat ini = info.loc.placa(), sz = info.cont.longitud() / 10;

        bool toca = false;
        for (nat d = 0; d < sz && !toca; ++d) if (marca[ini + d]) toca = true;
        if (!toca) continue;

        bool top = true;
        for (nat d = 0; d < sz && top; ++d)
            if (_altesa[fil * _m + ini + d] != (nat)(info.loc.pis() + 1)) top = false;
        if (!top) continue;

        if (!trobat || info.loc < res.loc) { trobat = true; res = info; }
    }
    return res;
}

void terminal::retira_contenidor(const string &mat) {
    if (!_cat.existeix(mat)) throw error(MatriculaInexistent);
    InfoCont info = _cat[mat];

    if (info.loc == POS_ESPERA) {
        for (auto it = _espera.begin(); it != _espera.end(); ++it) {
            if (it->matricula() == mat) { _espera.erase(it); break; }
        }
        _cat.elimina(mat);
        return;
    }

    nat fil = info.loc.filera(), col = info.loc.placa(), pis = info.loc.pis();
    nat pl = info.cont.longitud() / 10;

    bool* marca = new bool[_m];
    for (nat i = 0; i < _m; ++i) marca[i] = false;
    for (nat d = 0; d < pl; ++d) marca[col + d] = true;

    bool canvi = true;
    while (canvi) {
        canvi = false;
        for (nat c = 0; c < _m; ++c) {
            for (nat k = 0; k < _altesa[fil * _m + c]; ++k) {
                string m = _celes[k * _n * _m + fil * _m + c];
                if (m.empty()) continue;
                InfoCont inf = _cat[m];
                if (inf.loc.pis() <= (int)pis) continue;

                nat ini = inf.loc.placa(), sz = inf.cont.longitud() / 10;
                bool toca = false;
                for (nat d = 0; d < sz && !toca; ++d) if (marca[ini + d]) toca = true;

                if (toca) {
                    for (nat d = 0; d < sz; ++d) {
                        if (!marca[ini + d]) { marca[ini + d] = true; canvi = true; }
                    }
                }
            }
        }
    }

    while (true) {
        InfoCont sup = troba_contenidor_superior_marcat(fil, pis, marca);
        if (sup.loc == POS_BUIDA) break;
        nat sz = sup.cont.longitud() / 10;
        retira_contenidor_magatzem(sup.loc, sz);
        _cat.assig(sup.cont.matricula(), InfoCont{sup.cont, POS_ESPERA});
        _espera.push_back(sup.cont);
    }

    retira_contenidor_magatzem(info.loc, pl);
    _cat.elimina(mat);
    while (intenta_reubicar_espera()) { }
    delete[] marca;
}

# Container Manager - Gestor de Terminals Portuàries

Projecte d'Estructures de la Informació (ESIN) - Universitat Politècnica de Catalunya

## Descripció

Aquest projecte implementa un **sistema de gestió de contenidors** en una terminal portuària. Simula el funcionament d'un magatzem 3D on s'emmagatzemen contenidors de diferents mides, amb una grua que els col·loca i retira seguint diferents estratègies d'ubicació.

## Estructura del Projecte

```
Container-Manager/
├── include/              # Capçaleres (.hpp)
│   ├── contenidor.hpp    # Classe contenidor
│   ├── ubicacio.hpp      # Classe ubicació
│   ├── cataleg.hpp       # Classe catàleg (template)
│   ├── terminal.hpp      # Classe terminal
│   └── reps/             # Representacions internes
├── src/                  # Implementacions (.cpp)
│   ├── contenidor.cpp
│   ├── ubicacio.cpp
│   ├── cataleg.cpp
│   └── terminal.cpp
├── bin/                  # Executables i biblioteques
├── build/                # Arxius objecte (.o)
├── tests/                # Tests del projecte
└── docs/                 # Documentació
```

## Classes Principals

### Contenidor
Representa un contenidor marítim amb:
- **Matrícula**: identificador únic (format: 3 lletres + 7 dígits, ex: `ABC1234567`)
- **Longitud**: 10, 20 o 30 peus (cada 10 peus = 1 plaça)

### Ubicació
Representa una posició 3D dins el magatzem:
- **Filera** (i): fila horitzontal
- **Plaça** (j): columna dins la filera
- **Pis** (k): altura (0 = terra, màx = 7)

### Catàleg
Taula de dispersió (hash table) genèrica que permet:
- Cercar contenidors per matrícula en O(1) amortitzat
- Guardar informació associada (contenidor + ubicació)

### Terminal
Magatzem 3D que gestiona:
- **n** fileres × **m** places × **h** pisos
- Àrea d'espera per contenidors sense lloc
- Comptador d'operacions de grua

## Estratègies d'Ubicació

El sistema implementa dues estratègies per col·locar els contenidors:

### FIRST_FIT (Primer Forat)
Cerca seqüencialment el **primer forat** on càpiga el contenidor:
1. Recorre les fileres de 0 a n-1
2. Per cada filera, recorre les places de 0 a m-1
3. Retorna el primer espai contigu amb prou places i mateix nivell

```
Exemple: Contenidor de 20 peus (2 places) en terminal 3x5x3

Abans:          Després (FIRST_FIT):
[X][X][ ][ ][ ] [X][X][C][C][ ]  <- Col·locat a (0,2)
[ ][ ][ ][ ][ ] [ ][ ][ ][ ][ ]
[ ][ ][ ][ ][ ] [ ][ ][ ][ ][ ]
```

### LLIURE (Estratègia Best-Fit)

L'estratègia LLIURE implementa un algorisme **Best-Fit** que **minimitza la fragmentació** del magatzem escollint sempre el forat que s'ajusta millor a la mida del contenidor.

#### Algorisme

```cpp
// Cerca el forat amb mínim espai sobrant (Best-Fit)
static bool cerca_best_fit(nat n, nat m, nat h, nat mida, const nat *altures,
                           nat &fi, nat &fj, nat &fk) {
  bool trobat = false;
  nat min_sobrant = m + 1;
  nat min_altura = h + 1;

  for (nat filera = 0; filera < n; ++filera) {
    nat placa = 0;
    while (placa < m) {
      nat altura_base = altures[idx_alt(m, filera, placa)];
      if (altura_base >= h) { ++placa; continue; }

      // Mesurar longitud total del forat contigu
      nat longitud_forat = 0;
      nat p = placa;
      while (p < m) {
        nat alt = altures[idx_alt(m, filera, p)];
        if (alt != altura_base || alt >= h) break;
        ++longitud_forat;
        ++p;
      }

      // Avaluar si aquest forat és millor que l'actual
      if (longitud_forat >= mida) {
        nat sobrant = longitud_forat - mida;

        // Prioritat: 1) Mínim sobrant 2) Menor altura 3) Posició anterior
        bool es_millor = !trobat;
        if (!es_millor && sobrant < min_sobrant) es_millor = true;
        if (!es_millor && sobrant == min_sobrant && altura_base < min_altura) es_millor = true;
        if (!es_millor && sobrant == min_sobrant && altura_base == min_altura) {
          if (filera < fi || (filera == fi && placa < fj)) es_millor = true;
        }

        if (es_millor) {
          trobat = true;
          min_sobrant = sobrant;
          min_altura = altura_base;
          fi = filera; fj = placa; fk = altura_base;
        }
      }
      placa = p;  // Saltar al final del forat
    }
  }
  return trobat;
}
```

#### Funcionament Pas a Pas

1. **Exploració de forats contigus**: Identifica cada forat (seqüència de places amb mateixa altura) i calcula la seva mida.

2. **Criteri principal - Mínim sobrant**: Escull el forat on `mida_forat - mida_contenidor` sigui mínim.
   - Un contenidor de 20 peus preferirà un forat de 2 places abans que un de 5.
   - Això **conserva forats grans** per contenidors grans futurs.

3. **Criteri secundari - Menor altura**: En cas d'empat de sobrant, prefereix el forat més baix.

4. **Criteri terciari - Posició lexicogràfica**: Si encara hi ha empat, escull la posició menor.

#### Exemple Visual

```
Terminal 1x10 amb contenidors existents:

Pis 0:  [A][A][ ][ ][ ][B][B][ ][ ][ ]
        places: 0  1  2  3  4  5  6  7  8  9
                   |--3--|        |--3--|
                   forat1         forat2

Nou contenidor de 20 peus (2 places):

FIRST_FIT: Col·locaria a plaça 2 (primer forat) → deixa 1 plaça inútil
LLIURE:    Col·locaria a plaça 7 (forat de 3) → TAMBÉ deixa 1 plaça!

Però si el contenidor fos de 30 peus (3 places):
- Amb FIRST_FIT previ: forat1 té 1 plaça, forat2 té 3 → NOMÉS cap a forat2
- Amb LLIURE: forat1 té 3, forat2 té 3 → Cap a qualsevol (escull primer)

Avantatge: LLIURE evita crear forats petits inútils quan hi ha alternatives!
```

#### Cas Real de Millora

```
Terminal 1x6, contenidors de 10 i 20 peus:

Seqüència d'insercions: C1(20), C2(10), C3(10), C4(20)

AMB FIRST_FIT:
1. C1(20): [C1][C1][ ][ ][ ][ ]
2. C2(10): [C1][C1][C2][ ][ ][ ]
3. C3(10): [C1][C1][C2][C3][ ][ ]
4. C4(20): [C1][C1][C2][C3][C4][C4] ✓

AMB LLIURE (Best-Fit):
1. C1(20): [ ][ ][ ][ ][C1][C1]  ← escull forat més ajustat (6 places, a la dreta)
   Realment: [C1][C1][ ][ ][ ][ ] (forat únic, igual)
2. C2(10): [C1][C1][ ][ ][ ][C2]  ← forat d'1 és millor que forat de 4
   Però inicialment tot és igual...

Millor exemple - amb forats existents:
Estat: [X][X][ ][ ][ ][Y][Y][ ][ ][ ]
       forat de 3     forat de 3

Inserir C(20 peus = 2 places):
FIRST_FIT: [X][X][C][C][ ][Y][Y][ ][ ][ ] → deixa forat d'1
LLIURE:    [X][X][C][C][ ][Y][Y][ ][ ][ ] → mateix (empat, escull primer)

Inserir C(10 peus = 1 plaça):
FIRST_FIT: [X][X][C][ ][ ][Y][Y][ ][ ][ ]
LLIURE:    [X][X][ ][ ][C][Y][Y][ ][ ][ ] → ocupa el forat d'1 exacte!
```

#### Gestió de l'Àrea d'Espera

Quan es retira un contenidor i es buiden posicions, LLIURE intenta reubicar contenidors de l'àrea d'espera amb ordre **FIFO** (First In, First Out):

```cpp
// LLIURE: intentar des del principi (FIFO)
for (auto it = _espera.begin(); it != _espera.end(); ++it) {
  nat f, p, pis;
  nat mida = num_places_contenidor(*it);

  if (cerca_best_fit(_n, _m, _h, mida, _altesa, f, p, pis)) {
    ubicacio nova_loc(f, p, pis);
    col_loca_contenidor_magatzem(it->matricula(), nova_loc, mida);
    _cat.assig(it->matricula(), InfoCont{*it, nova_loc});
    _espera.erase(it);
    return true;
  }
}
```

Això garanteix que els contenidors que porten més temps esperant tenen prioritat.

## Compilació

```bash
make
```

## Execució

```bash
./bin/driver_gestterm < tests/entrada.txt
```

## Complexitat i Comparativa

| Operació | FIRST_FIT | LLIURE (Best-Fit) |
|----------|-----------|-------------------|
| Cerca forat | O(n·m) | O(n·m) |
| Inserció | O(n·m + k) | O(n·m + k) |
| Retirada | O(n·m·h + k) | O(n·m·h + k) |

On `k` és el nombre de contenidors a l'àrea d'espera.

| Característica | FIRST_FIT | LLIURE (Best-Fit) |
|----------------|-----------|-------------------|
| Fragmentació | Alta | **Baixa** |
| Velocitat | Ràpid (para al primer) | Igual complexitat |
| Forats grans | Els consumeix ràpid | **Els conserva** |
| Ús recomanat | Inserció massiva ràpida | **Ús general** |

## Autors

Projecte desenvolupat per a l'assignatura ESIN - UPC

## Llicència

Projecte acadèmic - Tots els drets reservats

# Resum Tema 6 - Diccionaris

## 6.1. Conceptes

f : K = -> V ; (K) = Claus ; (V) = Valors
Funció total: Totes les claus tenen valor associat.
Funció parcial: No tota clau té valor associat.
Funció parcial pot transformar-se en total associant valor indefinit a tota clau sense informació associada.

#### Diccionaris: conjunts de parells clau i valor (injectiu)

Diccionari té mètodes com:

- Cerca clau (Ex: Cerca info_persona (valor) amb dni (clau))

#### Conjunts: Cas particular de diccionari

Clau == Elements del conjunt ; Valor == bool. 

- True == Element del conjunt. 
- False == Elements que no ho és.

### 6.1.1 Classificació dels diccionaris

Classificació segons operacions d'actualització permesses. 
Sempre existeix operació de consulta de valor associat a clau.
N = elements del diccionari 

- Estàtic: N són coneguts. Prohibit insercions/eliminacions. 
Té constructor amb vector/llista d'elements.
- Semidinàmic: Permet inserció i modificació del valor associat a clau. Prohibides eliminacions.
- Dinàmic: Permet inserció, modificació i eliminacions.

## 6.2. Especificació

### 6.2.1 Especificació bàsica (dinàmica (cas més genèric)

```
template<typename Clau, typename Valor>
class dicc {
public:
    // Constructor dicc buit.
    dicc() throw(error);

    // Els tres grans.
    dicc(const dicc &d) throw(error);
    dicc& operator=(const dicc &d) throw(error);
    ~dicc() throw(); 
    
    // Afegeix parell <k, v> a dicc if(!existeix clau k); else(substitueix valor antic per v)
    void insereix(const Clau &k, const Valor &v) throw(error);

    // Elimina el parell <k, v> si existeix parell amb 'k'.
    void elimina(const Clau &k) throw();

    // Retorna true si i només si dicc té parell amb clau == k.
    bool existeix(const Clau &k) const throw();

    // Retorna valor associat a clau k si existeix parell amb clau == k. Si no llança excepció.
    Valor consulta(const Clau &k) const throw(error);

private:
    ...
};

```
### 6.2.2 Operacions adicionals

- Operacions entre dos o més diccionaris: unió, intersecció, diferència.
- Operacions específiques quan claus són strings: p.e. trobat tot string que comença amb un prefix.
- Operacions específiques quan claus admeten relació d'ordre total(operació de comparació entre claus):
    - Operacions per examinar elements en ordre de/creixent.
    - Operacions per posició (p.e. consultar i-èssim)
    - p.e. Eliminar tot element que clau sigui entre dos claus k1 i k2.

## Diccionaris recorribles

Relació ordre total == tenim operació de comparació '<'
If(existeix relació ordre total entre claus) Pot ser útil mètodes per examinar elements.
p.e. Examinar elements en ordre decreixent de les claus.

```
template<typename Clau, typename Valor>
class diccRecorrible{
public:
    typedef pair<Clau, Valor> pair_cv;
    ...
    // Retorna list amb tot pair_cv del dicc en ordre ascendent.
    void llista_dic(list<pair_cv> &l) const throw(error);

    // f(x)'s per recòrrer claus en ordre ascendent amb punt d'interés.
    void principi() throw(error);
    void avança() throw(error);
    pair_cv actual() const throw(error);
    bool final() const throw();
    ...
};
```
Recorreguts també poden ser amb iteradors.
```
template<typename Clau, typename Valor>
class diccRecorrible{
public:
    typedef pair<Clau, Valor> pair_cv;
    ...
    // Iterador del dicc amb mètodes habituals
    friend class iterador {
    public:
        friend class diccRecorrible;
        iterador();
        ...

        // Accedeix al parell clau-valor apuntat per iterador.
        pair_cv operator*() const throw(error);

        // Pre- i postincrement; avancen iterador.
        iterador& operator++() throw();
    
        // Operadors de comparació.
        bool operator==(const iterator &it) const throw();
        ...
    };

    // Iteradors al principi i al final (sentinella) del dicc.
    iterador principi() const throw();
    iterador final() const throw();
};
```

## 6.4 Usos del diccionari

p.e.
- Taula de símbols (variables, funcions, constants ...) d'un compilador.
- Taula per emmagatzemar nodes d'un graf
- Taules i índex per sistema gestor de bases de dades
- ...

### Objectiu dels diccionaris

Estructures lineals -> accés consecutiu a elements.
Diccionaris -> accés individual a elements. inserció, elimina, consulta i existeix tinguin cost menor de O(n).

## 6.5 Implementació 
Vector indexat per les claus: if(claus == enters) && if(nombre no gaire gran) && if(consecutives dins rang: ClauMIN .. ClauMAX).

Llista enllaçada desordenada
:
## 6.5 Implementació 

#### Vector indexat per les claus: 
if(claus == enters) && if(nombre no gaire gran) && if(consecutives dins rang: ClauMIN .. ClauMAX).

#### Llista enllaçada desordenada: 
Cada node de llista conté clau-valor.
Cost inserció, eliminació i consultes O(n). Espaial O(n).
Algorismes simples. 
Dicc petit. No bona idea si operacions recorregut ordenat.

#### Llista enllaçada desordenada amb autoorganització

Cost cas pitjor == llista enllaça desordenada.
Cost mig -> pot millorar molt si algunes claus es cerquen molt més sovint que altres.

#### Llista ordenada seqüencial dins d'un vector

Dicc estàtic o insercions i eliminacions poc freqüents.
Consultes O(log (n)) amb cerca dicotòmica o binària.
Suporta recorreguts eficients ordenats.

#### Llista enllaçada ordenada
Cost inserció, eliminació i consulta O(n) en cas pitjor i mig.Avantatge -> Permet implementar fàcilment operacions de recorregut ordenat del diccionari i d'unió, intersecció i diferencia entre diccionaris.

#### Altres implementacions:

- Arbres de cerca (BST's o AVL's)
- Taules de dispersió (Hashing Tables)
- Arbres digitals (tries i variants com TST's)

BST = Binary Search Tree
TST = Ternary Search Tree
AVL = ...

## Arbres binaris de cerca

### 6.6.1 Definició i exemples

*Arbre binari de cerca (BST)*
Arbre buit o arbre binari tal que, per tot node:
- Clau node és més gran que qualsevol clau subarbre esquerre 
- Clau node és més petita que qualsevol clau de subarbre dret
- No és necessari que BST sigui complet ni ple. 
- Quan s'empra BST per implementar un diccionari que guarda parells <clau, valor>, dins cada node de l'arbre es guarda el valor associat a la clau del node.

### 6.6.2 Especificació

```cpp
template <typename Clau, typename Valor>
class dicc{
public:
    void insereix(const Clau &k, const Valor &v) throw(error);
    void elimina(const Clau &k) throw();
    void consulta(const Clau &k, bool &hi_es, Valor &v) 
        const throw(error);
    ...

private:
    struct node {
        Clau _k;
        Valor _v;
        node* _esq;    
        node* _dret;
        node(const Clau &k, const Valor &v, node *esq = nullptr, 
            node* dret = nullptr) throw(error);
    };

    node *_arrel;

    // Mètodes privats
    static node* consulta_bst(node *n, const Clau &k) throw();
    static node* insereix_bst(node *n, const Clau &k,
        Valor& v) throw(error);
    static node* elimina_bst(node *n, const Clau &k) throw();
    static node* ajunta(node *t1, node *t2) throw();
    static node* elimina_maxim(node *n) throw();
    ...
};
```

### 6.6.3 Operacions i cost associat

#### 6.6.3.1 a) Consultar una clau k:

'k' coincideix amb arrel?
if(coincideix) trobat; //cerca s'acaba
if(k < clau arrel) node = node->f_esq
if(k > clau arrel) node = node->f_dret
Cost O(h) ; h = alçada arbre ; Cas pitjor h = n;
Implementació a la pàg 20 del pdf de temari ESIN.

#### 6.6.3.2 b) Obtenir llista ordenada de tot element del BST.

fill_esq < node < fill_dret -> Cal recórrer l'arbre en inordre:
- Primer els elements del subarbre en inordre
- Després l'arrel
- Després

#### 6.6.3.3 c) Mínim i Máxim
#### 6.6.3.4 d) Inserir un element
#### 6.6.3.5 e) Eliminar un element 

### 6.6.4 Altres algorismes sobre BSTs

# Resum Tema 6 - Diccionaris

## 6.1. Conceptes

f : K = -> V ; (K) = Claus ; (V) = Valors
Funció total: Totes les claus tenen valor associat.
Funció parcial: No tota clau té valor associat.
*Funció parcial pot transformar-se en total associant valor indefinit a tota clau sense informació associada.*

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

- *Estàtic*: N són coneguts. Prohibit insercions/eliminacions.
Té constructor amb vector/llista d'elements.
- *Semidinàmic*: Permet inserció i modificació del valor associat a clau. Prohibides eliminacions.
- *Dinàmic*: Permet inserció, modificació i eliminacions.

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

- *Operacions entre dos o més diccionaris*: unió, intersecció, diferència.
- *Operacions específiques quan claus són strings*: p.e. trobat tot string que comença amb un prefix.
- *Operacions específiques quan claus admeten relació d'ordre total*(operació de comparació entre claus):
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

*Estructures lineals -> accés consecutiu a elements*
*Diccionaris -> accés individual a elements*
inserció, elimina, consulta i existeix tinguin cost menor de O(n).

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

## 6.6 Arbres binaris de cerca

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

Recorregut en inordre (fill_esq < node < fill_dret) O(n)

#### 6.6.3.3 c) Mínim i Máxim

Mínim == 1r element en inordre == Fill més a l'esquerra
Maxím == Últim element en inordre == Fill més a la dreta
Cost == O(h)

#### 6.6.3.4 d) Inserir un element

Sempre al inserir, s'insereix una fulla.
O es troba l'element (i no s'insereix)
O es troba lloc on insertar
Algoritme molt semblant a una cerca

[Veure implementacions a pàgines 23->25 de la documentació d'ESIN.]

#### 6.6.3.5 e) Eliminar un element

Operació més complexa dels BST.
Diverses posibles situacions:
a) *Eliminar fulla* -> Simplement s'elimina el subarbre.
b) *Eliminar node amb un fill* -> Enllaçar pare de node eliminat amb únic fill.
c) *Eliminar node amb dos fills* -> Dues alternatives
    c.1) Canviar node a eliminar per successor i eliminar successor
    c.2) Canviar node a eliminar per predecessor i eliminar predecessor.
    IMPORTANT: és millor alternar entre mètode c.1 i c.2 per millorar distribució del BST. Veure pàgina 29.

[Implementacions a les pàgines 27 -> 29]

### 6.6.4 Altres algorismes sobre BSTs

BSTs permeten diversos algorismes simples amb costos mitjos O(h)

p.e.1
retorna llista ordenada amb tot element amb 'k' que k1 <= k <= k2.

```cpp
template <typename Clau, typename Valor>
void dicc<Clau, Valor>::llista_interval(const Clau &k1,
    const rllista_interval(_arrel, k1, k2, L);

template <typename Clau, typename Valor>
static void dicc<Clau, Valor>::rllista_interval(node *n, const Clau &k1,
    const Clau &k2, list<Valor> &L) throw(error)
{
    if(n != nullptr) {
        if(k1 <= n->_k) rllista_interval(n->_esq, k1, k2, L);
        if(k1 <= n->_k && n->_k <= k2) L.push_back(n->_v);
        if(k2 >= n->_k) rllista_interval(n->_dret, k1, k2, L);
    }
}

```
[A la pàgina 31 hi ha un altre exemple: Consulta o eliminació d’un element del BST per posició]

## 6.7 BSTs equilibrats (AVLs) (Adelson-Velsky i Landis)

### 6.7.1 Definició i exemples

Cost en majoria mètodes BST són O(h).
En pitjor cas, BST de n elements pot tenir alçada h = n. Operacions tindrien cost lineal O(n) ; n == # nodes
Dos opcions per millorar costos:
1. No fer res perquè es pot demostrar que si un BST introduïm elements de manera aleatòria, alçada d'arbre és O(log(n))

2. Podem forçar insercions i suspressions dins arbre mantinguin alçada log(n). Elements, +/- repartits equilibradament. Cost operacions serà logarítmic perquè alçada log(n).

Factor d'equilibri d'un node = |altura(fill_esquerre - altura(fill_dret|

Diferència màxima alçades dels subarbres d'un node <= 1.

### 6.7.2 Inserció en un arbre AVL

Dues etapes:
- Inserir tenint en compte -> BST
    -> if(node < arrel) baixar per esquerra; else per dreta.
- Cal assegurar arbre queda equilibrat. Reestructurar si cal.

*Desequilibri*
Subarbre actual = A
1. Subarbre dret d'A té h + 1, mentre Subarbre esquerre té h.
    Inserció anirà a dreta.
    S'incrementa en 1 alçada subarbre dret.
2. Subarbre esquerre d'A té h + 1, subarbre dret té h.
    Inserció anirà a esquerra.
    S'incrementa en 1 alçada subarbre esquerre.

Ambdues situacions són simètriques. Només ens centrarem en la 1a. Dos subcasos:
- Cas DD(Dreta-Dreta)
- Cas DE(Dreta-Esquerre)

#### 6.7.2.1 Cas Dreta-Dreta

Node s'insereix en subarbre dret de subarbre dret. Recorregut en inordre abans i desprès de reestructuració és el mateix.

#### 6.7.2.2 Cas Dreta-Esquerre

Node s'insereix en subarbre esquerre del subarbre dret. Reestructuració en dos passos:

1. Rotació cap a dreta voltant node B.
2. Rotació cap esquerre voltant node A.

[Veure figura 6.10 i 6.11 en pàgina 40, Tema 6, Teoria ESIN]

### 6.7.3 Supressió en AVL

Els mateixos dos casos de l'inserció però al invers, reduïnt les alçades.

1. Subarbre dret d'A té h + 1. Subarbre esquerre té h. Node a eliminar en subarbre esquerre.
2. Subarbre esquerre d'A té h + 1. Subarbre dret té h. Node a eliminar en subarbre dret -> decrement en 1 de la seva alçada.

Ambdues situacions simètriques. Ens centrem en la 1a.

#### 6.7.3.1 Cas DD (Dreta-Dreta) amb manteniment d'altura

B = Subarbre dret de A
If(dos subarbres de B tenen mateixa alçada) ens trobem en desequilibri DD del cas d'inserió, es resol amb mateixa manera.
Arbre resultant té mateixa alçada abans i després de supressió. Amb una rotació s'arregla.

#### 6.7.3.2 Cas DD (Dreta-Dreta) amb disminució d'altura

B = subarbre dret de A
If(alçada subarbre esquerre de B < alçada subarbre dret de B) rotació igual que abans; però alçada és una unitat més petita que abans de la supressió. Obliga a examinar si algun subarbre que l'engloba també es desequilibra.

#### 6.7.3.3 Cas DE (Dreta-Esquerre)

B = subarbre dret de A
if(alçada subarbre esquerre de B > alçada subarbre dret de B) rotació similar a cas Dreta-Esquerre. Alçada resultat és una unitat més petita que abans de supressió. Obliga a examinar si algun subarbre que l'engloba també es desequilibra.

## 6.8 Algorisme d'ordenació quicksort

### 6.8.1 Introducció

Quicksort empra "divideix i venceràs" però no assegura divisió en parts de mida similar.
Quicksort parteix. Donat un element *p* (1r) anomenat *pivot*, reorganitzem els elements col·locant a l'esquerre del pivot els més petits i a dreta els més grans.

p | <= p |              | >= p      |
|        |              |           |
u        i              j           v

<= p        | p | >= p              |
|           |   |                   |
u               j  i                v

Després només caldrà ordenar trossos a esquerre i dreta de pivot fent dues crides recurisves a quicksort.
Fusió no és necessària, trossos a ordenar queden ben repartits.
Mentre mergesort la partició és simple i la feina important és a la fusió, quicksort és tot el contrari

### 6.8.2

```cpp
template <typename T>
void quicksort(T A[], nat u, nat v)
{
    if(v-u+1 <= M)
    {
        //Emprar algorisme d'ordenació simple
        //p.e.
    }
    else
    {
        nat k = particio(A, u, v);
        quicksort(A, u, k-1);
        quicksort(A, k+1, v);
    }
}

template<typename T>
nat particio(T A[], nat u, nat v)
{
    nat i, j;
    T p = A[u];
    i = u+1;
    j = v;

    while(i < j+1)
    {
        while(i<j+1 && A[i]<=p) ++i;
        while(i<j+1 && A[j]>=p) --j;
        if(i < j+1)
        {
            T aux = A[j]; //intercanvi
            A[j] = A[i];
            A[i] =  aux;
        }
        T aux = A[u];   //intercanvi pivot i last dels menors
        A[u] = A[j];
        A[j] = aux;
        return j;
    }
}
```

### 6.8.3 Cost

Cas pitjor -> O(n²) | Un dels trossos té molts pocs elements, altre té gairebé tots. Ja estava ordenat de/creixentment.
Si ordenem vector desordenat, pivot +/- centrat a meitat de vector.
- Cas similar a mergesort.
- Cost 1 iteració de quicksort (partició) -> lineal
- Dues crides amb trossos +/- a meitat de l'original, cost quasi-lineal -> O(n * log(n))

## 6.9 Taules de dispersió

### 6.9.1 Definició

*Taula de dispersió (hash table / taula d'adreçament calculat)* emmagatzema conjunt d'elements identificats amb una clau:
- Dins d'una taula D[0 .. M - 1]
- Mitjançant *funció de dispersió (hash function, h())* que va de conjunt de claus *K* fins conjunt de posicions de la taula *D*.:
    - claus -> valors de dispersió (posicions taula)
    - h: K -> 0 .. M - 1

Idealment h() seria injectiva. Però, pot existir dues claus amb mateixa posició a la taula.

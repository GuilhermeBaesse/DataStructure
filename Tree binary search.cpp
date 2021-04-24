#include <iostream>
#include <queue>

using namespace std;

typedef int Dado;

enum Posicao
{
    DIR,
    ESQ
};

class Noh
{
    friend class ABB;

public:
    Noh(Dado d);
    bool FilhoDaDireita();
    Dado MenorRecursivo();
    bool Sucessor(Dado *ptResultado);

private:
    Dado valor;
    Noh *esq;
    Noh *dir;
    Noh *pai;
};

class ABB
{
public:
    ABB() { raiz = NULL; }
    ~ABB();
    Noh *Busca(Dado d);
    void EscreverNivelANivel(std::ostream &saida);
    void Inserir(Dado d);
    bool Sucessor(Dado d, Dado *ptResultado);
    void destruirRecursivamente(Noh *atual);
    void posOrdem() { posOrdem(raiz); }
    void posOrdem(Noh *atual);

private:
    Noh *raiz;
};

Noh::Noh(Dado d)
{
    esq = NULL;
    dir = NULL;
    pai = NULL;
    valor = d;
}
bool Noh::FilhoDaDireita()
{
    Noh *aux = this;
    if (aux->dir == NULL)
        return false;
    return true;
}

Dado Noh::MenorRecursivo()
{
    Noh *SubArvore = this;
    if (SubArvore->esq == NULL)
    {
        return valor;
    }
    return esq->MenorRecursivo();
}

bool Noh::Sucessor(Dado *ptResultado)
{
    if (FilhoDaDireita())
    {
        *ptResultado = dir->MenorRecursivo();
        return true;
    }

    return false;
}

ABB::~ABB()
{
    destruirRecursivamente(raiz);
}
void ABB::destruirRecursivamente(Noh *atual)
{
    if (atual != NULL)
    {
        posOrdem(atual->esq);
        posOrdem(atual->dir);
        delete atual;
    }
}
void ABB::posOrdem(Noh *atual)
{
    if (atual != NULL)
    {
        posOrdem(atual->esq);
        posOrdem(atual->dir);
    }
}

void ABB::Inserir(Dado d)
{
    Noh *novo = new Noh(d);
    if (raiz == NULL)
    {
        raiz = novo;
    }
    else
    {
        Noh *atual = raiz;
        Noh *anterior = NULL;

        while (atual != NULL)
        {
            anterior = atual;
            if (atual->valor > d)
            {
                atual = atual->esq;
            }
            else
            {
                atual = atual->dir;
            }
        }
        novo->pai = anterior;
        if (anterior->valor > novo->valor)
        {
            anterior->esq = novo;
        }
        else
        {
            anterior->dir = novo;
        }
    }
}

bool ABB::Sucessor(Dado d, Dado *ptResultado)
{

    Noh *chave = Busca(d);
    if (chave->FilhoDaDireita() == false)
    {
        while (chave->valor <= d and chave != raiz)
        {
            chave = chave->pai;
        }

        if (chave->valor > d)
        {
            *ptResultado = chave->valor;
            return true;
        }
        return false;
    }
    chave->Sucessor(ptResultado);
    return true;
}

Noh *ABB::Busca(Dado d)
{
    Noh *atual = raiz;
    while (atual != NULL)
    {
        if (atual->valor == d)
            return atual;
        else if (atual->valor > d)
        {
            atual = atual->esq;
        }
        else
        {
            atual = atual->dir;
        }
    }
    return atual;
}

void ABB::EscreverNivelANivel(ostream &saida)
{
    queue<Noh *> filhos;
    Noh noh = Dado();
    Noh *fimDeNivel = &noh;
    filhos.push(raiz);
    filhos.push(fimDeNivel);
    while (not filhos.empty())
    {
        Noh *ptNoh = filhos.front();
        filhos.pop();
        if (ptNoh == fimDeNivel)
        {
            saida << "\n";
            if (not filhos.empty())
                filhos.push(fimDeNivel);
        }
        else
        {
            saida << '[';
            if (ptNoh != NULL)
            {
                saida << ptNoh->valor;
                filhos.push(ptNoh->esq);
                filhos.push(ptNoh->dir);
            }
            saida << ']';
        }
    }
}

int main()
{
    ABB arvore;
    Dado chave;
    char operacao;
    do
    {
        cin >> operacao;
        switch (operacao)
        {
        case 'i':
            cin >> chave;
            arvore.Inserir(chave);
            break;
        case 'e':
            arvore.EscreverNivelANivel(cout);
            break;
        case 's':
        {
            cin >> chave;
            Dado sucessor;
            while (arvore.Sucessor(chave, &sucessor))
            {
                cout << sucessor << ' ';
                chave = sucessor;
            }
            cout << endl;
            break;
        }
        }
    } while (operacao != 'f');
    return 0;
}
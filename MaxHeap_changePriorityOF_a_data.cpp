#include <iostream>
#include <stdexcept>

using namespace std;

typedef int dado; // typedef permite criar novos tipos a partir de outros
typedef int posicao;

const posicao INVALIDO = -1;
const dado VALOR_PADRAO = -1;

class noh
{
    friend class listarranjo;

private:
    dado elemento; // poderia ser outro tipo de variável
    posicao proximo;
    posicao anterior;
    noh() : elemento(VALOR_PADRAO), proximo(INVALIDO), anterior(INVALIDO) {}
};

// lista dinamicamente encadeada
class listarranjo
{
private:
    noh *vetorDados; // vetor para armazenamento da lista
    posicao primeiro;
    posicao ultimo;
    unsigned tamanho;
    posicao primeiroApagado;
    unsigned capacidade;
    // "Aloca" para uso, se houver. Retorna a posicao alocada e informa se foi possível alocar.
    bool alocaPosicao(posicao *ptPosAlocada);

public:
    listarranjo(unsigned cap);
    ~listarranjo();
    // inserção, remoção e procura
    bool insereNoInicio(dado d);
    bool insereNoFim(dado d);
    bool removeNoInicio();
    bool removeNoFim();
    void imprime();
    void depura();
};

// constrói uma lista inicialmente vazia
listarranjo::listarranjo(unsigned cap)
{
    capacidade = cap;
    vetorDados = new noh[cap];
    tamanho = 0;
    primeiro = INVALIDO;
    ultimo = INVALIDO;
    primeiroApagado = 0;
    unsigned ult = cap - 1;
    for (unsigned i = 0; i < ult; ++i)
        vetorDados[i].proximo = i + 1;
    vetorDados[ult].proximo = INVALIDO;
}

// destrutor da lista
listarranjo::~listarranjo()
{
    delete[] vetorDados;
}

// Retorna uma posicao alocada para uso. Retorna tambem um booleano que indica
// se foi possivel fazer a alocacao.
bool listarranjo::alocaPosicao(posicao *ptPosAlocada)
{
    if (tamanho == capacidade)
        return false;
    *ptPosAlocada = primeiroApagado;
    primeiroApagado = vetorDados[primeiroApagado].proximo;
    return true;
}

// Insere no início da lista. Retorna um booleano que indica se foi possivel fazer a insercao.
bool listarranjo::insereNoInicio(dado d)
{
    posicao aux = primeiro;
    posicao aux2 = primeiroApagado;
    // verifica se ainda cabe um novo elemento
    if (alocaPosicao(&primeiro) == false)
    {
        return false;
    }
    // se a lista estiver vazia, inserimos o último nó
    if (tamanho == 0)
    {
        ultimo = primeiro;
        vetorDados[primeiro].elemento = d;
        vetorDados[primeiro].anterior = INVALIDO;
        vetorDados[ultimo].proximo = INVALIDO;
        tamanho++;
        return true;
    }

    // já tem elementos na lista, insere no início
    vetorDados[aux].anterior = aux2; // o anterior do primeiro antigo  é onde o novo primeiro.
    vetorDados[primeiro].elemento = d;
    vetorDados[primeiro].proximo = aux; // o proximo do meu novo primeiro é o primeiroAntigo.
    vetorDados[primeiro].anterior = INVALIDO;
    tamanho++;
    return true;
}

// Insere no final da lista. Retorna um booleano que indica se foi possivel fazer a insercao.
bool listarranjo::insereNoFim(dado d)
{
    posicao aux = ultimo;
    posicao aux2 = primeiroApagado;
    // verifica se ainda cabe um novo elemento
    if (alocaPosicao(&ultimo) == false)
    {
        return false;
    }
    // se a lista estiver vazia, inserimos o primeiro nó
    if (tamanho == 0)
    {
        primeiro = 0;
        vetorDados[ultimo].elemento = d;
        vetorDados[primeiro].anterior = INVALIDO;
        vetorDados[ultimo].proximo = INVALIDO;
        tamanho++;
        return true;
    }
    vetorDados[aux].proximo = ultimo; // o proximo do ultimo antigo é o novo ultimo
    vetorDados[ultimo].elemento = d;
    vetorDados[ultimo].proximo = INVALIDO;
    vetorDados[ultimo].anterior = aux; // o anterior no novo ultimo é o antigo ultimo
    tamanho++;
    return true;

    // se já tem elementos na lista, insere no final
}

bool listarranjo::removeNoInicio()
{

    if (tamanho == 0)
        return false;

    posicao aux = vetorDados[primeiro].proximo;
    vetorDados[primeiro].proximo = primeiroApagado;
    primeiroApagado = primeiro;
    primeiro = aux;
    vetorDados[primeiro].anterior = INVALIDO;
    tamanho--;
    if (tamanho == 0)
    {
        ultimo = INVALIDO;
    }
    return true;

    // remoção simplesmente marca a posição como inválida
    // e diminui tamanho da lista

    // precisa verificar se não está removendo o últimor
}

bool listarranjo::removeNoFim()
{

    if (tamanho == 0)
        return false;

    posicao aux = vetorDados[ultimo].anterior;
    vetorDados[ultimo].proximo = primeiroApagado;
    primeiroApagado = ultimo;
    ultimo = aux;
    vetorDados[ultimo].proximo = INVALIDO;
    tamanho--;
    if (ultimo == INVALIDO)
    {
        primeiroApagado = primeiro;
        primeiro = INVALIDO;
    }

    return true;
    // remoção simplesmente marca a posição como inválida

    // e diminui tamanho da minhaLista

    // precisa verificar se não está removendo o último
}

// método básico que *percorre* a lista, imprimindo seus elementos
void listarranjo::imprime()
{
    posicao aux = primeiro;

    while (aux != INVALIDO)
    {
        cout << vetorDados[aux].elemento << " ";
        aux = vetorDados[aux].proximo;
    }
    cout << endl;

    // imprime reverso (para mostrar duplo encadeamento)
    aux = ultimo;
    cout << ultimo;
    while (aux != INVALIDO)
    {
        cout << vetorDados[aux].elemento << " ";
        aux = vetorDados[aux].anterior;
    }
    cout << endl;
}

// método básico que imprime o vetor de dados, para depuração
void listarranjo::depura()
{
    cout << tamanho << "/{" << primeiro << "-" << ultimo << "}/" << primeiroApagado << ":";
    for (unsigned i = 0; i < capacidade; i++)
    {
        cout << "(" << vetorDados[i].anterior
             << ",[" << vetorDados[i].elemento << "],"
             << vetorDados[i].proximo << ")";
    }
    cout << endl;
}

int main()
{
    unsigned cap;
    cin >> cap;
    listarranjo minhaLista(cap);
    char operacao;
    dado valor;
    do
    {
        cin >> operacao;
        switch (operacao)
        {
        case 'I': // inserir no início
            cin >> valor;
            if (not minhaLista.insereNoInicio(valor))
                cout << "Lista cheia, incapaz de inserir\n";

            break;
        case 'i': // inserir no fim
            cin >> valor;
            if (not minhaLista.insereNoFim(valor))
                cout << "Lista cheia, incapaz de inserir\n";
            break;
        case 'R': // remover no início
            if (not minhaLista.removeNoInicio())
                cout << "Lista vazia, incapaz de remover\n";
            break;
        case 'r': // remove no fim
            if (not minhaLista.removeNoFim())
                cout << "Lista vazia, incapaz de remover\n";
            break;
        case 'p': // mostrar estrutura
            minhaLista.imprime();
            break;
        case 'd': // depura vetor de dados
            minhaLista.depura();
            break;
        case 's': // sair
            // será tratado no while
            break;
        default:
            cout << "Opção inválida!" << endl;
        }
    } while (operacao != 's');

    return 0;
}
// Tabela Hash com tratamento de colisão por endereçamento aberto
#include <iostream>
#include <string>
#include <stdexcept>

typedef int TValor;

class InfoHash
{
    friend class Hash; // essa classe só deve ser usada na classe Hash
    InfoHash();
    InfoHash(const std::string &chave, const TValor &valor);
    std::string mChave;
    TValor mValor;
};

class Hash
{
    friend void verifica(unsigned tam);

public:
    Hash(unsigned capacidade = 50);
    ~Hash();
    void EscreverEstrutura(std::ostream &saida) const;
    void Inserir(const std::string &chave, const TValor &valor);
    void Remover(const std::string &chave);
    TValor Valor(const std::string &chave) const;

protected:
    unsigned Buscar(const std::string &chave) const;
    void Inserir(InfoHash *ptPar);
    unsigned Posicao(const std::string &chave) const;
    // Altera a capacidade da estrutura.
    void Redimensionar(unsigned novaCapacidade);
    InfoHash **mVetPtDados;
    InfoHash *REMOVIDO;
    unsigned mCapacidade;
    unsigned mTamanho;
};

using namespace std;

InfoHash::InfoHash()
    : mChave(), mValor()
{
}

InfoHash::InfoHash(const std::string &chave, const TValor &valor)
    : mChave(chave), mValor(valor)
{
}

Hash::Hash(unsigned capacidade) // capacidade tem valor default
    : mVetPtDados(new InfoHash *[capacidade]), REMOVIDO(new InfoHash()), mCapacidade(capacidade), mTamanho(0)
{
    for (unsigned i = 0; i < capacidade; i++)
    {
        mVetPtDados[i] = NULL;
    }
}

Hash::~Hash()
{
    for (int i = 0; i < mTamanho; i++)
    {
        mVetPtDados[i] = NULL;
    }
    delete REMOVIDO;
    delete[] mVetPtDados;
}

unsigned Hash::Buscar(const std::string &chave) const
{
    unsigned contador = 0;
    unsigned posicao_aux = Posicao(chave);
    unsigned posicao_chave = 0;
    bool buscando = true;
    bool encontrei = false;
    while (contador < mCapacidade and buscando == true)
    {
        if ((mVetPtDados[posicao_aux] != NULL) and (mVetPtDados[posicao_aux] != REMOVIDO))
        {
            if (mVetPtDados[posicao_aux]->mChave == chave and encontrei == false)
            {
                posicao_chave = posicao_aux;
                encontrei = true;
            }
            else
            {
                if (posicao_aux + 1 == mCapacidade)
                {
                    posicao_aux = 0;
                }
                else
                {
                    posicao_aux++;
                }
                contador++;
            }
        }

        if ((mVetPtDados[posicao_aux] != NULL) and (mVetPtDados[posicao_aux] == REMOVIDO))
        {
            if (posicao_aux + 1 == mCapacidade)
            {
                posicao_aux = 0;
            }
            else
            {
                posicao_aux++;
            }
            contador++;
        }
        if ((mVetPtDados[posicao_aux] == NULL) and (mVetPtDados[posicao_aux] != REMOVIDO))
        {
            //busca tem que parar quando encontrar posicao vazia
            buscando = false;
        }
    }
    if ((mVetPtDados[posicao_chave]->mChave == chave) and (encontrei == true))
    {
        return posicao_chave;
    }
    else
    {
        throw invalid_argument("ERRO");
    }

    // Retorna a posicao em que uma chave está armazenada na estrutura. Protegido.
}

void Hash::EscreverEstrutura(std::ostream &saida) const
{
    // Mostra todos as posições de armazenamento da hash. Ajuda a debugar.
    for (unsigned i = 0; i < mCapacidade; ++i)
    {
        saida << '[' << i;
        if (mVetPtDados[i] != NULL)
        {
            if (mVetPtDados[i] == REMOVIDO)
                saida << "/removido";
            else
                saida << '/' << mVetPtDados[i]->mChave << '/' << mVetPtDados[i]->mValor;
        }
        saida << "] ";
    }
}

void Hash::Inserir(const string &chave, const TValor &valor)
{
    // Insere uma cópia do valor. Não permite inserção de chave repetida.
    if (mTamanho < mCapacidade)
    {
        InfoHash *novo = new InfoHash(chave, valor);
        int posicao_aux = Posicao(chave);
        while (mVetPtDados[posicao_aux] != NULL and mVetPtDados[posicao_aux]->mChave != chave and mVetPtDados[posicao_aux] != REMOVIDO)
        {
            posicao_aux++;
            posicao_aux %= mCapacidade;
        }
        if (mVetPtDados[posicao_aux] == NULL or mVetPtDados[posicao_aux] == REMOVIDO)
        {
            mTamanho++;
            mVetPtDados[posicao_aux] = novo;
        }
        else
        {
            throw invalid_argument("ERRO");
        }
    }
    else
    {
        Redimensionar(mCapacidade + 5);
        Inserir(chave, valor);
    }
}

void Hash::Inserir(InfoHash *ptPar)
{
    unsigned posicao = Posicao(ptPar->mChave);
    unsigned i = 0;
    while (mVetPtDados[(posicao + i) % mCapacidade] != NULL)
    {
        i++;
    }
    mVetPtDados[(posicao + i) % mCapacidade] = ptPar;
}

unsigned Hash::Posicao(const string &chave) const
{
    // Retorna a posição de armazenamento de uma chave, 0 <= posicao < mCapacidade. Protegido.
    unsigned posicao = 1;
    unsigned tamanho = unsigned(chave.length());
    for (unsigned i = 0; i < tamanho; ++i)
    {
        posicao = 7 * posicao + chave[i];
    }
    return posicao % mCapacidade;
}

void Hash::Redimensionar(unsigned novaCapacidade)
{
    if (novaCapacidade < mTamanho)
    {
        throw invalid_argument("ERRO");
    }
    else
    {
        InfoHash **auxVetDados = mVetPtDados;
        unsigned antigo = mCapacidade;
        mCapacidade = novaCapacidade;
        mVetPtDados = new InfoHash *[novaCapacidade];
        for (unsigned i = 0; i < novaCapacidade; i++)
        {
            mVetPtDados[i] = NULL;
        }
        InfoHash *aux;
        for (unsigned i = 0; i < antigo; i++)
        {
            aux = auxVetDados[i];
            if (aux != NULL)
            {
                Inserir(aux);
            }
            auxVetDados[i] = NULL;
        }
        delete[] auxVetDados;
    }
}
void verifica(unsigned tam)
{
    if (tam == 0)
    {
        throw invalid_argument("ERRO");
    }
}
void Hash::Remover(const std::string &chave)
{
    verifica(mTamanho);
    unsigned posicao = Buscar(chave);
    delete mVetPtDados[posicao];
    mVetPtDados[posicao] = REMOVIDO;
    mTamanho--;
}

TValor Hash::Valor(const std::string &chave) const
{
    unsigned posicao_aux = Buscar(chave);
    return mVetPtDados[posicao_aux]->mValor;
}

int main()
{
    unsigned capacidade;
    cin >> capacidade;
    Hash tabela(capacidade);
    char operacao;
    string chave;
    TValor valor;
    do
    {
        try
        {
            cin >> operacao;
            switch (operacao)
            {
            case 'i': // inserir
                cin >> chave >> valor;
                tabela.Inserir(chave, valor);
                break;
            case 'r': // remover
                cin >> chave;
                tabela.Remover(chave);
                break;
            case 'c': // consultar
                cin >> chave;
                valor = tabela.Valor(chave);
                cout << valor << endl;
                break;
            case 'd': // debug (mostrar estrutura)
                tabela.EscreverEstrutura(cout);
                cout << endl;
                break;
            case 'f': // finalizar
                // vai testar depois
                break;
            default:
                cerr << "operação inválida\n";
            }
        }
        catch (exception &e)
        {
            // ignorar a mensagem que explica o erro e só escrever ERRO.
            cout << "ERRO" << endl;
        }
    } while (operacao != 'f');
    return 0;
}
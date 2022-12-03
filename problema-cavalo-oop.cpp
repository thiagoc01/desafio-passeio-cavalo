#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <algorithm>
#include <string>

/*
    Problema em que backtracking pode ser aplicado.
    Porém, como temos 8 movimentos disponíveis (cavalos se movimentam em L,
    cada L é para cima ou para baixo), o teste fica com complexidade O(8 ^ (8 ^ 2)).

    Podemos aplicar a heurística de busca de Warnsdorff.
    Antes de selecionar uma casa, verificamos qual casa adjacente a essa casa adjacente à origem
    possui menos casas futuras disponíveis. A intuição do funcionamento dessa heurística é o fato de diminuirmos
    nossa disponibilidade de casas, o que reduz o espaço de busca futuro.

    Sempre devemos ter essa casa disponível em um tabuleiro 8 x 8. Se isso não acontece, não há solução.
    Como devemos visitar cada casa uma vez, o problema cai para complexidade O(8 * 8), já que são apenas
    64 movimentos que devem ser realizados.

    Basicamente, o problema se tornou um caso de encontrar um circuito hamiltoniano.
    Os vértices do grafo serão as casas. E as arestas seriam as casas adjacentes.
    Cada vez que visitarmos uma casa, todos os vizinhos terão essa casa removida, o que diminuiria o grau.

    Logo, iteramos 62 vezes, já que a 63ª vez iria verificar pelo 64º movimento, que, teoricamente, é o ponto de partida.

    A cada iteração, partimos da casa atual, procuramos uma casa válida não visitada cuja as casas adjacentes à ela
    tenham o menor grau de todas. Devolvemos essa casa e vamos para o próximo movimento, até o último.

    Após retornar essa casa, colocamos no vetor de soluções a coordenada. Ao fim, imprimimos todas em ordem. 
*/

#define N 8

/* Utilizado para transformar a coordenada X em letra para a impressão na tela */

std::unordered_map<short int, char> coordenadaTraducao = 
{
    {1, 'a'},
    {2, 'b'},
    {3, 'c'},
    {4, 'd'},
    {5, 'e'},
    {6, 'f'},
    {7, 'g'},
    {8, 'h'}
};

/* Vetor com a coordenada do i-ésimo movimento */

std::vector<std::pair<char, short int>> coordenadasSolucao = std::vector<std::pair<char, short int>>(N * N);

void imprimeSolucao()
{
    for (const auto& [coordenadaX, coordenadaY] : coordenadasSolucao)
        std::cout << coordenadaX << coordenadaY << std::endl;
}

class Vertice
{
    protected:
        std::pair<short int, short int> id; // As coordenadas identificam a casa
        std::set<Vertice *> casasVizinhas;          

    public:
        Vertice(const short int& x, const short int& y)
        {
            this->id = {x, y};
        }

        std::pair<short int, short int> getID() const
        {
            return id;
        }

        void adicionaVizinho(Vertice*& v)
        {
            casasVizinhas.insert(v);
        }

        std::set<Vertice *>& retornaCasasVizinhas()
        {
            return casasVizinhas;
        }
};

class Grafo
{
    private:
        std::map<std::pair<short int, short int>, Vertice *> casas; // Tabuleiro

        /* Todos os movimentos disponíveis para um cavalo. Cada par é um deslocamento no eixo x e y para a coordenada atual, representando um L */

        const std::vector<std::pair<short int, short int>> movimentosPossiveis = 
        {    
            {2, 1},
            {2, -1},
            {1, 2},
            {1, -2},
            {-1, 2},
            {-1, -2},
            {-2, 1},
            {-2, -1}       
        };

        bool ePosicaoValida(const short int& x, const short int& y)
        {
            return x >= 1 && x <= N && y >= 1 && y <= N;    // Está nos limites do tabuleiro?
        }

        short int retornaGrau(const short int& x, const short int& y)
        {
            Vertice *v = casas.at({x, y});

            return v->retornaCasasVizinhas().size(); // A quantidade de casas vizinhas indica as casas disponíveis
        }

        Vertice* retornaProximaCoordenada(const short int& x, const short int& y)
        {
            /* Teste básico de menor valor*/

            short int testeMenorGrau, menorGrau = N + 1;
            Vertice *melhorCoordenada = casas.at({x, y});
            std::set<Vertice *> coordenadasAdjacentes = casas.at({x, y})->retornaCasasVizinhas();

            for (auto& coordenada : coordenadasAdjacentes)  // Para todas as coordenadas adjacentes à atual
            {
                /* Se o grau de uma casa adjacente a essa nova é menor que de outras, essa é a casa destino */

                auto [coordenadaX, coordenadaY] = coordenada->getID();

                if ( (testeMenorGrau = retornaGrau(coordenadaX, coordenadaY)) < menorGrau )
                {
                    melhorCoordenada = coordenada;
                    menorGrau = testeMenorGrau;
                }
            }

            if (melhorCoordenada == casas.at({x, y}))  // Não há casa disponível, retornamos a atual. O que implica que não há solução
                return casas.at({x, y});

            /* Se chegou aqui, há um movimento. Retorna essa coordenada nova */

            for (auto &coordenadaAdjacente : melhorCoordenada->retornaCasasVizinhas()) // Retira a disponibilidade dessa casa para todas as casas adjacentes
                coordenadaAdjacente->retornaCasasVizinhas().erase(melhorCoordenada);

            return melhorCoordenada;
        }

        /* Para cada casa, adiciona as casas adjacentes a ela */

        void adicionaArestas()
        {
            for (short int i = 1 ; i <= N ; i++)
            {
                for (short int j = 1 ; j <= N ; j++)
                {
                    for (short int movPossivel = 0 ; movPossivel < N ; movPossivel++)   // Considera casa adjacente se podemos ir para ela
                    {
                        auto [deslocamentoXJogada, deslocamentoYJogada] = movimentosPossiveis[movPossivel];

                        short int idX = i + deslocamentoXJogada;     // Abscissa da coordenada destino
                        short int idY = j + deslocamentoYJogada;    // Ordenada da coordenada destino

                        if (ePosicaoValida(idX, idY))
                        {
                            Vertice *v = casas.at({i, j}), *w = casas.at({idX, idY});

                            v->adicionaVizinho(w);
                        }                        
                    }
                }               
            }     
        }	

    public:
        Grafo()
        {
            /* Inicializa tabuleiro */

            for (short int linha = 1 ; linha <= N ; linha++)
            {
                for (short int coluna = 1 ; coluna <= N ; coluna++)
                {
                    std::pair<short int, short int> p = {linha, coluna};
                    casas.insert(std::make_pair(p, new Vertice({linha, coluna}))); 
                }                       
            } 

            adicionaArestas();  // Adiciona as casas adjacentes
        }        

        bool achaSolucao(short int x, short int y)
        {
            Vertice *v = casas.at({x, y});

            for (auto &coordenadaAdjacente : v->retornaCasasVizinhas()) // Retira a disponibilidade dessa casa para todas as casas adjacentes
                coordenadaAdjacente->retornaCasasVizinhas().erase(v);

            for (short int i = 1 ; i <= N * N - 1 ; i++)    // Itera até o movimento 63
            {
                Vertice *novaCoordenada = retornaProximaCoordenada(x, y);

                /* Se a coordenada nova é igual à atual, não há movimento disponível. Retorna que não há solução */

                if (novaCoordenada == v)
                    return false;

                else    // Caso contrário, atualiza posição para próximo movimento e coloca no vetor de movimentação
                {           
                    x = novaCoordenada->getID().first;
                    y = novaCoordenada->getID().second;

                    char indiceParaLetra = coordenadaTraducao[x];   // Transforma o eixo x, que é número, para letra

                    coordenadasSolucao[i] = {indiceParaLetra, y};
                }
            }

            return true;
        }     

        ~Grafo()
        {
            std::for_each(casas.begin(), casas.end(), [](std::map<std::pair<short int, short int>, Vertice *>::value_type& p) { delete p.second; });
        }
};

void trataEntrada(int argc, char **argv)
{
    /* Recebe entrada da linha de comando (assume que foi passado) */

    char xInicialChar = argv[1][0];

    short int yInicial = std::stoi(std::string(argv[1] + 1)), xInicial;

    for (const auto& [coordenadaNumero, coordenadaAlgebrica] : coordenadaTraducao)   // Transforma o eixo x de letra para número
    {
        if (coordenadaAlgebrica == xInicialChar)
        {
            xInicial = coordenadaNumero;
            break;
        }
    }

    coordenadasSolucao[0] = {xInicialChar, yInicial};   // Guarda a origem na solução

    Grafo g;

    if (g.achaSolucao(xInicial, yInicial))
        imprimeSolucao();
    
    else
        std::cout << "Não há solução" << std::endl;
}

int main(int argc, char **argv)
{
    trataEntrada(argc, argv);    

    return 0;
}
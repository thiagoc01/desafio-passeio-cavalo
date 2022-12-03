#include <iostream>
#include <vector>
#include <unordered_map>
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
    Os vértices do grafo seriam as casas. E as arestas seriam as movimentações de um cavalo dessa casa para outra.
    Cada vez que visitássemos uma casa, todos os vizinhos teriam essa casa removida, o que diminuiria o grau.

    Nesse caso, a matriz casas são os vértices. O vetor movimentosPossiveis são os deslocadores.
    Como o máximo são 8 vizinhos para cada um, calculamos a adjacência em tempo real.

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

/* Indica se a casa na linha x e coluna j foi visitada (64 casas) */

std::vector<std::vector<bool>> casas = std::vector<std::vector<bool>>(N + 1, std::vector<bool>(N + 1, false));

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

void imprimeSolucao()
{
    std::vector<std::pair<char, short int>>::iterator iterador = coordenadasSolucao.begin();

    while (iterador != coordenadasSolucao.end())
        std::cout << (*iterador).first << (*iterador++).second << std::endl;
}

bool ePosicaoValida(const short int& x, const short int& y)
{
    return x >= 1 && x <= N && y >= 1 && y <= N;    // Está nos limites do tabuleiro?
}

bool checaVisitacao(const short int& x, const short int& y)
{
    return ePosicaoValida(x, y) && !casas[x][y];    // Está nos limites e foi visitada?
}

short int retornaGrau(const short int& x, const short int& y)
{
    short int grau = 0;

    /* Conta o número de casas disponíveis a partir dessa */

	for (int i = 0; i < N; i++)
	{        
        if (checaVisitacao(x + movimentosPossiveis[i].first, y + movimentosPossiveis[i].second))
			grau++;
    }

	return grau;
}

/* Procura uma casa disponível para movimentação a qual possui o menor grau de todas as outras disponíveis */

std::pair<short int, short int> retornaProximaCoordenada(const short int& x, const short int& y)
{
    /* Teste básico de menor valor. Inicialização com -1 para posição de vetor e uma unidade a mais do último índice, que é 8 */

	short int melhorMovimento = -1, testeMenorGrau, menorGrau = N + 1, proxCoordX, proxCoordY;

	for (short int movPossivel = 0; movPossivel < N; movPossivel++) // Temos 8 movimentos para o cavalo
	{
        /* Gera nova coordenada após o salto do cavalo */

		proxCoordX = x + movimentosPossiveis[movPossivel].first;
		proxCoordY = y + movimentosPossiveis[movPossivel].second;

        /* Se a casa está disponível e o grau de uma casa adjacente a essa nova é menor que de outras, essa é a casa destino */

		if (checaVisitacao(proxCoordX, proxCoordY) && (testeMenorGrau = retornaGrau(proxCoordX, proxCoordY)) < menorGrau)
		{
			melhorMovimento = movPossivel;
			menorGrau = testeMenorGrau;
		}
	}

	if (melhorMovimento == -1)  // Não há casa disponível, retornamos a atual. O que implica que não há solução
		return {x, y};

    /* Se chegou aqui, há um movimento. Retorna essa coordenada nova */

    proxCoordX = x + movimentosPossiveis[melhorMovimento].first;
	proxCoordY = y + movimentosPossiveis[melhorMovimento].second;    

	return {proxCoordX, proxCoordY};
}

bool achaSolucao(short int x, short int y)
{    
    casas[x][y] = true; // Marca o ponto de partida do usuário como visitado

    for (short int i = 1 ; i <= N * N - 1 ; i++)    // Itera até o movimento 63
	{
        std::pair<short int, short int> novaCoordenada = retornaProximaCoordenada(x, y);

        /* Se a coordenada nova é igual à atual, não há movimento disponível. Retorna que não há solução */

        if (novaCoordenada.first == x && novaCoordenada.second == y)
			return false;

        else    // Caso contrário, atualiza posição para próximo movimento e coloca no vetor de movimentação
        {           
            x = novaCoordenada.first;
            y = novaCoordenada.second;

            casas[x][y] = true; // Visita casa

            char indiceParaLetra = coordenadaTraducao[x];   // Transforma o eixo x, que é número, para letra

            coordenadasSolucao[i] = {indiceParaLetra, y};
        }
    }

	return true;
}

void trataEntrada(int argc, char **argv)
{
    /* Recebe entrada da linha de comando (assume que foi passado) */

    char xInicialChar = argv[1][0];

    short int yInicial = std::stoi(std::string(argv[1] + 1)), xInicial;

    for (const auto& coordenada : coordenadaTraducao)   // Transforma o eixo x de letra para número
    {
        if (coordenada.second == xInicialChar)
        {
            xInicial = coordenada.first;
            break;
        }
    }

    coordenadasSolucao[0] = {xInicialChar, yInicial};   // Guarda a origem na solução

    if (achaSolucao(xInicial, yInicial))
        imprimeSolucao();
    
    else
        std::cout << "Não há solução" << std::endl;
}

int main(int argc, char **argv)
{
    trataEntrada(argc, argv);

    return 0;
}

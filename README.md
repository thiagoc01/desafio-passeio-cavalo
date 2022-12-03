# Problema do passeio do cavalo


##### Repositório com os programas que encontram a solução do problema. Um deles encontra diversas soluções.


&nbsp;

## Como funciona?

Ambas as versões modelam o tabuleiro para um grafo, onde as casas são os vértices e as arestas são as casas
adjacentes disponíveis para a movimentação de um cavalo a partir de outra casa. Uma versão implementa utilizando
classes e a outra com matriz simples. O problema consiste em encontrar um circuito hamiltoniano. A versão
que implementa backtracking para encontrar tal circuito é extremamente custosa. Em ambos os programas,
utiliza-se a heurística de Warnsdorff, que tem complexidade linear em N, onde N é a dimensão de uma linha ou coluna
do tabuleiro.

## Formato da entrada

A entrada é fornecida via linha de comando. Ela é o ponto de partida do cavalo em coordenada algébrica (a1, a2, ..., h7, h8).

## Como compilar?

##### Baixe o código ou clone o repositório com:

```
$ git clone https://github.com/thiagoc01/desafio-passeio-cavalo.git
```

##### Dentro do diretório, execute o compilador g++:

```
$ g++ problema-cavalo-matriz.cpp -o <nome do executável>
```

ou

```
$ g++ problema-cavalo-oop.cpp -o <nome do executável>
```

##### Execute o programa com:

```
$ ./<nome do executável> <coordenada algébrica>
```

Ex.:

```
$ ./cavalo a1
```

O programa irá imprimir todos os movimentos feitos pelo cavalo em coordenadas algébricas até visitar a última casa.

:warning: A versão oop encontra diversas soluções!

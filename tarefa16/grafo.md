# Representando o problema com um grafo

## O que significa cada elemento?
Temos que cada cela e cada jogador são um vértice do grafo. As arestas são de tamanhos variáveis, representando a distância entre dois vértices, ou seja, a quantidade de rodadas que são necessárias para que um personagem entre numa cela, por exemplo. Se o personagem já pode estar na cela, então não há aresta.

## A matriz
Foi utilizada uma matriz para representar o tamanho de cada aresta. Tal tamanho foi calculado como a distancia entre dois vertices, baseada nas regras do jogo. Se a aresta não existir, seu tamanho é 0.

## Saída
Na matriz, se todas as posições da linha/coluna associada a uma cela forem menores ou iguais a 2, então sabemos que todos os personagens conseguem chegar nessa cela em no máximo duas rodadas, possibilitando o encontro do grupo. Colocando em termos do grafo, temos que se todas as arestas partindo de um vertice tem tamanho menor ou igual a 2, então esse vertice pode se tornar coincidente com suas adjacencias em no máximo duas rodadas.
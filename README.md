# Trabalho 1 - Teoria dos Grafos

## Execução do programa

Para executar o programa, compile usando:

g++ -o menorcaminho main.cpp

e execute usando:

./menorcaminho grafo.txt s

onde s é o nó inicial dos caminhos e grafo.txt é o arquivo que contém o grafo

## Execução dos testes

Para utilizar o codigo de teste implementado, compile usando:

g++ -o test test.cpp

Antes de executar, crie dois arquivos de texto, um contendo os caminhos para os arquivos de saida gerados pelo programa,
um em cada linha. No outro arquivo, coloque os caminhos dos gabaritos correspondentes, seguindo o exemplo a seguir:

out.txt:                        gabarito.txt

./out/g1-out4.txt               ./exemplos/g1-out4.txt
./out/g1-out6.txt               ./exemplos/g1-out4.txt
./out/g1.out8.txt               ./exemplos/g1-out4.txt

O arquivo presente na primeira linha do arquivo out.txt será comparado ao arquivo na primeira linha do arquivo gabarito.txt

Portanto os dois devem ter a mesma quantidade de linhas.

execute usando o seguinte comando:

./test out.txt gabarito.txt
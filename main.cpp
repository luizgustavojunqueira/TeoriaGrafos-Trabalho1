#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <climits>
#include <set>

using namespace std;

//Classe usada para armazenar os caminhos e formatar corretamente para a saída
class Path{
    public:
        int origem;
        int destino;
        int custo;
        int comprimento;
        bool temCaminho = true;
        vector<int> caminho;

        Path(int t, int comprimento, int custo, int origem): destino(t), comprimento(comprimento), custo(custo), origem(origem){}

        //Constroi o caminho da origem ao destino utilizando o vetor de anteriores calculado pelo algoritmo de Dijkstra
        void constroiCaminho(vector<int> anteriores){
            int t = this->destino;

            //Se o anterior do destino é -1, ou seja, não tem anterior, e esse destino não é a origem, então não tem caminho para esse destino
            if(anteriores[t] == -1 && t != origem){
                this->temCaminho = false;
            }

            //Percorre pelo vetor de anteriores sempre buscando o vertice anterior ao t atual e adiciona no vetor caminho
            while(t != -1 && this->temCaminho){
                this->caminho.insert(this->caminho.begin(), t);
                t = anteriores[t];
            }
        }

        void printarCaminho(){
            cout << "P " << destino << " " << custo << " " << comprimento << " ";
            for(int i = 0; i < this->caminho.size(); i++){
                cout << this->caminho[i] << " ";
            }
            cout << endl;
        }
};

//Classe usada para armazenar o destino e o custo de um arco
class Edge{
    public:
        int custo;
        int destino;

        Edge(int custo, int destino) : custo(custo), destino(destino){}
        Edge(){}
};

//Classe usada para representar os vértices do grafo, contém o custo de entrada e de saída, além de um vetor de Edge, que contém todos arcos que saem do vértice
class Node{
    public:
        int custoEntrada;
        int custoSaida;
        vector<Edge> vizinhos;

        Node(int custoEntrada, int custoSaida) : custoEntrada(custoEntrada), custoSaida(custoSaida){}

        void adicionarVizinho(int destino, int custo){
            vizinhos.push_back(Edge(custo, destino));
        }
};

//Classe usada para representar o Grafo lido.
class Graph{

    public:
        int numVertices;
        int numArestas;
        vector<Node> listaAdjacencia; // Lista de adjacência, um vetor de Nodes, e cada Node contem um vetor de Vizinhos(Edge)
        vector<int> comprimento; //Vetor que armazena os comprimentos de cada caminho. Comprimento[i] = comprimento do caminho de S até I
        vector<int> distancia; // Vetor que armazena os custos dos caminhos. Distancia[i] = custo do caminho de S até I

        Graph(int numVertices, int numArestas) : numVertices(numVertices), numArestas(numArestas), comprimento(numVertices), distancia(numVertices, INT_MAX){}
        Graph(){}

        //Apensar insere o novo Node no final da lista de adjacência
        void adicionarVertice(int custoEntrada, int custoSaida){
            listaAdjacencia.push_back(Node(custoEntrada, custoSaida));
        }

        // Adiciona o Arco com destino "v" e custo "custo" no vetor de vizinhos do vertice "u"
        void adicionarVizinho(int u, int v, int custo){
            listaAdjacencia[u].adicionarVizinho(v, custo);
        }

        //Função utilizada para visualizar cada nó do grafo, seus graus os arcos que saem dele, contendo destino e custo de cada arco
        void printarGrafo(){
            for(int i = 0; i < listaAdjacencia.size(); i++){
                cout << " ( " << i << ", " << listaAdjacencia[i].custoEntrada << ", " << listaAdjacencia[i].custoSaida <<" ) -> ";
                for(int j = 0; j < listaAdjacencia[i].vizinhos.size(); j++){
                    cout << "( " << listaAdjacencia[i].vizinhos[j].destino << ", " << listaAdjacencia[i].vizinhos[j].custo <<" ); ";
                    
                }
                cout << endl;
            }
        }
};

// Algoritmo de Dijkstra implementado sem lista de prioridade
vector<int> Dijkstra(Graph* g, int s){
    set<int> visitados; // Armazenar os vertices que já foram visitados
    vector<int> anterior(g->numVertices, -1); // Vetor que guardar o vertice anterior de cada vertice. anterior[i] = numero do vertice anterior à i.

    g->distancia[s] = 0; // A distancia da origem do caminho para ela mesma é 0
    g->comprimento[s] = 0; // O comprimento do caminho que vai da origem até ela mesma é 0

    //Enquanto ainda não houver visitado todos os vértices
    while(visitados.size() < g->numVertices){

        int u = -1; // Vertice que será visitado
        int v = -1; // Variável auxiliar para escolher o vertice

        int menorDistancia = INT_MAX;
        // Define o u como o vertíce que tem a menor distância de s
        for(int i = 0; i < g->numVertices; i++){
            if(visitados.find(i) == visitados.end() && g->distancia[i] < menorDistancia){
                menorDistancia = g->distancia[i];
                u = i;
            }
        }

        // Se houver dois vértices com a mesma distância, e esta for a menorDistancia, então pega aquele que tem o menor comprimento entre os dois
        for(int i = 0; i < g->numVertices; i++){
            if(visitados.find(i) == visitados.end() && g->distancia[i] == menorDistancia && g->comprimento[i] < g->comprimento[u]){
                u = i;
            }else if(visitados.find(i) == visitados.end() && g->distancia[i] == menorDistancia && g->comprimento[i] == g->comprimento[u]){
                // Caso tenha mais de um, com a mesma distancia e mesmo comprimento, pega o que tem o menor número
                if(i < u){
                    u = i;
                }
            }
        }

        if(u == -1) break;

        //Marca u como visitado
        visitados.insert(u);

        // Percorre pelo vetor de Arcos que saem do vértice u, pegando seu destino e custo
        for(const Edge& edge : g->listaAdjacencia[u].vizinhos){
            int v = edge.destino;
            int custo = edge.custo;

            //Se o destino daquele arco ainda não foi visitado e a distancia da origem ao vertice u + custo, for menor do que a distancia da origem ao vertice v
            if(visitados.find(v) == visitados.end() && g->distancia[u] + custo < g->distancia[v]){
                g->distancia[v] = g->distancia[u] + custo; // Atualiza distancia até v para o menor valor
                g->comprimento[v] = g->comprimento[u] + 1;  // Define o numero de arcos do caminho da origem à v
                anterior[v] = u;    // Define o anterior de v como sendo u

            //Caso a distancia da origem ao vertice u + custo seja igual a distancia da origem ao vertice v
            }else if(visitados.find(v) == visitados.end() && g->distancia[u] + custo == g->distancia[v]){ 
                // Se o (comprimento da origem à u + 1) for menor que (comprimento da origem à v) OU (esses dois forem iguais E o vertice u é menor numericamente do que o vertice anterior à v)
                if(g->comprimento[u] + 1 < g->comprimento[v] || (g->comprimento[u] + 1 == g->comprimento[v] && u < anterior[v])){
                    g->comprimento[v] = g->comprimento[u] + 1;
                    anterior[v] = u;
                }
            }
        }

    }

    return anterior;
}

int main(int argv, char** argc){

    //Se não forem passados os argumentos necessário o programa é encerrado
    if(argv != 3){
        return 0;
    }

    string linha;
    // Abre o arquivo passado como argumento
    ifstream arquivoEntrada(argc[1]);
    // Pega o vertice de origem passado como argumento
    int s = stoi(argc[2]);

    int numVertices;
    int numArestas;

    Graph g;

    // Percorre linha por linha do arquivo de entrada
    while(getline(arquivoEntrada, linha)){
        istringstream linhaStream(linha);
        string palavra;
        
        // Percorre cada palavra em cada linha do arquivo de entrada
        while(linhaStream >> palavra){
            if(palavra == "I"){
                // Se a linha começa com I, pega o numero de vertices e arestas do grafo
                linhaStream >> numVertices >> numArestas;
                g = Graph(numVertices, numArestas);
            }else if(palavra == "N"){
                // Se a linha começa com N, pega as informações de cada vertice do grafo
                int vertice, custoEntrada, custoSaida;
                linhaStream >> vertice >> custoEntrada >> custoSaida;
                g.adicionarVertice(custoEntrada, custoSaida);
            }else if(palavra == "E"){
                // Se a linha começa com E, pega as informações de cada aresta do grafo
                int origem, destino, custo;
                linhaStream >> origem >> destino >> custo;
                g.adicionarVizinho(origem, destino, custo);
            }
        }

    }

    arquivoEntrada.close();

    // Executa do algoritmo de Dijkstra para gerar o vetor de anteriores, comprimento e distancia
    vector<int> anteriores = Dijkstra(&g, s);

    // Cria o arquivo de saida
    string fileName = argc[1];
    fileName = fileName.substr(fileName.find_last_of("/") + 1);
    fileName = fileName.substr(0, fileName.size() - 4);
    string path = "./out/" + fileName + "-out" + to_string(s) + ".txt";
    cout << path << endl;
    ofstream arquivoSaida(path);

    for(int i = 0; i < numVertices; i++){
        // Cria um caminho(Path) para cada vertice do grafo
        Path p(i, g.comprimento[i], g.distancia[i], s);
        // Constroi o caminho do grafo passando o vetor de anteriores
        p.constroiCaminho(anteriores);

        // Se o caminho existe
        if(p.temCaminho){
            // Escreve no arquivo de saida conforma o especificado
            // P destino custo comprimento v1 v2 v3 ... vn
            arquivoSaida << "P " << p.destino << " " << p.custo << " " << p.comprimento << " ";
            for(int i = 0; i < p.caminho.size(); i++){
                if(i == p.caminho.size() - 1){
                    arquivoSaida<< p.caminho[i];
                }else{
                    arquivoSaida << p.caminho[i] << " ";
                }
            }
            arquivoSaida << endl;
        }else{
            // Caso o não tenha caminho, escreve "U destino" no arquivo
            arquivoSaida << "U " << p.destino << endl;
        }

    }

    arquivoSaida.close();
}
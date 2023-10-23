#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <climits>
#include <set>

using namespace std;

class Path{
    public:
        int origem;
        int destino;
        int custo;
        int comprimento;
        bool temCaminho = true;
        vector<int> caminho;

        Path(int t, int comprimento, int custo, int origem): destino(t), comprimento(comprimento), custo(custo), origem(origem){}

        void constroiCaminho(vector<int> anteriores){
            int t = this->destino;
            if(anteriores[t] == -1 && t != origem){
                this->temCaminho = false;
            }
            while(t != -1){
                if(!this->temCaminho) break;
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

class Edge{
    public:
        int custo;
        int destino;

        Edge(int custo, int destino) : custo(custo), destino(destino){}
        Edge(){}
};

class Node{
    public:
        int custoEntrada;
        int custoSaida;
        vector<Edge> vizinhos;

        Node(int custoEntrada, int custoSaida) : custoEntrada(custoEntrada), custoSaida(custoSaida){}

        void adicionarVizinho(int destino, int custo){
            vizinhos.push_back(Edge(custo, destino));
        }

        int getCustoArco(int destino){
            for(int i = 0; i < vizinhos.size(); i++){
                if(vizinhos[i].destino == destino){
                    return vizinhos[i].custo;
                }
            }
            return -1;
        }
};

class Graph{

    public:
        int numVertices;
        int numArestas;
        vector<Node> listaAdjacencia;
        vector<int> comprimento;
        vector<int> distancia;

        Graph(int numVertices, int numArestas) : numVertices(numVertices), numArestas(numArestas), comprimento(numVertices), distancia(numVertices, INT_MAX){}
        Graph(){}

        void adicionarVertice(int custoEntrada, int custoSaida){
            listaAdjacencia.push_back(Node(custoEntrada, custoSaida));
        }

        void adicionarAresta(int u, int v, int custo){
            listaAdjacencia[u].adicionarVizinho(v, custo);
        }

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

vector<int> Dijkstra(Graph* g, int s){
    set<int> visitados;
    vector<int> anterior(g->numVertices, -1);

    g->distancia[s] = 0;
    g->comprimento[s] = 0;

    while(visitados.size() < g->numVertices){

        int u = -1;
        int v = -1;

        int menorDistancia = INT_MAX;

        for(int i = 0; i < g->numVertices; i++){
            if(visitados.find(i) == visitados.end() && g->distancia[i] < menorDistancia){
                menorDistancia = g->distancia[i];
                u = i;
            }
        }

        for(int i = 0; i < g->numVertices; i++){
            if(visitados.find(i) == visitados.end() && g->distancia[i] == menorDistancia && g->comprimento[i] < g->comprimento[u]){
                u = i;
            }
        }

        if(u == -1){
            break;
        }

        visitados.insert(u);

        for(const Edge& edge : g->listaAdjacencia[u].vizinhos){
            int v = edge.destino;
            int custo = edge.custo;
            if(visitados.find(v) == visitados.end() && g->distancia[u] + custo < g->distancia[v]){
                g->distancia[v] = g->distancia[u] + custo;
                g->comprimento[v] = g->comprimento[u] + 1;
                anterior[v] = u;
            }else if(visitados.find(v) == visitados.end() && g->distancia[u] + custo == g->distancia[v]){
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

    if(argv != 3){
        return 0;
    }

    string linha;
    ifstream arquivoEntrada(argc[1]);
    int s = stoi(argc[2]);

    int numVertices;
    int numArestas;

    Graph g;

    while(getline(arquivoEntrada, linha)){
        istringstream linhaStream(linha);
        string palavra;
        while(linhaStream >> palavra){
            if(palavra == "I"){
                linhaStream >> numVertices >> numArestas;
                g = Graph(numVertices, numArestas);
            }else if(palavra == "N"){
                int vertice, custoEntrada, custoSaida;
                linhaStream >> vertice >> custoEntrada >> custoSaida;
                g.adicionarVertice(custoEntrada, custoSaida);
            }else if(palavra == "E"){
                int origem, destino, custo;
                linhaStream >> origem >> destino >> custo;
                g.adicionarAresta(origem, destino, custo);
            }
        }

    }

    arquivoEntrada.close();

    vector<int> anteriores = Dijkstra(&g, s);

    ofstream arquivoSaida("saida.txt");

    for(int i = 0; i < numVertices; i++){
        Path p(i, g.comprimento[i], g.distancia[i], s);
        p.constroiCaminho(anteriores);

        if(p.temCaminho){
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
            arquivoSaida << "U " << p.destino << endl;
        }

    }
}
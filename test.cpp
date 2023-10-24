#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

int main(int argv, char** argc){
    if(argv != 3){
        return 0;
    }

    string path, pathGabarito;
    ifstream outPaths(argc[1]);
    ifstream gabaritoPaths(argc[2]);
    while(getline(outPaths, path) && getline(gabaritoPaths, pathGabarito)){
        string linha, linhaGabarito;
        ifstream arquivoGerado(path);
        ifstream gabarito(pathGabarito);

        int i = 0, j = 0;

        bool correto = true;

        while(getline(arquivoGerado, linha) && getline(gabarito, linhaGabarito)){
            istringstream linhaStream(linha);
            istringstream linhaGabaritoStream(linhaGabarito);
            string palavra, palavraGabarito;

            while(linhaStream >> palavra && linhaGabaritoStream >> palavraGabarito){
                if(palavra != palavraGabarito){
                    cout << "i: " << i << " j: " << j << endl;
                    cout << palavra << " != " << palavraGabarito << endl;
                    correto = false;
                }

                j += 1;
            }

            i += 1;
            j = 0;
        }

        if(correto){
            cout << "CORRETO" << endl;
        }
    }

}
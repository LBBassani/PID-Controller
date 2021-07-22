#include "command.h"
#include <iostream>

using namespace std;

int main(){

    cout << "Testando: Comandos" << endl;

    Command comando = Command(string( "{'comando':'ajustar_vel','parametros':{'vel_l':10,'vel_d':10}"));

    cout << "Comando: " << comando.comando << endl;
    
    for(int i=0; i < comando.args.size(); i++){
        string valor;
        valor = comando.args[i];
        valor = valor.erase(0, valor.find(":")+1);
        cout << "Parametro " << i << ": " << comando.args[i] << endl;
        cout << "Valor: " << stoi(valor) << endl;
    }

    comando = Command(string( "{'comando':'ajustar_params','parametros':{'ki':10.0,'kd':1.56, 'kp':2.4}"));

    cout << "Comando: " << comando.comando << endl;
    
    for(int i=0; i < comando.args.size(); i++){
        string valor;
        valor = comando.args[i];
        valor = valor.erase(0, valor.find(":")+1);
        cout << "Parametro " << i << ": " << comando.args[i] << endl;
        cout << "Valor: " << stod(valor) << endl;
    }

    return 0;
}
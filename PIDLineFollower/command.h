#ifndef COMMAND_H
#define COMMAND_H

#include <string>
#include <vector>

using namespace std;

class Command
{    

public:
    enum Comando{
        DESCONHECIDO = 0,
        AJUSTAR_VEL,
        AJUSTAR_PARAMS,
        RODAR,
        PARAR,
        RETORNAR_ERRO,
        RESETAR_ERRO
    };
    
    Comando comando;
    vector<string> args;
    Command(const string &command);
    ~Command();
};


#endif
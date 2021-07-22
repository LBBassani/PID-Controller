#include "command.h"

std::vector<std::string> split(const std::string& text, char sep)
{
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;

    while ((end = text.find(sep, start)) != std::string::npos)
    {
        tokens.push_back(text.substr(start, end - start));
        start = end + 1;
    }

    tokens.push_back(text.substr(start));
    return tokens;
}

void eraseSubStr(std::string & mainStr, const std::string & toErase)
{
    // Search for the substring in string
    size_t pos = mainStr.find(toErase);
    if (pos != std::string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());

    }
}

/*
 * Erase all Occurrences of given substring from main string.
 */
void eraseAllSubStr(std::string & mainStr, const std::string & toErase)
{
    size_t pos = std::string::npos;
    // Search for the substring in string in a loop untill nothing is found
    while ((pos  = mainStr.find(toErase) )!= std::string::npos)
    {
        // If found then erase it from string
        mainStr.erase(pos, toErase.length());
    }
}

Command::Command(const string &command)
{
    vector<string> separado = split(command, ',');

    vector<string> comando = split(separado.at(0), ':');

    if (comando.at(0).find("comando") != string::npos){

        if(comando.at(1).find("ajustar_vel") != string::npos) this->comando = Comando::AJUSTAR_VEL;
        else if (comando.at(1).find("ajustar_params") != string::npos) this->comando = Comando::AJUSTAR_PARAMS;
        else if (comando.at(1).find("rodar") != string::npos) this->comando = Comando::RODAR;
        else if (comando.at(1).find("parar") != string::npos) this->comando = Comando::PARAR;
        else if (comando.at(1).find("retornar_erro") != string::npos) this->comando = Comando::RETORNAR_ERRO;
        else if (comando.at(1).find("resetar_erro") != string::npos) this->comando = Comando::RESETAR_ERRO;
        else this ->comando = Comando::DESCONHECIDO;

    }else{
        this->comando = Comando::DESCONHECIDO;
    }

    if(separado.size() > 1 && this->comando != Comando::DESCONHECIDO){
        separado.erase(separado.begin());
        eraseSubStr(separado.at(0), string("'parametros':"));
        eraseSubStr(separado.at(0), string("{"));
        eraseSubStr(separado.at(separado.size() - 1), string("}"));
        this->args = separado;
    }
}

Command::~Command()
{
}
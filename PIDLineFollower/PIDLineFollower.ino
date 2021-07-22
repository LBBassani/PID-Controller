#include "command.h"

using namespace std;

double kp = 1, ki = 0, kd = 0;
int vel_l = 255, vel_d = 255;
bool running = false;
double erro = 0;

void resetar_erro(){
  erro = 0;
}

void set_running(bool r){
  running = r;
}

void change_vel(vector<string> velocidades){
  string valor_1, valor_2;
  valor_1 = velocidades[0];
  valor_1 = valor_1.erase(0, valor_1.find(":")+1);

  valor_2 = velocidades[1];
  valor_2 = valor_2.erase(0, valor_2.find(":")+1);

  if(velocidades[0].find('vel_l') != string::npos){
    vel_l = stoi(valor_1);
    vel_d = stoi(valor_2);
  } else{
    vel_l = stoi(valor_2);
    vel_d = stoi(valor_1);
  }
}

void change_k(vector<string> ks){
  string valor_1, valor_2, valor_3;
  valor_1 = ks[0];
  valor_1 = valor_1.erase(0, valor_1.find(":")+1);

  valor_2 = ks[1];
  valor_2 = valor_2.erase(0, valor_2.find(":")+1);

  valor_3 = ks[2];
  valor_3 = valor_3.erase(0, valor_3.find(":")+1);

  if(ks[0].find('ki') != string::npos){
    ki = stod(valor_1);
    if(ks[1].find('kd') != string::npos){
      kd = stod(valor_2);
      kp = stod(valor_3);
    }else{
      kd = stod(valor_3);
      kp = stod(valor_2);
    }
  }else if (ks[1].find('ki') != string::npos){
    ki = stod(valor_2);
    if(ks[0].find('kd') != string::npos){
      kd = stod(valor_1);
      kp = stod(valor_3);
    }else{
      kd = stod(valor_3);
      kp = stod(valor_1);
    }
  }else{
    ki = stod(valor_3);
    if(ks[0].find('kd') != string::npos){
      kd = stod(valor_1);
      kp = stod(valor_2);
    }else{
      kd = stod(valor_2);
      kp = stod(valor_1);
    }
  }
}

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

}

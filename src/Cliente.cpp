#include "Cliente.h"
using namespace std;

Cliente::Cliente(string nomeCompleto, string cpf, float dinheiro)
    : Pessoa(nomeCompleto, cpf), dinheiro(dinheiro) {}

float Cliente::quantosReais() const {
    return dinheiro;
}

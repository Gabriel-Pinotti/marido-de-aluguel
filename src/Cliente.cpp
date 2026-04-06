#include "Cliente.h"

Cliente::Cliente(std::string nomeCompleto, std::string cpf, float dinheiro)
    : Pessoa(nomeCompleto, cpf), dinheiro(dinheiro) {}

float Cliente::quantosReais() const {
    return dinheiro;
}

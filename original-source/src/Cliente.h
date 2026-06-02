#pragma once
#include "Pessoa.h"

class Cliente : public Pessoa {
private:
    float dinheiro;

public:
    Cliente(std::string nomeCompleto, std::string cpf, float dinheiro);
    float quantosReais() const;
};

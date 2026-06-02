#pragma once
#include <string>

class Pessoa {
public:
    std::string nomeCompleto;
    std::string cpf; // string: CPF tem 11 dígitos, estoura int32

    Pessoa(std::string nomeCompleto, std::string cpf);
    virtual ~Pessoa() = default;
};

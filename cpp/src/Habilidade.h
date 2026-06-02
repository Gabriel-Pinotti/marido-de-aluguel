#pragma once
#include <string>

class Habilidade {
private:
    std::string nome;
    float valorOperacao;

public:
    Habilidade(std::string nome, float valorOperacao);
    std::string getNome() const;
    float getValor() const;
};

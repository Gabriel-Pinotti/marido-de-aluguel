#pragma once
#include <string>

// Classe ABSTRATA: tem um método virtual puro (papel()), então não pode ser
// instanciada diretamente — só através das subclasses Cliente e Trabalhador.
class Pessoa {
private:
    std::string nomeCompleto; // encapsulado (antes era público)
    std::string cpf;

public:
    Pessoa(std::string nomeCompleto, std::string cpf);
    virtual ~Pessoa() = default;

    // encapsulamento: acesso só por getters
    std::string getNome() const;
    std::string getCpf() const;

    // método virtual PURO -> torna a classe abstrata; sobrescrito nas filhas
    virtual std::string papel() const = 0;
};

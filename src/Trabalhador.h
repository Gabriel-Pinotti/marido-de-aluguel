#pragma once
#include "Pessoa.h"
#include "Habilidade.h"
#include "Trabalho.h"
#include <vector>

class Trabalhador : public Pessoa {
public:
    std::vector<Habilidade> habilidades;

private:
    std::vector<Trabalho> trabalhos;

public:
    Trabalhador(std::string nomeCompleto, std::string cpf, std::vector<Habilidade> habilidades);

    Trabalho contratar(Cliente& c, Data d, Habilidade h);
    bool estaLivre(Data d) const;
    bool temHabilidade(const Habilidade& h) const;
};

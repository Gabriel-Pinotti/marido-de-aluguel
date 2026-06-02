#pragma once
#include "Pessoa.h"
#include "ISerializavel.h"
#include "Habilidade.h"
#include "Trabalho.h"
#include <vector>

// Herda de Pessoa (abstrata) e implementa a interface ISerializavel.
class Trabalhador : public Pessoa, public ISerializavel {
private:
    std::vector<Habilidade> habilidades; // encapsulado (antes era público)
    std::vector<Trabalho> trabalhos;

    static int totalCadastrados; // ATRIBUTO STATIC: quantos trabalhadores existem

public:
    Trabalhador(std::string nomeCompleto, std::string cpf, std::vector<Habilidade> habilidades);

    Trabalho contratar(Cliente& c, Data d, Habilidade h);
    void adicionarTrabalho(const Trabalho& t); // usado pelo CSVManager ao carregar
    bool estaLivre(Data d) const;

    // SOBRECARGA: mesmo nome, assinaturas diferentes
    bool temHabilidade(const Habilidade& h) const;
    bool temHabilidade(const std::string& nomeHab) const;

    const std::vector<Habilidade>& getHabilidades() const;
    const std::vector<Trabalho>& getTrabalhos() const;
    void definirHabilidades(std::vector<Habilidade> novas); // usado na edição

    static int getTotalCadastrados(); // MÉTODO STATIC

    std::string papel() const override;  // SOBRESCRITA (de Pessoa)
    std::string paraCSV() const override; // SOBRESCRITA (de ISerializavel)
};

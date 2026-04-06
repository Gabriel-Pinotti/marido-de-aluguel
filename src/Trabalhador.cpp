#include "Trabalhador.h"

Trabalhador::Trabalhador(std::string nomeCompleto, std::string cpf, std::vector<Habilidade> habilidades)
    : Pessoa(nomeCompleto, cpf), habilidades(habilidades) {}

Trabalho Trabalhador::contratar(Cliente& c, Data d, Habilidade h) {
    Trabalho t(d, c, h);
    trabalhos.push_back(t);
    return t;
}

void Trabalhador::adicionarTrabalho(const Trabalho& t) {
    trabalhos.push_back(t);
}

const std::vector<Trabalho>& Trabalhador::getTrabalhos() const {
    return trabalhos;
}

bool Trabalhador::estaLivre(Data d) const {
    for (const auto& t : trabalhos) {
        if (t.getDataInicio() == d) return false;
    }
    return true;
}

bool Trabalhador::temHabilidade(const Habilidade& h) const {
    for (const auto& hab : habilidades) {
        if (hab.nome == h.nome) return true;
    }
    return false;
}

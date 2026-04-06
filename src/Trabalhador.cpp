#include "Trabalhador.h"
using namespace std;

Trabalhador::Trabalhador(string nomeCompleto, string cpf, vector<Habilidade> habilidades)
    : Pessoa(nomeCompleto, cpf), habilidades(habilidades) {}

Trabalho Trabalhador::contratar(Cliente& c, Data d, Habilidade h) {
    Trabalho t(d, c, h);
    trabalhos.push_back(t);
    return t;
}

void Trabalhador::adicionarTrabalho(const Trabalho& t) {
    trabalhos.push_back(t);
}

const vector<Trabalho>& Trabalhador::getTrabalhos() const {
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

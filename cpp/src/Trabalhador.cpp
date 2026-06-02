#include "Trabalhador.h"
#include <sstream>
#include <iomanip>
using namespace std;

int Trabalhador::totalCadastrados = 0; // definição do atributo static

Trabalhador::Trabalhador(string nomeCompleto, string cpf, vector<Habilidade> habilidades)
    : Pessoa(nomeCompleto, cpf), habilidades(habilidades) {
    totalCadastrados++;
}

Trabalho Trabalhador::contratar(Cliente& c, Data d, Habilidade h) {
    Trabalho t(d, c, h);
    trabalhos.push_back(t);
    return t;
}

void Trabalhador::adicionarTrabalho(const Trabalho& t) {
    trabalhos.push_back(t);
}

const vector<Habilidade>& Trabalhador::getHabilidades() const {
    return habilidades;
}

void Trabalhador::definirHabilidades(vector<Habilidade> novas) {
    habilidades = novas;
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

// sobrecarga: delega para a versão que recebe o nome
bool Trabalhador::temHabilidade(const Habilidade& h) const {
    return temHabilidade(h.getNome());
}

bool Trabalhador::temHabilidade(const string& nomeHab) const {
    for (const auto& hab : habilidades) {
        if (hab.getNome() == nomeHab) return true;
    }
    return false;
}

int Trabalhador::getTotalCadastrados() { return totalCadastrados; }

string Trabalhador::papel() const { return "Trabalhador"; }

// Serialização para a linha de trabalhadores.csv: nome,cpf,Hab:val:Hab:val
string Trabalhador::paraCSV() const {
    ostringstream os;
    os << getNome() << "," << getCpf() << ",";
    for (size_t i = 0; i < habilidades.size(); i++) {
        os << habilidades[i].getNome() << ":" << fixed << setprecision(2) << habilidades[i].getValor();
        if (i + 1 < habilidades.size()) os << ":";
    }
    return os.str();
}

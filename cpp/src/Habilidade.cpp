#include "Habilidade.h"
using namespace std;

Habilidade::Habilidade(string nome, float valorOperacao)
    : nome(nome), valorOperacao(valorOperacao) {}

string Habilidade::getNome() const { return nome; }
float Habilidade::getValor() const { return valorOperacao; }

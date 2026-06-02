#include "Pessoa.h"
using namespace std;

Pessoa::Pessoa(string nomeCompleto, string cpf)
    : nomeCompleto(nomeCompleto), cpf(cpf) {}

string Pessoa::getNome() const { return nomeCompleto; }
string Pessoa::getCpf() const { return cpf; }

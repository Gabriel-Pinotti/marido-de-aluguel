#include "Trabalho.h"

Trabalho::Trabalho(Data d, Cliente c, Habilidade h)
    : dataInicio(d), cliente(c), habilidade(h) {}

Data Trabalho::getDataInicio() const { return dataInicio; }
Cliente Trabalho::getCliente() const { return cliente; }
Habilidade Trabalho::getHabilidade() const { return habilidade; }

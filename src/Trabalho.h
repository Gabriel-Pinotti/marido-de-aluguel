#pragma once
#include "Data.h"
#include "Cliente.h"
#include "Habilidade.h"

class Trabalho {
private:
    Data dataInicio;
    Cliente cliente; // cópia — referência quebra semântica de vetor
    Habilidade habilidade;

public:
    Trabalho(Data d, Cliente c, Habilidade h);
    Data getDataInicio() const;
    Cliente getCliente() const;
    Habilidade getHabilidade() const;
};

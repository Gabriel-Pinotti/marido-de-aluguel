#include "Data.h"

Data::Data(int dia, int mes, int ano) : dia(dia), mes(mes), ano(ano) {}

std::string Data::toString() const {
    auto pad = [](int n) { return (n < 10 ? "0" : "") + std::to_string(n); };
    return pad(dia) + "/" + pad(mes) + "/" + std::to_string(ano);
}

bool Data::operator==(const Data& other) const {
    return dia == other.dia && mes == other.mes && ano == other.ano;
}

#pragma once
#include <string>

class Data {
private:
    int dia, mes, ano;

public:
    Data(int dia, int mes, int ano);
    int getDia() const;
    int getMes() const;
    int getAno() const;
    std::string toString() const;
    bool operator==(const Data& other) const; // SOBRECARGA de operador
};

#pragma once
#include <string>

class Data {
public:
    int dia, mes, ano;

    Data(int dia, int mes, int ano);
    std::string toString() const;
    bool operator==(const Data& other) const;
};

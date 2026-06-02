#pragma once
#include <string>

// Classe TOTALMENTE ABSTRATA (interface): apenas métodos virtuais puros.
// Quem implementar precisa saber se converter para uma linha de CSV.
class ISerializavel {
public:
    virtual ~ISerializavel() = default;
    virtual std::string paraCSV() const = 0;
};

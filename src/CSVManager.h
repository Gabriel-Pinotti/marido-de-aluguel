#pragma once
#include "Trabalhador.h"
#include <string>
#include <vector>

class CSVManager {
public:
    static std::vector<Trabalhador> carregarTrabalhadores(const std::string& caminho);
    static void carregarTrabalhos(const std::string& caminho, std::vector<Trabalhador>& trabalhadores);
    static void salvarTrabalhadores(const std::string& caminho, const std::vector<Trabalhador>& trabalhadores);
    static void salvarTrabalho(const std::string& caminho, const std::string& cpfTrabalhador, const Trabalho& trabalho);
};

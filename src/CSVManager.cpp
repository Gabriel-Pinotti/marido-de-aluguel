#include "CSVManager.h"
#include <fstream>
#include <sstream>
#include <iostream>

// TODO Gabriel - adicionar isso tudo ao draw.io (ps: misericórdia)

static std::vector<std::string> split(const std::string& linha, char delim) {
    std::vector<std::string> partes;
    std::stringstream ss(linha);
    std::string parte;
    while (std::getline(ss, parte, delim)) {
        partes.push_back(parte);
    }
    return partes;
}

std::vector<Trabalhador> CSVManager::carregarTrabalhadores(const std::string& caminho) {
    std::vector<Trabalhador> trabalhadores;
    std::ifstream arquivo(caminho);

    if (!arquivo.is_open()) {
        std::cerr << "Erro: nao foi possivel abrir " << caminho << "\n";
        return trabalhadores;
    }

    std::string linha;
    std::getline(arquivo, linha);

    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;

        auto colunas = split(linha, ',');
        if (colunas.size() < 3) continue;

        std::string nome = colunas[0];
        std::string cpf  = colunas[1];

        std::vector<Habilidade> habilidades;
        auto tokens = split(colunas[2], ':');
        for (size_t i = 0; i + 1 < tokens.size(); i += 2) {
            std::string nomeHab = tokens[i];
            float valor = std::stof(tokens[i + 1]);
            habilidades.push_back(Habilidade(nomeHab, valor));
        }

        trabalhadores.push_back(Trabalhador(nome, cpf, habilidades));
    }

    return trabalhadores;
}

void CSVManager::carregarTrabalhos(const std::string& caminho, std::vector<Trabalhador>& trabalhadores) {
    std::ifstream arquivo(caminho);

    if (!arquivo.is_open()) {
        std::cerr << "Erro: nao foi possivel abrir " << caminho << "\n";
        return;
    }

    std::string linha;
    std::getline(arquivo, linha);

    while (std::getline(arquivo, linha)) {
        if (linha.empty()) continue;

        auto col = split(linha, ',');
        if (col.size() < 8) continue;

        int dia  = std::stoi(col[0]);
        int mes  = std::stoi(col[1]);
        int ano  = std::stoi(col[2]);
        std::string cpfTrabalhador = col[3];
        std::string nomeCliente    = col[4];
        std::string cpfCliente     = col[5];
        std::string nomeHab        = col[6];
        float valorHab             = std::stof(col[7]);

        Data data(dia, mes, ano);
        Cliente cliente(nomeCliente, cpfCliente, 0.0f);
        Habilidade hab(nomeHab, valorHab);
        Trabalho trabalho(data, cliente, hab);

        for (auto& t : trabalhadores) {
            if (t.cpf == cpfTrabalhador) {
                t.adicionarTrabalho(trabalho);
                break;
            }
        }
    }
}

void CSVManager::salvarTrabalho(const std::string& caminho, const std::string& cpfTrabalhador, const Trabalho& trabalho) {
    std::ofstream arquivo(caminho, std::ios::app);

    if (!arquivo.is_open()) {
        std::cerr << "Erro: nao foi possivel abrir " << caminho << "\n";
        return;
    }

    Data d = trabalho.getDataInicio();
    Cliente c = trabalho.getCliente();
    Habilidade h = trabalho.getHabilidade();

    arquivo << d.dia << "," << d.mes << "," << d.ano << ","
            << cpfTrabalhador << ","
            << c.nomeCompleto << ","
            << c.cpf << ","
            << h.nome << ","
            << h.valorOperacao << "\n";
}

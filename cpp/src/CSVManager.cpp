#include "CSVManager.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
using namespace std;

static vector<string> split(const string& linha, char delim) {
    vector<string> partes;
    stringstream ss(linha);
    string parte;
    while (getline(ss, parte, delim)) {
        partes.push_back(parte);
    }
    return partes;
}

vector<Trabalhador> CSVManager::carregarTrabalhadores(const string& caminho) {
    vector<Trabalhador> trabalhadores;
    ifstream arquivo(caminho);

    if (!arquivo.is_open()) {
        cerr << "Erro: nao foi possivel abrir " << caminho << "\n";
        return trabalhadores;
    }

    string linha;
    getline(arquivo, linha);

    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        auto colunas = split(linha, ',');
        if (colunas.size() < 3) continue;

        string nome = colunas[0];
        string cpf  = colunas[1];

        vector<Habilidade> habilidades;
        auto tokens = split(colunas[2], ':');
        for (size_t i = 0; i + 1 < tokens.size(); i += 2) {
            string nomeHab = tokens[i];
            float valor = stof(tokens[i + 1]);
            habilidades.push_back(Habilidade(nomeHab, valor));
        }

        trabalhadores.push_back(Trabalhador(nome, cpf, habilidades));
    }

    return trabalhadores;
}

void CSVManager::carregarTrabalhos(const string& caminho, vector<Trabalhador>& trabalhadores) {
    ifstream arquivo(caminho);

    if (!arquivo.is_open()) {
        cerr << "Erro: nao foi possivel abrir " << caminho << "\n";
        return;
    }

    string linha;
    getline(arquivo, linha);

    while (getline(arquivo, linha)) {
        if (linha.empty()) continue;

        auto col = split(linha, ',');
        if (col.size() < 8) continue;

        int dia, mes, ano;
        float valorHab;
        try {
            dia      = stoi(col[0]);
            mes      = stoi(col[1]);
            ano      = stoi(col[2]);
            valorHab = stof(col[7]);
        } catch (...) {
            cerr << "Aviso: linha ignorada por dados inválidos: " << linha << "\n";
            continue;
        }

        string cpfTrabalhador = col[3];
        string nomeCliente    = col[4];
        string cpfCliente     = col[5];
        string nomeHab        = col[6];

        Data data(dia, mes, ano);
        Cliente cliente(nomeCliente, cpfCliente, 0.0f);
        Habilidade hab(nomeHab, valorHab);
        Trabalho trabalho(data, cliente, hab);

        for (auto& t : trabalhadores) {
            if (t.getCpf() == cpfTrabalhador) {
                t.adicionarTrabalho(trabalho);
                break;
            }
        }
    }
}

void CSVManager::salvarTrabalhadores(const string& caminho, const vector<Trabalhador>& trabalhadores) {
    ofstream arquivo(caminho);

    if (!arquivo.is_open()) {
        cerr << "Erro: nao foi possivel abrir " << caminho << "\n";
        return;
    }

    arquivo << "nome,cpf,habilidades\n";
    for (const auto& t : trabalhadores) {
        // polimorfismo via interface: cada Trabalhador sabe se serializar
        arquivo << t.paraCSV() << "\n";
    }
}

void CSVManager::salvarTrabalho(const string& caminho, const string& cpfTrabalhador, const Trabalho& trabalho) {
    ofstream arquivo(caminho, ios::app);

    if (!arquivo.is_open()) {
        cerr << "Erro: nao foi possivel abrir " << caminho << "\n";
        return;
    }

    Data d = trabalho.getDataInicio();
    Cliente c = trabalho.getCliente();
    Habilidade h = trabalho.getHabilidade();

    arquivo << d.getDia() << "," << d.getMes() << "," << d.getAno() << ","
            << cpfTrabalhador << ","
            << c.getNome() << ","
            << c.getCpf() << ","
            << h.getNome() << ","
            << h.getValor() << "\n";
}

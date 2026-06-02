#include "Cli.h"
#include "CSVManager.h"
#include "Trabalhador.h"
#include "Cliente.h"
#include "Data.h"
#include "Habilidade.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

// Caminhos relativos ao diretório de onde o binário é executado (a raiz do
// projeto). O plugin do Vite vai rodar com cwd na raiz.
static const string CAMINHO_TRABALHADORES = "data/trabalhadores.csv";
static const string CAMINHO_TRABALHOS = "data/trabalhos.csv";
static const string CAMINHO_CLIENTES = "data/clientes.csv";

// ---------- helpers de JSON ----------

static string jsonEscape(const string& s) {
    string out;
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;
        }
    }
    return out;
}

static string jsonStr(const string& s) { return "\"" + jsonEscape(s) + "\""; }

// valores monetários sempre com 2 casas -> JSON válido e sem perda (150.00 -> 150)
static string jsonValor(float v) {
    ostringstream os;
    os << fixed << setprecision(2) << v;
    return os.str();
}

// ---------- serialização de cada entidade para JSON ----------

static string trabalhadorJson(const Trabalhador& t) {
    ostringstream os;
    os << "{\"nome\":" << jsonStr(t.getNome())
       << ",\"cpf\":" << jsonStr(t.getCpf())
       << ",\"habilidades\":[";
    const auto& habs = t.getHabilidades();
    for (size_t i = 0; i < habs.size(); i++) {
        os << "{\"nome\":" << jsonStr(habs[i].getNome())
           << ",\"valor\":" << jsonValor(habs[i].getValor()) << "}";
        if (i + 1 < habs.size()) os << ",";
    }
    os << "]}";
    return os.str();
}

static string clienteJson(const Cliente& c) {
    ostringstream os;
    os << "{\"nome\":" << jsonStr(c.getNome())
       << ",\"cpf\":" << jsonStr(c.getCpf()) << "}";
    return os.str();
}

// ---------- comandos ----------

static int listarTrabalhadores() {
    auto trabalhadores = CSVManager::carregarTrabalhadores(CAMINHO_TRABALHADORES);
    ostringstream os;
    os << "[";
    for (size_t i = 0; i < trabalhadores.size(); i++) {
        os << trabalhadorJson(trabalhadores[i]);
        if (i + 1 < trabalhadores.size()) os << ",";
    }
    os << "]";
    cout << os.str() << "\n";
    return 0;
}

static int listarTrabalhos() {
    auto trabalhadores = CSVManager::carregarTrabalhadores(CAMINHO_TRABALHADORES);
    CSVManager::carregarTrabalhos(CAMINHO_TRABALHOS, trabalhadores);

    ostringstream os;
    os << "[";
    bool primeiro = true;
    for (const auto& t : trabalhadores) {
        for (const auto& w : t.getTrabalhos()) {
            if (!primeiro) os << ",";
            primeiro = false;
            Data d = w.getDataInicio();
            Cliente c = w.getCliente();
            Habilidade h = w.getHabilidade();
            os << "{\"dia\":" << d.getDia()
               << ",\"mes\":" << d.getMes()
               << ",\"ano\":" << d.getAno()
               << ",\"cpfTrabalhador\":" << jsonStr(t.getCpf())
               << ",\"nomeCliente\":" << jsonStr(c.getNome())
               << ",\"cpfCliente\":" << jsonStr(c.getCpf())
               << ",\"habilidade\":" << jsonStr(h.getNome())
               << ",\"valor\":" << jsonValor(h.getValor()) << "}";
        }
    }
    os << "]";
    cout << os.str() << "\n";
    return 0;
}

static int listarClientes() {
    auto clientes = CSVManager::carregarClientes(CAMINHO_CLIENTES);
    ostringstream os;
    os << "[";
    for (size_t i = 0; i < clientes.size(); i++) {
        os << clienteJson(clientes[i]);
        if (i + 1 < clientes.size()) os << ",";
    }
    os << "]";
    cout << os.str() << "\n";
    return 0;
}

// ---------- dispatcher ----------

int executarCli(int argc, char** argv) {
    string cmd = argv[1];

    if (cmd == "list" && argc >= 3) {
        string alvo = argv[2];
        if (alvo == "trabalhadores") return listarTrabalhadores();
        if (alvo == "trabalhos")     return listarTrabalhos();
        if (alvo == "clientes")      return listarClientes();
    }

    cerr << "{\"erro\":\"comando desconhecido\"}\n";
    return 2;
}

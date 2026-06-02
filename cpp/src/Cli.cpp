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

// ---------- helpers de texto / validação (espelham as regras do sistema) ----------

static vector<string> split(const string& s, char delim) {
    vector<string> partes;
    stringstream ss(s);
    string p;
    while (getline(ss, p, delim)) partes.push_back(p);
    return partes;
}

static string trim(const string& s) {
    size_t ini = s.find_first_not_of(" \t\r\n");
    if (ini == string::npos) return "";
    size_t fim = s.find_last_not_of(" \t\r\n");
    return s.substr(ini, fim - ini + 1);
}

static bool cpfValido(const string& cpf) {
    return cpf.size() == 11 && cpf.find_first_not_of("0123456789") == string::npos;
}

static bool dataValida(int dia, int mes, int ano) {
    return dia >= 1 && dia <= 31 && mes >= 1 && mes <= 12 && ano >= 2026;
}

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

// imprime erro como JSON em stdout e devolve código 1 (erro de validação)
static int erro(const string& msg) {
    cout << "{\"erro\":" << jsonStr(msg) << "}\n";
    return 1;
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

static string trabalhoJson(const string& cpfTrab, const Trabalho& w) {
    Data d = w.getDataInicio();
    Cliente c = w.getCliente();
    Habilidade h = w.getHabilidade();
    ostringstream os;
    os << "{\"dia\":" << d.getDia()
       << ",\"mes\":" << d.getMes()
       << ",\"ano\":" << d.getAno()
       << ",\"cpfTrabalhador\":" << jsonStr(cpfTrab)
       << ",\"nomeCliente\":" << jsonStr(c.getNome())
       << ",\"cpfCliente\":" << jsonStr(c.getCpf())
       << ",\"habilidade\":" << jsonStr(h.getNome())
       << ",\"valor\":" << jsonValor(h.getValor()) << "}";
    return os.str();
}

// ---------- comandos de LEITURA ----------

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
            os << trabalhoJson(t.getCpf(), w);
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

// ---------- comandos de ESCRITA ----------

// add-trabalhador <nome> <cpf> <habilidadesStr="Nome:valor:Nome:valor">
static int addTrabalhador(const string& nomeRaw, const string& cpfRaw, const string& habsStr) {
    string nome = trim(nomeRaw);
    string cpf = trim(cpfRaw);
    if (nome.empty()) return erro("Nome não pode ser vazio.");
    if (!cpfValido(cpf)) return erro("CPF inválido. Digite exatamente 11 números.");

    auto trabalhadores = CSVManager::carregarTrabalhadores(CAMINHO_TRABALHADORES);
    for (const auto& t : trabalhadores)
        if (t.getCpf() == cpf) return erro("CPF já cadastrado.");
    for (const auto& c : CSVManager::carregarClientes(CAMINHO_CLIENTES))
        if (c.getCpf() == cpf) return erro("CPF já cadastrado."); // conflito de papel

    vector<Habilidade> habs;
    auto tokens = split(habsStr, ':');
    for (size_t i = 0; i + 1 < tokens.size(); i += 2) {
        string nh = trim(tokens[i]);
        if (nh.empty()) return erro("Nome da habilidade não pode ser vazio.");
        float val;
        try { val = stof(tokens[i + 1]); }
        catch (...) { return erro("Valor inválido. Digite um número positivo."); }
        if (!(val > 0)) return erro("Valor inválido. Digite um número positivo.");
        habs.push_back(Habilidade(nh, val));
    }
    if (habs.empty()) return erro("Adicione pelo menos uma habilidade.");

    Trabalhador novo(nome, cpf, habs);
    trabalhadores.push_back(novo);
    CSVManager::salvarTrabalhadores(CAMINHO_TRABALHADORES, trabalhadores);

    cout << "{\"ok\":true,\"trabalhador\":" << trabalhadorJson(novo) << "}\n";
    return 0;
}

// add-cliente <nome> <cpf>
static int addCliente(const string& nomeRaw, const string& cpfRaw) {
    string nome = trim(nomeRaw);
    string cpf = trim(cpfRaw);
    if (nome.empty()) return erro("Nome não pode ser vazio.");
    if (!cpfValido(cpf)) return erro("CPF inválido. Digite exatamente 11 números.");

    auto clientes = CSVManager::carregarClientes(CAMINHO_CLIENTES);
    for (const auto& c : clientes)
        if (c.getCpf() == cpf) return erro("CPF já cadastrado.");
    for (const auto& t : CSVManager::carregarTrabalhadores(CAMINHO_TRABALHADORES))
        if (t.getCpf() == cpf) return erro("CPF já cadastrado."); // conflito de papel

    Cliente novo(nome, cpf, 0.0f);
    clientes.push_back(novo);
    CSVManager::salvarClientes(CAMINHO_CLIENTES, clientes);

    cout << "{\"ok\":true,\"cliente\":" << clienteJson(novo) << "}\n";
    return 0;
}

// contratar <cpfTrab> <habilidade> <dia> <mes> <ano> <nomeCliente> <cpfCliente>
static int contratar(const string& cpfTrab, const string& habNome,
                     int dia, int mes, int ano,
                     const string& nomeClienteRaw, const string& cpfClienteRaw) {
    if (!dataValida(dia, mes, ano))
        return erro("Data fora do intervalo válido (dia 1-31, mês 1-12, ano >= 2026).");
    string nomeCliente = trim(nomeClienteRaw);
    string cpfCliente = trim(cpfClienteRaw);
    if (nomeCliente.empty()) return erro("Nome não pode ser vazio.");
    if (!cpfValido(cpfCliente)) return erro("CPF inválido. Digite exatamente 11 números.");

    auto trabalhadores = CSVManager::carregarTrabalhadores(CAMINHO_TRABALHADORES);
    CSVManager::carregarTrabalhos(CAMINHO_TRABALHOS, trabalhadores);

    Trabalhador* alvo = nullptr;
    for (auto& t : trabalhadores)
        if (t.getCpf() == cpfTrab) { alvo = &t; break; }
    if (!alvo) return erro("Trabalhador não encontrado.");

    if (!alvo->temHabilidade(habNome)) return erro("Trabalhador não possui essa habilidade.");

    float valor = 0.0f;
    for (const auto& h : alvo->getHabilidades())
        if (h.getNome() == habNome) { valor = h.getValor(); break; }

    Data data(dia, mes, ano);
    if (!alvo->estaLivre(data)) return erro("Trabalhador não está livre nessa data.");

    Cliente cliente(nomeCliente, cpfCliente, 0.0f);
    Habilidade hab(habNome, valor);
    Trabalho t = alvo->contratar(cliente, data, hab);
    CSVManager::salvarTrabalho(CAMINHO_TRABALHOS, cpfTrab, t);

    cout << "{\"ok\":true,\"trabalho\":" << trabalhoJson(cpfTrab, t) << "}\n";
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

    if (cmd == "add-trabalhador" && argc >= 5)
        return addTrabalhador(argv[2], argv[3], argv[4]);

    if (cmd == "add-cliente" && argc >= 4)
        return addCliente(argv[2], argv[3]);

    if (cmd == "contratar" && argc >= 9) {
        int dia, mes, ano;
        try { dia = stoi(argv[4]); mes = stoi(argv[5]); ano = stoi(argv[6]); }
        catch (...) { return erro("Data inválida."); }
        return contratar(argv[2], argv[3], dia, mes, ano, argv[7], argv[8]);
    }

    cerr << "{\"erro\":\"comando desconhecido ou argumentos insuficientes\"}\n";
    return 2;
}

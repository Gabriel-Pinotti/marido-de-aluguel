#include "Menu.h"
#include "CSVManager.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>
#include <algorithm>
using namespace std;

static void limparTela() {
    system("clear");
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

static void printAgendaTrabalhador(const Trabalhador& t) {
    limparTela();
    cout << "----- " << t.nomeCompleto << " ";
    cout << string(max(0, 30 - (int)t.nomeCompleto.size()), '-') << "\n";

    cout << "Habilidades: ";
    for (size_t i = 0; i < t.habilidades.size(); i++) {
        cout << t.habilidades[i].nome << " (R$ " << fixed << setprecision(2) << t.habilidades[i].valorOperacao << ")";
        if (i + 1 < t.habilidades.size()) cout << " | ";
    }
    cout << "\n\nDatas ocupadas:\n";

    const auto& trabalhos = t.getTrabalhos();
    if (trabalhos.empty()) {
        cout << "  sem compromissos agendados\n";
    } else {
        for (const auto& trab : trabalhos) {
            Data d = trab.getDataInicio();
            cout << "  " << d.toString()
                 << "  " << left << setw(12) << trab.getHabilidade().nome
                 << "  cliente: " << trab.getCliente().nomeCompleto << "\n";
        }
    }

    cout << "\nPressione Enter para voltar...";
    cin.ignore(1000, '\n');
    cin.get();
}

void menuCadastrarTrabalhador(vector<Trabalhador>& trabalhadores) {
    string erro = "";

    while (true) {
        limparTela();
        cout << "===== Cadastrar Trabalhador =====\n";
        if (!erro.empty()) cout << "\n  ! " << erro << "\n";
        erro = "";

        // --- nome ---
        string nome;
        cout << "\nNome completo: ";
        getline(cin, nome);
        nome = trim(nome);
        if (nome.empty()) {
            erro = "Nome não pode ser vazio.";
            continue;
        }

        // --- CPF ---
        string cpf;
        cout << "CPF (11 dígitos, apenas números): ";
        cin >> cpf;
        cin.ignore(1000, '\n');

        if (!cpfValido(cpf)) {
            erro = "CPF inválido. Digite exatamente 11 números.";
            continue;
        }
        bool cpfDuplicado = false;
        for (const auto& t : trabalhadores)
            if (t.cpf == cpf) { cpfDuplicado = true; break; }
        if (cpfDuplicado) {
            erro = "CPF já cadastrado.";
            continue;
        }

        // --- habilidades ---
        vector<Habilidade> habilidades;
        while (true) {
            limparTela();
            cout << "===== Cadastrar Trabalhador =====\n";
            cout << "\nNome: " << nome << "  |  CPF: " << cpf << "\n";

            if (!habilidades.empty()) {
                cout << "\nHabilidades adicionadas:\n";
                for (const auto& h : habilidades)
                    cout << "  - " << h.nome << " (R$ " << fixed << setprecision(2) << h.valorOperacao << ")\n";
            }

            if (!erro.empty()) cout << "\n  ! " << erro << "\n";
            erro = "";

            cout << "\nNome da habilidade (0 para finalizar): ";
            string nomeHab;
            getline(cin, nomeHab);
            nomeHab = trim(nomeHab);

            if (nomeHab == "0") {
                if (habilidades.empty()) {
                    erro = "Adicione pelo menos uma habilidade.";
                    continue;
                }
                break;
            }
            if (nomeHab.empty()) {
                erro = "Nome da habilidade não pode ser vazio.";
                continue;
            }

            cout << "Valor por serviço (R$): ";
            float valor;
            if (!(cin >> valor) || valor <= 0) {
                cin.clear();
                cin.ignore(1000, '\n');
                erro = "Valor inválido. Digite um número positivo.";
                continue;
            }
            cin.ignore(1000, '\n');

            habilidades.push_back(Habilidade(nomeHab, valor));
        }

        // --- salvar ---
        Trabalhador novo(nome, cpf, habilidades);
        trabalhadores.push_back(novo);
        CSVManager::salvarTrabalhadores("data/trabalhadores.csv", trabalhadores);

        limparTela();
        cout << "===== Cadastrar Trabalhador =====\n\nTrabalhador cadastrado!\n";
        cout << "  Nome: " << nome << "\n";
        cout << "  CPF:  " << cpf << "\n";
        cout << "  Habilidades: ";
        for (size_t i = 0; i < habilidades.size(); i++) {
            cout << habilidades[i].nome << " (R$ " << fixed << setprecision(2) << habilidades[i].valorOperacao << ")";
            if (i + 1 < habilidades.size()) cout << ", ";
        }
        cout << "\n\nPressione Enter para voltar ao menu inicial...";
        cin.get();
        break;
    }
}

void menuContratacao(vector<Trabalhador>& trabalhadores) {
    if (trabalhadores.empty()) {
        limparTela();
        cout << "===== Contratar Serviços =====\n\n";
        cout << "  Nenhum trabalhador cadastrado.\n";
        cout << "\nPressione Enter para voltar...";
        cin.ignore(1000, '\n');
        cin.get();
        return;
    }

    int opcao;
    string erro = "";

    do {
        vector<string> habilidades;
        for (const auto& t : trabalhadores)
            for (const auto& h : t.habilidades) {
                bool existe = false;
                for (const auto& nome : habilidades)
                    if (nome == h.nome) { existe = true; break; }
                if (!existe) habilidades.push_back(h.nome);
            }

        if (habilidades.empty()) {
            limparTela();
            cout << "===== Contratar Serviços =====\n\n";
            cout << "  Nenhuma habilidade cadastrada nos trabalhadores.\n";
            cout << "\nPressione Enter para voltar...";
            cin.ignore(1000, '\n');
            cin.get();
            break;
        }

        limparTela();
        cout << "===== Contratar Serviços =====\n\nHabilidades disponíveis:\n";
        for (size_t i = 0; i < habilidades.size(); i++)
            cout << "  " << i + 1 << ". " << habilidades[i] << "\n";
        cout << "\n  0. Voltar";
        if (!erro.empty()) cout << "\n\n  ! " << erro;
        cout << "\n\n  R: ";
        erro = "";

        if (!(cin >> opcao) || opcao < 0 || opcao > (int)habilidades.size()) {
            cin.clear();
            cin.ignore(1000, '\n');
            erro = "Opção inválida. Tente novamente.";
            continue;
        }
        if (opcao == 0) break;
        string habSelecionada = habilidades[opcao - 1];

        int dia, mes, ano;
        limparTela();
        cout << "===== Contratar Serviços =====\n\n";
        cout << "Serviço: " << habSelecionada << "\n";
        if (!erro.empty()) cout << "\n  ! " << erro << "\n";
        erro = "";
        cout << "\nData desejada (dd mm aaaa): ";
        if (!(cin >> dia >> mes >> ano)) {
            cin.clear();
            cin.ignore(1000, '\n');
            erro = "Data inválida. Tente novamente.";
            continue;
        }
        if (!dataValida(dia, mes, ano)) {
            cin.ignore(1000, '\n');
            erro = "Data fora do intervalo válido (dia 1-31, mês 1-12, ano >= 2026).";
            continue;
        }
        Data dataSelecionada(dia, mes, ano);

        vector<Trabalhador*> disponiveis;
        vector<Habilidade> habDisponiveis;
        for (auto& t : trabalhadores)
            for (const auto& h : t.habilidades)
                if (h.nome == habSelecionada && t.estaLivre(dataSelecionada)) {
                    disponiveis.push_back(&t);
                    habDisponiveis.push_back(h);
                    break;
                }

        if (disponiveis.empty()) {
            limparTela();
            cout << "===== Contratar Serviços =====\n\n";
            cout << "  Nenhum trabalhador disponível para " << habSelecionada
                 << " em " << dataSelecionada.toString() << ".\n";
            cout << "\nPressione Enter para voltar ao menu inicial...";
            cin.ignore(1000, '\n');
            cin.get();
            break;
        }

        limparTela();
        cout << "===== Contratar Serviços =====\n\n";
        cout << "Trabalhadores disponíveis em " << dataSelecionada.toString() << ":\n";
        for (size_t i = 0; i < disponiveis.size(); i++)
            cout << "  " << i + 1 << ". " << left << setw(20) << disponiveis[i]->nomeCompleto
                 << "R$ " << fixed << setprecision(2) << habDisponiveis[i].valorOperacao << "\n";
        cout << "\n  0. Voltar";
        if (!erro.empty()) cout << "\n\n  ! " << erro;
        cout << "\n\n  R: ";
        erro = "";

        if (!(cin >> opcao) || opcao < 0 || opcao > (int)disponiveis.size()) {
            cin.clear();
            cin.ignore(1000, '\n');
            erro = "Opção inválida. Tente novamente.";
            continue;
        }
        if (opcao == 0) continue;

        Trabalhador* trabSelecionado = disponiveis[opcao - 1];
        Habilidade habEscolhida = habDisponiveis[opcao - 1];

        cin.ignore(1000, '\n');
        string nomeCliente, cpfCliente;

        while (true) {
            limparTela();
            cout << "===== Contratar Serviços =====\n\n";
            if (!erro.empty()) cout << "  ! " << erro << "\n\n";
            erro = "";
            cout << "Seus dados:\n";
            cout << "  Nome: ";
            getline(cin, nomeCliente);
            nomeCliente = trim(nomeCliente);
            if (nomeCliente.empty()) {
                erro = "Nome não pode ser vazio.";
                continue;
            }
            cout << "  CPF (11 dígitos): ";
            getline(cin, cpfCliente);
            cpfCliente = trim(cpfCliente);
            if (!cpfValido(cpfCliente)) {
                erro = "CPF inválido. Digite exatamente 11 números.";
                continue;
            }
            break;
        }

        Cliente cliente(nomeCliente, cpfCliente, 0.0f);

        Trabalho t = trabSelecionado->contratar(cliente, dataSelecionada, habEscolhida);
        CSVManager::salvarTrabalho("data/trabalhos.csv", trabSelecionado->cpf, t);

        limparTela();
        cout << "===== Contratar Serviços =====\n\n";
        cout << "Contratação realizada!\n"
             << "  Trabalhador: " << trabSelecionado->nomeCompleto << "\n"
             << "  Serviço:     " << habEscolhida.nome << "\n"
             << "  Data:        " << dataSelecionada.toString() << "\n"
             << "  Valor:       R$ " << fixed << setprecision(2) << habEscolhida.valorOperacao << "\n";

        cout << "\nPressione Enter para voltar ao menu inicial...";
        cin.get();
        break;

    } while (true);
}

void menuDisponibilidade(vector<Trabalhador>& trabalhadores) {
    if (trabalhadores.empty()) {
        limparTela();
        cout << "===== Checar Disponibilidade =====\n\n";
        cout << "  Nenhum trabalhador cadastrado.\n";
        cout << "\nPressione Enter para voltar...";
        cin.ignore(1000, '\n');
        cin.get();
        return;
    }

    int opcao;
    string erro = "";

    do {
        limparTela();
        cout << "===== Checar Disponibilidade =====\n\n";

        for (size_t i = 0; i < trabalhadores.size(); i++) {
            cout << "  " << i + 1 << ". " << left << setw(20) << trabalhadores[i].nomeCompleto << "| ";
            for (size_t j = 0; j < trabalhadores[i].habilidades.size(); j++) {
                cout << trabalhadores[i].habilidades[j].nome;
                if (j + 1 < trabalhadores[i].habilidades.size()) cout << ", ";
            }
            cout << "\n";
        }

        cout << "\n  0. Voltar";
        if (!erro.empty()) cout << "\n\n  ! " << erro;
        cout << "\n\n  R: ";
        erro = "";

        if (!(cin >> opcao) || opcao < 0 || opcao > (int)trabalhadores.size()) {
            cin.clear();
            cin.ignore(1000, '\n');
            erro = "Opção inválida. Tente novamente.";
            continue;
        }

        if (opcao == 0) break;

        printAgendaTrabalhador(trabalhadores[opcao - 1]);

    } while (true);
}

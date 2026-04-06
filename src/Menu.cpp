#include "Menu.h"
#include "CSVManager.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

static void limparTela() {
    system("clear");
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

void menuContratacao(vector<Trabalhador>& trabalhadores) {
    int opcao;
    string erro = "";

    do {
        // --- passo 1: listar habilidades únicas disponíveis ---
        vector<string> habilidades;
        for (const auto& t : trabalhadores)
            for (const auto& h : t.habilidades) {
                bool existe = false;
                for (const auto& nome : habilidades)
                    if (nome == h.nome) { existe = true; break; }
                if (!existe) habilidades.push_back(h.nome);
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

        // --- passo 2: pedir data ---
        int dia, mes, ano;
        cout << "\nData desejada (dd mm aaaa): ";
        if (!(cin >> dia >> mes >> ano)) {
            cin.clear();
            cin.ignore(1000, '\n');
            erro = "Data inválida. Tente novamente.";
            continue;
        }
        Data dataSelecionada(dia, mes, ano);

        // --- passo 3: listar trabalhadores disponíveis ---
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

        // --- passo 4: dados do cliente ---
        string nomeCliente, cpfCliente;
        cin.ignore(1000, '\n');
        cout << "\nSeu nome: ";
        getline(cin, nomeCliente);
        cout << "Seu CPF: ";
        getline(cin, cpfCliente);

        Cliente cliente(nomeCliente, cpfCliente, 0.0f);

        // --- passo 5: confirmar contratação ---
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

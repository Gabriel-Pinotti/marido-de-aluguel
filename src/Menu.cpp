#include "Menu.h"
#include <iostream>
#include <iomanip>
using namespace std;

static void printAgendaTrabalhador(const Trabalhador& t) {
    cout << "\n----- " << t.nomeCompleto << " ";
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

void menuDisponibilidade(vector<Trabalhador>& trabalhadores) {
    int opcao;

    do {
        cout << "\n\n\n===== Checar Disponibilidade =====\n\n";

        for (size_t i = 0; i < trabalhadores.size(); i++) {
            cout << "  " << i + 1 << ". " << left << setw(20) << trabalhadores[i].nomeCompleto << "| ";
            for (size_t j = 0; j < trabalhadores[i].habilidades.size(); j++) {
                cout << trabalhadores[i].habilidades[j].nome;
                if (j + 1 < trabalhadores[i].habilidades.size()) cout << ", ";
            }
            cout << "\n";
        }

        cout << "\n  0. Voltar\n  R: ";

        if (!(cin >> opcao) || opcao < 0 || opcao > (int)trabalhadores.size()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nOpção inválida. Tente novamente.";
            continue;
        }

        if (opcao == 0) break;

        printAgendaTrabalhador(trabalhadores[opcao - 1]);

    } while (true);
}

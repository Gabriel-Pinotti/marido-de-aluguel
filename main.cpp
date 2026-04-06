#include <iostream>
#include <vector>
#include "src/Data.h"
#include "src/Habilidade.h"
#include "src/Pessoa.h"
#include "src/Cliente.h"
#include "src/Trabalho.h"
#include "src/Trabalhador.h"
#include "src/CSVManager.h"
#include "src/Menu.h"
using namespace std;

enum SelectedMenu {
    HIRE_MENU,
    AVAILABILITY_TABLE,
    REGISTER_WORKER,
};

void printMainMenu(const string& erro = "") {
    limparTela();
    cout << "===== Marido de Aluguel =====";
    cout << "\n\nSelecione o menu desejado:";
    cout << "\n1- Contratar serviços";
    cout << "\n2- Checar disponibilidade";
    cout << "\n3- Cadastrar trabalhador";
    cout << "\n0- Sair";
    if (!erro.empty()) cout << "\n\n  ! " << erro;
    cout << "\n\n  R: ";
}

int main() {
    vector<Trabalhador> trabalhadores = CSVManager::carregarTrabalhadores("data/trabalhadores.csv");
    CSVManager::carregarTrabalhos("data/trabalhos.csv", trabalhadores);

    int tempInput;
    string erro = "";

    do {
        printMainMenu(erro);
        erro = "";

        if (!(cin >> tempInput) || tempInput < 0 || tempInput > 3) {
            cin.clear();
            cin.ignore(1000, '\n');
            erro = "Opção inválida. Tente novamente.";
            continue;
        }

        if (tempInput == 0) break;

        switch (static_cast<SelectedMenu>(tempInput - 1)) {
            case HIRE_MENU:
                menuContratacao(trabalhadores);
                break;
            case AVAILABILITY_TABLE:
                menuDisponibilidade(trabalhadores);
                break;
            case REGISTER_WORKER:
                cin.ignore(1000, '\n');
                menuCadastrarTrabalhador(trabalhadores);
                break;
        }

    } while (true);

    return 0;
}

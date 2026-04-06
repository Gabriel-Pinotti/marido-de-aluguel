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
};

void printMainMenu() {
    cout << "\n\n\n";
    cout << "===== Marido de Aluguel =====";
    cout << "\n\n selecione o menu desejado:";
    cout << "\n1- Contratar serviços";
    cout << "\n2- Checar disponibilidade";
    cout << "\n0- Sair";
    cout << "\n  R: ";
}

int main() {
    vector<Trabalhador> trabalhadores = CSVManager::carregarTrabalhadores("data/trabalhadores.csv");
    CSVManager::carregarTrabalhos("data/trabalhos.csv", trabalhadores);

    int tempInput;

    do {
        printMainMenu();
        if (!(cin >> tempInput) || tempInput < 0 || tempInput > 2) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nOpção inválida. Tente novamente.";
            continue;
        }

        if (tempInput == 0) break;

        switch (static_cast<SelectedMenu>(tempInput - 1)) {
            case HIRE_MENU:
                // TODO: menuContratacao(trabalhadores)
                break;
            case AVAILABILITY_TABLE:
                menuDisponibilidade(trabalhadores);
                break;
        }

    } while (true);

    return 0;
}

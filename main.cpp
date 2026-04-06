#include <iostream>
#include "src/Data.h"
#include "src/Habilidade.h"
#include "src/Pessoa.h"
#include "src/Cliente.h"
#include "src/Trabalho.h"
#include "src/Trabalhador.h"
#include "src/CSVManager.h"
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
    cout << "\n  R: ";
}

// -- Global variables
SelectedMenu userMenuSelection;

int main() {
    int tempInput;

    do {
        printMainMenu();
        if (!(cin >> tempInput) || tempInput < 1 || tempInput > 2) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "\nOpção inválida. Tente novamente.";
            continue;
        }
        userMenuSelection = static_cast<SelectedMenu>(tempInput - 1);
        break;
    } while (true);

    return 0;
}

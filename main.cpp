#include <iostream>
#include "src/Data.h"
#include "src/Habilidade.h"
#include "src/Pessoa.h"
#include "src/Cliente.h"
#include "src/Trabalho.h"
#include "src/Trabalhador.h"

enum SelectedMenu {
    HIRE_MENU,
    AVAILABILITY_TABLE,
};

void printMainMenu() {
    std::cout << "\n\n\n";
    std::cout << "===== Marido de Aluguel =====";
    std::cout << "\n\n selecione o menu desejado:";
    std::cout << "\n1- Contratar serviços";
    std::cout << "\n2- Checar disponibilidade";
    std::cout << "\n  R: ";
}

// -- Global variables
SelectedMenu userMenuSelection;

int main() { // TODO Gabriel - modularizar
    printMainMenu();
    int tempInput;
    std::cin >> tempInput;
    userMenuSelection = static_cast<SelectedMenu>(tempInput - 1); // TODO Gabriel - tratar inputs inválidos

    return 0;
}

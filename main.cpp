#include <iostream>
// vamos usar "using namespace"? acho meio esquisito não usar

// TODO Gustavo - declarar as classes baseado no diagrama do Leandro
// arquivo diagrama.drawio acesso pelo draw.io

enum SelectedMenu {
    AVAILABILITY_TABLE,
    HIRE_MENU,
};

void printMainMenu(){
    std::cout << "\n\n\n";
    std::cout << "===== Marido de Aluguel =====";
    std::cout << "\n\n selecione o menu desejado:";
    std::cout << "\n1- Contratar serviços";
    std::cout << "\n2- Checar disponibilidade";
    std::cout << "\n  R: ";
};

// -- Global variables
SelectedMenu userMenuSelection;

int main(){ // TODO Gabriel - modularizar
    
    printMainMenu();
    int tempInput;
    std::cin >> tempInput;
    userMenuSelection = static_cast<SelectedMenu>(tempInput - 1); // TODO Gabriel - tratar inputs inválidos

    return 0;
};
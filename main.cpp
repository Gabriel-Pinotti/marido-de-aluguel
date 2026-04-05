#include <iostream>
#include <iostream>
using namespace std;

// forward declarations
class Data;
class Habilidade;
class Trabalho;
class Cliente;
class Trabalhador;

class Data {
public:
    int dia; 
    int mes; 
    int ano; 

    // construtor
    Data(int dia, int mes, int ano);
};

// habilidade que trabalhador pode ter
class Habilidade {
public:
    string nome;        // nome da habilidade
    float valorOperacao; // valor cobrado por hora/serviço dessa habilidade

    // construtor
    Habilidade(string n, float v);
};

class Pessoa {
public:
    string nomeCompleto; 
    int cpf;           

    // construtor
    Pessoa(string nomeCompleto, int cpf);
};

// composição com Trabalhador
// dependência com Data
// agregação com Habilidade
class Trabalho {
private:
    Data dataInicio;    
    Cliente& cliente;   
    Habilidade habilidade; 

public:
    // construtor
    Trabalho(Data d, Cliente& c, Habilidade h);
};

// herança de Pessoa
// agregação com Habilidade
// composição com Trabalho
// dependência com Data
class Trabalhador : public Pessoa {
public:
    Habilidade* habilidades; // array dinâmico
    int numHabilidades;      

private:
    Trabalho** trabalhos;    // array dinâmico de ponteiros 
    int numTrabalhos;        

public:
    // contrata o trabalhador para um cliente em uma data usando uma habilidade específica
    // return Trabalho criado
    Trabalho contratar(Cliente& c, Data d, Habilidade h);

    // verifica se o trabalhador está livre
    bool estaLivre(Data d);

    // se o trabalhador possui uma determinada habilidade
    bool temHabilidade(Habilidade h);

    // construtor
    Trabalhador(string nomeCompleto, int cpf,
                Habilidade* habilidades, int numHabilidades);

    // Destrutor (esse n sei se fiz certo guys, o Java n tem essas patifarias)
    ~Trabalhador();
};

// herança de Pessoa
// dependência com Trabalhador
class Cliente : public Pessoa {
private:
    float dinheiro; 

public:
    // return saldo atual 
    float quantosReais();

    // construtor
    Cliente(string nomeCompleto, int cpf, float dinheiro);
};



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

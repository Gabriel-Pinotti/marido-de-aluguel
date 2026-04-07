# Marido de Aluguel

![C++](https://img.shields.io/badge/C%2B%2B-17-00599C?logo=c%2B%2B&logoColor=white)
![CURSO](https://img.shields.io/badge/Matéria-OOP-blueviolet)

---

## Funcionalidades

- Cadastro de trabalhadores com habilidades e valores por serviço
- Contratação de serviços por profissional e data
- Consulta de disponibilidade por profissional
- Persistência de dados via arquivos CSV
- Menu interativo no terminal

## Estrutura do projeto

```
marido-de-aluguel/
├── main.cpp
├── src/
│   ├── Pessoa.h / Pessoa.cpp        # Classe base
│   ├── Cliente.h / Cliente.cpp      # Herda de Pessoa
│   ├── Trabalhador.h / Trabalhador.cpp  # Herda de Pessoa; agrega Habilidade, compõe Trabalho
│   ├── Trabalho.h / Trabalho.cpp    # Registro de serviço contratado
│   ├── Habilidade.h / Habilidade.cpp
│   ├── Data.h / Data.cpp
│   ├── CSVManager.h / CSVManager.cpp
│   └── Menu.cpp
└── data/
    ├── trabalhadores.csv
    └── trabalhos.csv
```

## Para compilar o projeto

**Linux / macOS**
```bash
g++ main.cpp src/*.cpp -o a.out
./a.out
```

**Windows (CMD + MinGW)**
```cmd
g++ main.cpp src\Cliente.cpp src\CSVManager.cpp src\Data.cpp src\Habilidade.cpp src\Menu.cpp src\Pessoa.cpp src\Trabalho.cpp src\Trabalhador.cpp -o a.exe
a.exe
```

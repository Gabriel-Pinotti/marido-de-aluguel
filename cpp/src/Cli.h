#pragma once

// Modo backend: o React (via plugin do Vite) executa o binário com argumentos.
// Recebe (argc, argv), executa o comando, imprime JSON em stdout e devolve o
// código de saída (0 = ok, != 0 = erro). Sem argumentos, main usa o menu.
int executarCli(int argc, char** argv);

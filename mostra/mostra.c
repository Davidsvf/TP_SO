#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define MAX 1000

int main (int argc, char* argv[]){
    if(argc != 2){
        char* erro = "Forma correta: mostra <ficheiro>\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }

    int ficheiro;
    ficheiro = open(argv[1], O_RDONLY);

    if(ficheiro < 0){
        char* erro= "Erro ao abrir ficheiro!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1; 
    }

    char buffer [MAX];

    ssize_t bytes_lidos = read(ficheiro, buffer, MAX);

    if(bytes_lidos < 0){
        char* erro = "Erro ao ler do ficheiro!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        close(ficheiro);
        return 1;
    }

    if(write(STDOUT_FILENO, buffer, bytes_lidos) == -1){
        char* erro = "Erro ao escrever na consola!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }

    if(close(ficheiro) < 0){
        char* erro = "Erro ao fechar ficheiro!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return -1;
    }


    return 0;

}
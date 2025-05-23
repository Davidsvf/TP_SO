#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

#define MAX 1000

int main(int argc, char *argv[]){
    if(argc != 2){
        char* erro = "Forma correta: conta <ficheiro>!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }

    int ficheiro = open(argv[1], O_RDONLY);  // abrir ficheiro para leitura

    if(ficheiro < 0){
        char* erro = "Erro ao abrir o ficherio!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }

    char caracter;
    int linhas=0;
    ssize_t leitura;

    while((leitura = read(ficheiro, &caracter, 1)) >0){   // enquanto houver caracteres para ler (read retoma 0 no fim do ficheiro)
        if(caracter == '\n'){
            linhas++;
        }
    }
    

    if(leitura <0){
        char* erro = "Erro ao ler do ficheiro!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        close (ficheiro);
        return 1;
    }

    
    if(close(ficheiro) < 0){                                     // fecahr ficheiro
        char* erro = "Erro ao fechar ficheiro!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }

    printf("Número de linhas: %d\n", linhas);

    return 0;

}
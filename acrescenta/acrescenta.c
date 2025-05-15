#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define TAM_BUFFER 1000

int main(int argc, char* argv[]){

    char buffer[TAM_BUFFER];

    // Verifica argumentos
    if(argc!=3){
        char* erro="Forma correta: acrescenta <origem> <destino>\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }

     // Abre ficheiro de origem para leitura
    int fd_in= open(argv[1], O_RDONLY);
    if(fd_in==-1){
        char *erro = "Erro ao abrir ficheiro de origem!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }
    
    // Abre ficehiro de destino para ecrerver no final do mesmo
    int fd_out=open(argv[2], O_WRONLY | O_APPEND);
    if(fd_out==-1){
        char* erro= "Erro ao abrir ficheiro de destino!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        close(fd_in);
        return 1;
    }

    // Copia conteúdo do origem para o final do destino
    ssize_t bytes_lidos;
    while ((bytes_lidos = read(fd_in, buffer, TAM_BUFFER)) > 0) {
        ssize_t bytes_escritos = write(fd_out, buffer, bytes_lidos);
        if (bytes_escritos == -1) {
            char *erro = "Erro ao escrever no ficheiro de destino!\n";
            write(STDERR_FILENO, erro, strlen(erro));
            close(fd_in);
            close(fd_out);
            return 1;
        }
    }

    if (bytes_lidos==-1) {
        char *erro= "Erro ao ler do ficheiro de origem!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        close(fd_in);
        close(fd_out);
        return 1;
    }

    close(fd_in);
    close(fd_out);

    return 0;
}
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>


#define TAM_BUFFER 1000

int main(int argc, char* argv[]){

    char buffer[TAM_BUFFER];

    // Verifica número de argumentos e mostra mensagem de erro caso nao seja diferente de 2
    if(argc!=2){
        char* erro="Forma correta: copia <ficheiro>\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }

    // Abre ficheiro passado por parâmetro em modo de leitura
    int fd_in= open(argv[1], O_RDONLY);
    if(fd_in==-1){
        char* erro="Erro ao tentar abrir ficheiro!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }
    
    // Abre e cria ficheiro para copiar
    int fd_out= open("ficheiro.copia", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if(fd_out== -1){
        char* erro = "Erro ao criar ficheiro destino!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        close(fd_in);

        if(close(fd_in)<0){
            char* erro = "Erro ao fechar ficheiro!\n";
            write(STDERR_FILENO, erro, strlen(erro));
            return 1;
        }
        return 1;
    }

    // Lê o ficheiro e copia para o ficheiro.copia
    ssize_t bytes_lidos;
    while((bytes_lidos= read(fd_in, buffer, TAM_BUFFER))>0){
        ssize_t bytes_escritos= write(fd_out, buffer, bytes_lidos);
        if(bytes_escritos==-1){
            char* erro = "Erro ao escrever no ficheiro.copia!\n";
            write(STDERR_FILENO, erro, strlen(erro));
            close(fd_in);
            close(fd_out);
            return 1;
        }
    }
    if(bytes_lidos==-1){
        char* erro = "Erro ao ler do ficheiro!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        close(fd_in);
        close(fd_out);
        return 1;
    }
    close(fd_in);
    close(fd_out);

    return 0;
}
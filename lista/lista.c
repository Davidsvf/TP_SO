#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define TAM_MENSAGEM 300
#define TAM_CAMINHO 1000

int main(int argc, char* argv[]){
    char* caminho;
    struct dirent* entrada;
    struct stat estado;
    DIR* diretorio;
    char mensagem[TAM_MENSAGEM];
    char* erro;

    if(argc!=2){
        erro= "Forma correta: lista <diretoria>\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }

    caminho=argv[1];

    diretorio=opendir(caminho);
    if(diretorio==NULL){
        erro="Erro ao abrir a diretoria\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }

    snprintf(mensagem, sizeof(mensagem), "Conteúdo da diretoria: %s\n", caminho);
    write(STDOUT_FILENO, mensagem, strlen(mensagem));

    while((entrada=readdir(diretorio))!=NULL){
        char caminho_completo[TAM_CAMINHO];

        snprintf(caminho_completo, sizeof(caminho_completo), "%s/%s", caminho, entrada->d_name);

        if(stat(caminho_completo, &estado)==-1){
            erro="Erro ao obter informações do ficheiro\n";
            write(STDERR_FILENO, erro, strlen(erro));
            continue;
        }

        if(S_ISDIR(estado.st_mode)){
            snprintf(mensagem, sizeof(mensagem), "Diretoria: %s\n", entrada->d_name);
        }else if(S_ISREG(estado.st_mode)){
            snprintf(mensagem, sizeof(mensagem), " Ficheiro: %s\n", entrada->d_name);
        }else{
            snprintf(mensagem, sizeof(mensagem), "Outro: %s\n", entrada->d_name);
        }

        write(STDOUT_FILENO, mensagem, strlen(mensagem));
    }
    closedir(diretorio);
    return 0;
}
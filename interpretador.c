#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 1000

void executar_comando(char* comando) {
    pid_t pid;
    int estado;

    pid = fork();

    if (pid < 0){
        char *erro = "Fork falhou\n";
        write(STDERR_FILENO,erro, strlen(erro));
        exit(1);
    }
    else if (pid == 0) {

        char* argumentos[MAX / 2 + 1];
        char* token;
        int i = 0;

        token = strtok(comando, " \t\n");

        while (token != NULL) {
            argumentos[i++] = token;
            token = strtok(NULL, " \t\n");
        }
        argumentos[i] = NULL;

        if (execvp(argumentos[0], argumentos) < 0){
            char *erro = "Execução falhou\n";
            write(STDERR_FILENO,erro, strlen(erro));
            exit(1);
        }
    }
    else {

        waitpid(pid, &estado, 0);

        if (WIFEXITED(estado)) {
            int codigo = WEXITSTATUS(estado);
            printf("O comando %s terminou com o código %d\n", comando, estado);
        }
        else {
            char *erro = "Comando não funcionou\n";
            write(STDOUT_FILENO, erro, strlen(erro));
        }
    }
}

int main() {
    char comando[MAX];
    while (1) {
        const char* prompt = "% ";
        write(STDOUT_FILENO, prompt, strlen(prompt));

        if (fgets(comando, sizeof(comando), stdin) == NULL){
            char *erro = "Erro ao ler comando";
            write(STDERR_FILENO, erro, strlen(erro));
            exit(1);
        }

        size_t tamanho = strlen(comando);
        if (comando[tamanho - 1] == '\n') {
            comando[tamanho - 1] = '\0';
        }

        if (strcmp(comando, "termina") == 0) {
            break;
        }

        if (strncmp(comando, "cd ", 3) == 0) {
            char* caminho = comando + 3;
            if (chdir(caminho) != 0) {
                char *erro = "cd falhou";
                write(STDERR_FILENO, erro, strlen(erro));
            }
            continue;
        }

        executar_comando(comando);
    }

    return 0;
}
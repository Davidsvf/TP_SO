#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX 1000

void execute_command(char* command) {
    pid_t pid;
    int codigo_saida;

    pid = fork();

    if (pid < 0){
        char *erro = "Fork falhou\n";
        write(STDERR_FILENO,erro, sizeof(erro));
        exit(1);
    }
    else if (pid == 0) {

        char* argumentos[MAX / 2 + 1];
        char* token;
        int indice = 0;

        token = strtok(command, " \t\n");

        while (token != NULL) {
            argumentos[indice++] = token;
            token = strtok(NULL, " \t\n");
        }
        argumentos[indice] = NULL;

        if (execvp(argumentos[0], argumentos) < 0){
            char *erro = "Execução falhou\n";
            write(STDERR_FILENO,erro, sizeof(erro));
            exit(1);
        }
    }
    else {

        waitpid(pid, &codigo_saida, 0);

        if (WIFEXITED(codigo_saida)) {
            int exit_status = WEXITSTATUS(codigo_saida);
            char msg[128];
            snprintf(msg, sizeof(msg), "O comando %s terminou com o código %d\n", command, exit_status);
            write(STDOUT_FILENO, msg, strlen(msg));
        }
        else {
            char msg[128] = "Comando não funcionou\n";
            write(STDOUT_FILENO, msg, strlen(msg));
        }
    }
}

int main() {
    char command[MAX];
    while (1) {
        const char* prompt = "% ";
        write(STDOUT_FILENO, prompt, strlen(prompt));

        if (fgets(command, sizeof(command), stdin) == NULL){
            char *erro = "Erro ao ler comando";
            write(STDERR_FILENO, erro, sizeof(erro));
            exit(1);
        }

        size_t tamanho = strlen(command);
        if (command[tamanho - 1] == '\n') {
            command[tamanho - 1] = '\0';
        }

        if (strcmp(command, "termina") == 0) {
            break;
        }

        if (strncmp(command, "cd ", 3) == 0) {
            char* caminho = command + 3;
            if (chdir(caminho) != 0) {
                char *erro = "cd falhou";
                write(STDERR_FILENO, erro, sizeof(erro));
            }
            continue;
        }

        execute_command(command);
    }

    return 0;
}
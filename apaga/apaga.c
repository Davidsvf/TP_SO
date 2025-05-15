#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char* argv[]){
    if(argc != 2){
        char* erro = "Forma correta: apaga <ficheiro>\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }

    int apagar;
    apagar = unlink(argv[1]);

    if(apagar < 0){
        char* erro = "Erro, ficheiro não existe!\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }
    return 0;
}
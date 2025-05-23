#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

#define MAX 1000

int main(int argc, char *argv[]){
    struct stat info;

    if(argc != 2){
        char *erro = "Forma correta: informa <ficheiro>\n";
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }

    if(stat(argv[1], &info) == -1){
        char *erro = "Erro ao aceder ao ficheiro!\n";                     
        write(STDERR_FILENO, erro, strlen(erro));
        return 1;
    }

    char *tipo = "Tipo de ficheiro: ";
    write(STDOUT_FILENO, tipo, strlen(tipo));

    if (S_ISREG(info.st_mode))
        write(STDOUT_FILENO, "Ficheiro normal\n", 16);
    else if (S_ISDIR(info.st_mode))
        write(STDOUT_FILENO, "Diretoria\n", 10);
    else if (S_ISLNK(info.st_mode))
        write(STDOUT_FILENO, "Link simbolico\n", 15);                // escreve  tipo de ficheiro
    else if (S_ISCHR(info.st_mode))
        write(STDOUT_FILENO, "Dispositivo de caractere\n", 26);
    else if (S_ISBLK(info.st_mode))
        write(STDOUT_FILENO, "Dispositivo de bloco\n", 22);
    else if (S_ISFIFO(info.st_mode))
        write(STDOUT_FILENO, "FIFO (pipe)\n", 12);
    else if (S_ISSOCK(info.st_mode))
        write(STDOUT_FILENO, "Socket\n", 7);
    else
        write(STDOUT_FILENO, "Tipo desconhecido\n", 19);


    printf("i-node: %ld\n", (long) info.st_ino);                


    struct passwd *pw = getpwuid(info.st_uid);
    struct group *gr = getgrgid(info.st_gid);

    printf("Utilizador dono: %s (UID: %d)\n", pw->pw_name, info.st_uid);
    printf("Grupo: %s (GID: %d)\n", gr->gr_name, info.st_gid);

    char data[64];
    strftime(data, sizeof(data), "%Y-%m-%d %H:%M:%S", localtime(&info.st_ctime));
    printf("Data de criação: %s\n", data);

    strftime(data, sizeof(data), "%Y-%m-%d %H:%M:%S", localtime(&info.st_atime));
    printf("Último acesso: %s\n", data);

    strftime(data, sizeof(data), "%Y-%m-%d %H:%M:%S", localtime(&info.st_mtime));
    printf("Última modificação: %s\n", data);

    return 0;
}
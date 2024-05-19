#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

// Função para imprimir permissões do arquivo
void print_permissions(mode_t mode) {
    char permissions[11];
    strcpy(permissions, "----------");

    if (S_ISDIR(mode)) permissions[0] = 'd';
    if (S_ISLNK(mode)) permissions[0] = 'l';
    if (mode & S_IRUSR) permissions[1] = 'r';
    if (mode & S_IWUSR) permissions[2] = 'w';
    if (mode & S_IXUSR) permissions[3] = 'x';
    if (mode & S_IRGRP) permissions[4] = 'r';
    if (mode & S_IWGRP) permissions[5] = 'w';
    if (mode & S_IXGRP) permissions[6] = 'x';
    if (mode & S_IROTH) permissions[7] = 'r';
    if (mode & S_IWOTH) permissions[8] = 'w';
    if (mode & S_IXOTH) permissions[9] = 'x';

    printf("%s ", permissions);
}

// Função para imprimir informações do arquivo no formato longo
void print_long_format(const char *name, const struct stat *sb) {
    struct passwd *pw = getpwuid(sb->st_uid);
    struct group *gr = getgrgid(sb->st_gid);
    char timebuf[80];
    strftime(timebuf, sizeof(timebuf), "%b %d %H:%M", localtime(&sb->st_mtime));

    print_permissions(sb->st_mode);
    printf("%ld %s %s %5ld %s %s\n",
           (long) sb->st_nlink,
           pw->pw_name,
           gr->gr_name,
           (long) sb->st_size,
           timebuf,
           name);
}

int main(int argc, char *argv[]) {
    DIR *dir;
    struct dirent *ent;
    struct stat sb;
    int show_all = 0, long_format = 0;
    char *dir_name = "."; // Diretório padrão é o atual

    // Processa os argumentos
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (strchr(argv[i], 'a')) show_all = 1;
            if (strchr(argv[i], 'l')) long_format = 1;
        } else {
            dir_name = argv[i];
        }
    }

    dir = opendir(dir_name);
    if (dir == NULL) {
        perror("Não foi possível abrir o diretório");
        return 2;
    }

    while ((ent = readdir(dir)) != NULL) {
        if (!show_all && ent->d_name[0] == '.') {
            continue;
        }

        if (long_format) {
            if (stat(ent->d_name, &sb) == -1) {
                perror("stat");
                continue;
            }
            print_long_format(ent->d_name, &sb);
        } else {
            printf("%s\n", ent->d_name);
        }
    }

    closedir(dir);
    return 0;
}

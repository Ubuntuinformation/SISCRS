#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define MAX_CMD_LEN 1024
#define MAX_ARGS 64

typedef struct {
    const char* name;
    const char* description;
    int (*func)(int argc, char** argv);
} command_t;

/* Comandos built-in */

int cmd_help(int argc, char** argv);
int cmd_echo(int argc, char** argv);
int cmd_pwd(int argc, char** argv);
int cmd_cd(int argc, char** argv);
int cmd_ls(int argc, char** argv);
int cmd_mkdir(int argc, char** argv);
int cmd_rm(int argc, char** argv);
int cmd_cat(int argc, char** argv);
int cmd_gcc(int argc, char** argv);
int cmd_clang(int argc, char** argv);
int cmd_exit(int argc, char** argv);
int cmd_clear(int argc, char** argv);
int cmd_whoami(int argc, char** argv);
int cmd_date(int argc, char** argv);

command_t builtins[] = {
    {"help", "Mostra ajuda", cmd_help},
    {"echo", "Exibe um texto", cmd_echo},
    {"pwd", "Diretório atual", cmd_pwd},
    {"cd", "Muda de diretório", cmd_cd},
    {"ls", "Lista arquivos", cmd_ls},
    {"mkdir", "Cria diretório", cmd_mkdir},
    {"rm", "Remove arquivo", cmd_rm},
    {"cat", "Exibe conteúdo de arquivo", cmd_cat},
    {"gcc", "Compilador GCC", cmd_gcc},
    {"clang", "Compilador Clang", cmd_clang},
    {"clear", "Limpa tela", cmd_clear},
    {"whoami", "Exibe nome do usuário", cmd_whoami},
    {"date", "Exibe data e hora", cmd_date},
    {"exit", "Sai do shell", cmd_exit},
    {NULL, NULL, NULL}
};

int running = 1;

int cmd_help(int argc, char** argv) {
    (void)argc;
    (void)argv;
    
    printf("=== SiscrShell v1.0 ===\n");
    printf("Inspirado em Unix. Forní no OS - Sistema Operacional Livre\n\n");
    printf("Comandos disponíveis:\n");
    
    for (int i = 0; builtins[i].name; i++) {
        printf("  %-15s - %s\n", builtins[i].name, builtins[i].description);
    }
    
    printf("\nComandos externos podem ser executados diretamente.\n");
    return 0;
}

int cmd_echo(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        printf("%s", argv[i]);
        if (i < argc - 1) printf(" ");
    }
    printf("\n");
    return 0;
}

int cmd_pwd(int argc, char** argv) {
    (void)argc;
    (void)argv;
    
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        printf("Erro ao obter diretório\n");
        return 1;
    }
    return 0;
}

int cmd_cd(int argc, char** argv) {
    if (argc < 2) {
        const char* home = getenv("HOME");
        if (chdir(home ? home : "/") != 0) {
            printf("cd: erro ao mudar de diretório\n");
            return 1;
        }
    } else {
        if (chdir(argv[1]) != 0) {
            printf("cd: %s: Arquivo ou diretório não encontrado\n", argv[1]);
            return 1;
        }
    }
    return 0;
}

int cmd_ls(int argc, char** argv) {
    (void)argc;
    (void)argv;
    
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "/bin/ls -la %s", argc > 1 ? argv[1] : ".");
    system(cmd);
    return 0;
}

int cmd_mkdir(int argc, char** argv) {
    if (argc < 2) {
        printf("mkdir: argumento necessário\n");
        return 1;
    }
    return mkdir(argv[1], 0755);
}

int cmd_rm(int argc, char** argv) {
    if (argc < 2) {
        printf("rm: argumento necessário\n");
        return 1;
    }
    return remove(argv[1]);
}

int cmd_cat(int argc, char** argv) {
    if (argc < 2) {
        printf("cat: argumento necessário\n");
        return 1;
    }
    
    FILE* f = fopen(argv[1], "r");
    if (!f) {
        printf("cat: %s: Arquivo não encontrado\n", argv[1]);
        return 1;
    }
    
    int c;
    while ((c = fgetc(f)) != EOF) {
        putchar(c);
    }
    fclose(f);
    return 0;
}

int cmd_gcc(int argc, char** argv) {
    char cmd[1024] = "gcc";
    for (int i = 1; i < argc; i++) {
        strcat(cmd, " ");
        strcat(cmd, argv[i]);
    }
    return system(cmd);
}

int cmd_clang(int argc, char** argv) {
    char cmd[1024] = "clang";
    for (int i = 1; i < argc; i++) {
        strcat(cmd, " ");
        strcat(cmd, argv[i]);
    }
    return system(cmd);
}

int cmd_clear(int argc, char** argv) {
    (void)argc;
    (void)argv;
    system("clear");
    return 0;
}

int cmd_whoami(int argc, char** argv) {
    (void)argc;
    (void)argv;
    
    const char* user = getenv("USER");
    if (user) {
        printf("%s\n", user);
    } else {
        printf("root\n");
    }
    return 0;
}

int cmd_date(int argc, char** argv) {
    (void)argc;
    (void)argv;
    system("date");
    return 0;
}

int cmd_exit(int argc, char** argv) {
    (void)argc;
    (void)argv;
    running = 0;
    printf("Bye!\n");
    return 0;
}

void parse_command(const char* line, int* argc, char** argv) {
    *argc = 0;
    char* buffer = malloc(strlen(line) + 1);
    strcpy(buffer, line);
    
    char* token = strtok(buffer, " \t\n");
    while (token && *argc < MAX_ARGS - 1) {
        argv[*argc] = malloc(strlen(token) + 1);
        strcpy(argv[*argc], token);
        (*argc)++;
        token = strtok(NULL, " \t\n");
    }
    argv[*argc] = NULL;
    free(buffer);
}

void execute_command(int argc, char** argv) {
    if (argc == 0) return;
    
    /* Verificar comandos built-in */
    for (int i = 0; builtins[i].name; i++) {
        if (strcmp(argv[0], builtins[i].name) == 0) {
            builtins[i].func(argc, argv);
            return;
        }
    }
    
    /* Executar comando externo */
    pid_t pid = fork();
    if (pid == 0) {
        execvp(argv[0], argv);
        printf("%s: comando não encontrado\n", argv[0]);
        exit(127);
    } else if (pid > 0) {
        wait(NULL);
    } else {
        printf("Erro ao criar processo\n");
    }
}

void show_prompt(void) {
    char cwd[256];
    char* user = (char*)getenv("USER");
    if (!user) user = "root";
    
    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        strcpy(cwd, "~");
    }
    
    printf("[%s@fortini %s]$ ", user, cwd);
    fflush(stdout);
}

int main(void) {
    printf("=== SiscrShell v1.0 ===\n");
    printf("Fortini OS - Sistema Operacional Livre\n");
    printf("Digite 'help' para ajuda\n\n");
    
    char line[MAX_CMD_LEN];
    char* argv[MAX_ARGS];
    int argc;
    
    while (running) {
        show_prompt();
        
        if (fgets(line, sizeof(line), stdin) == NULL) {
            break;
        }
        
        /* Remove newline */
        size_t len = strlen(line);
        if (len > 0 && line[len-1] == '\n') {
            line[len-1] = '\0';
        }
        
        parse_command(line, &argc, argv);
        execute_command(argc, argv);
        
        /* Liberar memória dos argumentos */
        for (int i = 0; i < argc; i++) {
            free(argv[i]);
        }
    }
    
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../argparse.h"

static const char *const usages[] = {
    "basic [options] [[--] args]",
    "basic [options]",
    NULL,
};

#define PERM_READ  (1<<0)
#define PERM_WRITE (1<<1)
#define PERM_EXEC  (1<<2)

int LLVMFuzzerTestOneInput(const uint8_t *Data, size_t Size){
    int force = 0;
    int test = 0;
    int int_num = 0;
    float flt_num = 0.f;
    const char *path = NULL;
    int perms = 0;
    struct argparse_option options[] = {
        OPT_HELP(),
        OPT_GROUP("Basic options"),
        OPT_BOOLEAN('f', "force", &force, "force to do", NULL, 0, 0),
        OPT_BOOLEAN('t', "test", &test, "test only", NULL, 0, 0),
        OPT_STRING('p', "path", &path, "path to read", NULL, 0, 0),
        OPT_INTEGER('i', "int", &int_num, "selected integer", NULL, 0, 0),
        OPT_FLOAT('s', "float", &flt_num, "selected float", NULL, 0, 0),
        OPT_GROUP("Bits options"),
        OPT_BIT(0, "read", &perms, "read perm", NULL, PERM_READ, OPT_NONEG),
        OPT_BIT(0, "write", &perms, "write perm", NULL, PERM_WRITE, 0),
        OPT_BIT(0, "exec", &perms, "exec perm", NULL, PERM_EXEC, 0),
        OPT_END(),
    };

    if(Size<1) return 1;

    int argc = 0;
    char **argv;
    
    char *opt_fuzz = malloc(Size+1);

    memcpy(opt_fuzz, Data, Size);
    opt_fuzz[Size] = 0;
    
    //for argv[0]
    int i=0;
    argv = (char **)malloc(sizeof(char *));
    *argv = (char *)malloc(12);
    strncpy(*argv,"./fuzz-main",11);
    (*argv)[11] = '\0';
    argc++;
    //printf("here: %s\n",argv[i]);

    char *temp = strtok(opt_fuzz," ");
    while(temp != NULL){
        argc++;
        i++;
        argv = (char **)realloc(argv, sizeof(char *)*argc);
        argv[i] = temp;
        temp = strtok(NULL," ");
    }

    printf("check: %d\n",argc);
    for(int i=0;i<argc;i++){
        printf("argv[%d]: %s\n",i,argv[i]);
    }

    struct argparse argparse;
    argparse_init(&argparse, options, usages, 0);
    argparse_describe(&argparse, "\nA brief description of what the program does and how it works.", "\nAdditional description of the program after the description of the arguments.");
    argc = argparse_parse(&argparse, argc, (const char **)argv);
    if (force != 0)
        printf("force: %d\n", force);
    if (test != 0)
        printf("test: %d\n", test);
    if (path != NULL)
        printf("path: %s\n", path);
    if (int_num != 0)
        printf("int_num: %d\n", int_num);
    if (flt_num != 0)
        printf("flt_num: %g\n", flt_num);
    if (argc != 0) {
        printf("argc: %d\n", argc);
        int i;
        for (i = 0; i < argc; i++) {
            printf("argv[%d]: %s\n", i, *(argv + i));
        }
    }
    if (perms) {
        printf("perms: %d\n", perms);
    }
    for(int i=0;i<argc;i++){
        free(argv[i]);
    }
    free(argv);
    return 0;
}
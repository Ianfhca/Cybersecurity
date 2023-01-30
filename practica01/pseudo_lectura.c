#include <stdio.h>
#include <string.h>

int main() {

    char line[500];
    FILE *f = fopen("data", "r");
    char *token;
    const char delimiter[] = " ";
    
    if (f == NULL) {
        printf("No se puede abrir el archivo");
        return 1;
    }
    
    while (fgets(line, 100, f) != NULL) {
        token = strtok(line, delimiter);
        while (token != NULL) {
            printf("Mensaje %s\n", token);
            token = strtok(NULL, delimiter);
        }
        // printf("%s", line);
    }
    
    fclose(f);
    return 0;
}
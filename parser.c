#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * This is a fairly simple Markdown to HTML converter.
 * TODO: 
 *  - Add more stuff
 *  - Optimize this more
 *  - Fix h1 shit
*/

bool startsWith(const char *a, const char *b) {
    return (strncmp(a, b, strlen(b)) == 0);
}

bool startsWithAndEndsWith(const char *line, const char *startChar, const char *endChar) {
    size_t len = strlen(line);
    if (len >= 2 && line[0] == *startChar && line[len - 1] == *endChar) {
        return true;
    }
    return false;
}

char* extractText(const char *line, const char *startToken, const char *endToken) {
    const char *start = strstr(line, startToken);
    if (start != NULL) {
        start += strlen(startToken);
        const char *end = strstr(start, endToken);
        if (end != NULL) {
            size_t length = end - start;
            char *extracted = malloc(length + 1);
            if (extracted != NULL) {
                strncpy(extracted, start, length);
                extracted[length] = '\0';
                return extracted;
            }
        }
    }
    return NULL;
}

char* createFile(char* fileName, char* content) {
    FILE *file;
    file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Unable to create html file");
        exit(0);
    }
    fprintf(file, "%s", content);
    fclose(file);
}

void doMagic(char* fileName) {
    system("cls");
    FILE* file;
    char* line = NULL;
    size_t len = 0;
    int read;

    file = fopen(fileName, "r");
    if (file == NULL) {
        printf("Unable to locate file!");
        exit(0);
    }

    char *italic = NULL;

    while ((read = (int)getline(&line, &len, file)) != -1) {
        if (startsWith(line, "#")) {
            if (line[strlen(line) - 1] == '\n') {
                line[strlen(line) - 1] = '\0';
            }
            printf("<h1>%s</h1>\n", line + 1);
        }
        char *extractedItalic = extractText(line, "*", "*");
        if (extractedItalic != NULL) {
            char* italicText = malloc(strlen(extractedItalic) + 8);
            if (italicText != NULL) {
                strcpy(italicText, "<i>");
                strcat(italicText, extractedItalic);
                strcat(italicText, "</i>\n");
                if (italic == NULL) {
                    italic = italicText;
                } else {
                    size_t length = strlen(italic);
                    size_t newLength = length + strlen(italicText) + 1;
                    italic = realloc(italic, newLength);
                    strcat(italic, italicText);
                    free(italicText);
                }
            }
            free(extractedItalic);
        }
    }

    if (italic != NULL) {
        printf("%s", italic);
        free(italic);
    }
    
    if (line != NULL) {
        free(line);
    }
    fclose(file);
}

int main(int argc, char *argv[]) {
    if (argc > 1 && strcmp(argv[1], "--new") == 0) {
        doMagic(argv[2]);
    } else {
        printf("Usage: %s --new <filename>\n", argv[0]);
    }
    return 0;
}

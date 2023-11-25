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
 *  - Whenever I feel like it, move the functions to a separate file
 *  - Paragraphs
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

char* createFile(char* fileName, char* content) { // for later
    FILE *file;
    file = fopen(fileName, "w");
    if (file == NULL) {
        printf("Unable to create html file");
        exit(0);
    }
    fprintf(file, "%s", content);
    fclose(file);
}

char* convertExtractedIdk(char* extracted, char* HTML_tag1, char*HTML_tag2, int amount) {
    if (strlen(extracted) == 0) {
        free(extracted);
        return NULL;
    }
    char* text = malloc(strlen(extracted) + amount); // for example the italic text: (extracted) + 8
    if (text != NULL) {
        strcpy(text, HTML_tag1);
        strcat(text, extracted);
        strcat(text, HTML_tag2);
        strcat(text, "\n");
    }
    return text;
}

char* concatenateStrings(char* first, char* second) {
    size_t length = strlen(first);
    size_t newLength = length + strlen(second) + 1;
    first = realloc(first, newLength);
    strcat(first, second);
    return first;
}

void handleHTML(char* extracted, char* HTML_tag1, char* HTML_tag2, int amount, char** chr) { // it took me 15 mins to fix this shit
    char* text = convertExtractedIdk(extracted, HTML_tag1, HTML_tag2, amount);
    if (text != NULL) {
        if (*chr == NULL) {
            *chr = strdup(text);
        } else {
            *chr = concatenateStrings(*chr, text);
        }
        free(text);
    }
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
        exit(EXIT_FAILURE);
    }

    char *italic = NULL;
    char *bold = NULL;

    while ((read = (int)getline(&line, &len, file)) != -1) {
        if (startsWith(line, "#")) {
            if (line[strlen(line) - 1] == '\n') {
                line[strlen(line) - 1] = '\0';
            }
            printf("<h1>%s</h1>\n", line + 1);
        }

        char *extractedItalic = extractText(line, "*", "*");
        if (extractedItalic != NULL) {
            handleHTML(extractedItalic, "<i>", "</i>", 8, &italic);
            free(extractedItalic);
        }

        char *extractedBold = extractText(line, "**", "**");
        if (extractedBold != NULL) {
            handleHTML(extractedBold, "<strong>", "</strong>", 18, &bold);
            free(extractedBold);
        }
    }

    if (italic != NULL) {
        printf("%s", italic);
        free(italic);
    }

    if (bold != NULL) {
        printf("%s", bold);
        free(bold);
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

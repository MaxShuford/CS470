#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Max_Shuford_LIBFS.h"

FileEntry file_table[MAX_FILES];
int file_count = 0;

int fileCreate(const char* filename) {
    
    // Check if file already exists
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].filename, filename) == 0) {
            printf("File '%s' already exists.\n", filename);
            return -1;
        }
    }

    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Error creating file");
        return -1;
    }
    fclose(file);

    strcpy(file_table[file_count].filename, filename);
    file_table[file_count].size = 0;
    file_table[file_count].is_open = 0;
    file_count++;

    printf("File '%s' created successfully.\n", filename);
    return 0;
}

int fileOpen(const char* filename) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].filename, filename) == 0) {
            if(file_table[i].is_open == 0) {
                file_table[i].is_open = 1;
                printf("File '%s' opened successfully.\n", filename);
                return i;
            } else {
                printf("File '%s' is already open.\n", filename);
                return i;
            }
        }
    }
    printf("File '%s' not found.\n", filename);
    return -1;
}

int fileWrite(int file_index, const char* data) {
    
    if (file_index < 0 || file_index >= file_count) {
        printf("Invalid file index.\n");
        return -1;
    }
    if (file_table[file_index].is_open == 1) {
        FILE *file = fopen(file_table[file_index].filename, "a");
        if (!file) {
            perror("Error writing to file");
            return -1;
        }
        fwrite(data, 1, strlen(data), file);
        fclose(file);
        file_table[file_index].size += strlen(data);
        printf("Data written to file '%s' successfully.\n", file_table[file_index].filename);
        return 0;
    } else {
        printf("File '%s' is not open.\n", file_table[file_index].filename);
        return -1;
    }
}

char* fileRead(int file_index) {

    if (file_index < 0 || file_index >= file_count) {
        printf("Invalid file index.\n");
        return NULL;
    }

    if (!file_table[file_index].is_open) {
        printf("File must be open before reading.\n");
        return NULL;
    }

    FILE *file = fopen(file_table[file_index].filename, "r");

    if (!file) {
        perror("Error opening file");
        return NULL;
    }

    char *buffer = malloc(MAX_FILE_SIZE);

    if (!buffer) {
        printf("Memory allocation failed.\n");
        fclose(file);
        return NULL;
    }

    size_t bytes_read = fread(buffer, 1, MAX_FILE_SIZE - 1, file);
    buffer[bytes_read] = '\0';

    fclose(file);

    return buffer;
}

int fileClose(int file_index) {
    
    if (file_index < 0 || file_index >= file_count) {
        printf("Invalid file index.\n");
        return -1;
    }
    else if (file_table[file_index].is_open == 1) {
        file_table[file_index].is_open = 0;
        printf("File '%s' closed successfully.\n", file_table[file_index].filename);
        return 0;
    } else {
        printf("File '%s' is not open.\n", file_table[file_index].filename);
        return -1;
    }
}

int fileDelete(const char* filename) {
    for (int i = 0; i < file_count; i++) {
        if (strcmp(file_table[i].filename, filename) == 0) {
            if (file_table[i].is_open == 1) {
                printf("Cannot delete file '%s' because it is open.\n", filename);
                return -1;
            }
            if (remove(filename) != 0) {
                perror("Error deleting file");
                return -1;
            }
            for (int j = i; j < file_count - 1; j++) {
                file_table[j] = file_table[j + 1];
            }
            file_count--;
            printf("File '%s' deleted successfully.\n", filename);
            return 0;
        }
    }
    printf("File '%s' not found.\n", filename);
    return -1;
}
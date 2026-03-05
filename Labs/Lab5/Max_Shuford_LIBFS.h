#ifndef Max_Shuford_LIBFS_H
#define Max_Shuford_LIBFS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FILENAME 50
#define MAX_FILES 100
#define MAX_FILE_SIZE 1024

typedef struct {
    char filename[MAX_FILENAME];
    int size;
    int is_open;
} FileEntry;

extern FileEntry file_table[MAX_FILES];
extern int file_count;

int fileCreate(const char* filename);
int fileOpen(const char* filename);
char* fileRead(int file_index);
int fileWrite(int file_index, const char* data);
int fileClose(int file_index);
int fileDelete(const char* filename);

#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Max_Shuford_LIBFS.h"

void displayMenu() {
    printf("\n=== File System Test Menu ===\n");
    printf("1. Create file\n");
    printf("2. Write to file\n");
    printf("3. Open file\n");
    printf("4. Read file contents\n");
    printf("5. Delete file\n");
    printf("6. Close file\n");
    printf("7. Exit\n");
    printf("Enter your choice (1-7): ");
}

int main() {

    const char* filename = "Max_Shuford_Introduction.txt";
    int choice;
    int file = -1;

    while (1) {

        displayMenu();

        scanf("%d", &choice);
        getchar();

        switch (choice) {

        case 1:

            printf("\n--- Creating File ---\n");

            file = fileCreate(filename);

            break;

        case 2:

            printf("\n--- Writing to File ---\n");

            file = fileOpen(filename);

            if (file >= 0) {

                const char* introduction =
                "Hello, I am Max Shuford, a computer science student. "
                "I am interested in open source development and cybersecurity. "
                "I have experience with various programming languages including C, Python, and Java.\n\n"
                "I enjoy working on projects that challenge my problem-solving skills and allow me to learn new technologies. "
                "In my free time, I contribute to open source projects. "
                "I am passionate about technology and always eager to collaborate with others in the tech community.";

                fileWrite(file, introduction);

                fileClose(file);
            }
            else {
                printf("Error opening file for writing.\n");
            }

            break;
        
        case 3:

            printf("\n--- Opening File ---\n");

            file = fileOpen(filename);
            break;
        case 4:

            printf("\n--- Reading File ---\n");

            file = fileOpen(filename);

            if (file >= 0) {

                char* content = fileRead(file);

                if (content) {
                    printf("\nFile Contents:\n%s\n", content);
                    free(content);
                }

                fileClose(file);
            }

            break;

        case 5:

            printf("\n--- Deleting File ---\n");

            if (fileDelete(filename) == 0)
                printf("File deleted successfully!\n");
            else
                printf("Error deleting file.\n");

            break;

        case 6:
            printf("\n--- Close File ---\n");
            if (file >= 0) {
                fileClose(file);
            } else {
                printf("No file is currently open.\n");
            }
            break;
        case 7:

            printf("Exiting program. Goodbye!\n");
            return 0;

        default:
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
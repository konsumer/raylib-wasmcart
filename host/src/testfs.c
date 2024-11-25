#include <stdio.h>
#include <stdlib.h>
#include <physfs.h>

void print_physfs_error(void) {
    PHYSFS_ErrorCode error = PHYSFS_getLastErrorCode();
    const char* errorMsg = PHYSFS_getErrorByCode(error);
    printf("PhysFS Error %d: %s\n", error, errorMsg);
}

// List all files in the mounted archive
void list_files(const char* dir) {
    char **rc = PHYSFS_enumerateFiles(dir);
    char **i;

    for (i = rc; *i != NULL; i++) {
        PHYSFS_Stat stat;
        char fullpath[512];
        snprintf(fullpath, sizeof(fullpath), "%s/%s", dir, *i);
        
        if (PHYSFS_stat(fullpath, &stat)) {
            if (stat.filetype == PHYSFS_FILETYPE_DIRECTORY) {
                printf("DIR:  %s\n", fullpath);
                list_files(fullpath);  // Recurse into subdirectories
            } else {
                printf("FILE: %s (size: %lld)\n", fullpath, stat.filesize);
            }
        }
    }

    PHYSFS_freeList(rc);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <zipfile>\n", argv[0]);
        return 1;
    }

    // Initialize PhysFS
    if (!PHYSFS_init(argv[0])) {
        printf("Failed to init PhysFS: ");
        print_physfs_error();
        return 1;
    }

    // Mount the ZIP file
    if (!PHYSFS_mount(argv[1], "/", 0)) {
        printf("Failed to mount %s: ", argv[1]);
        print_physfs_error();
        PHYSFS_deinit();
        return 1;
    }

    printf("Successfully mounted %s\n", argv[1]);
    printf("Listing contents:\n");
    list_files("");

    // Example of reading a specific file
    const char* test_file = "test.txt";  // Change this to a file you know exists in your ZIP
    PHYSFS_file* file = PHYSFS_openRead(test_file);
    if (file) {
        PHYSFS_sint64 file_size = PHYSFS_fileLength(file);
        char* buffer = malloc(file_size + 1);
        if (buffer) {
            PHYSFS_sint64 length_read = PHYSFS_readBytes(file, buffer, file_size);
            if (length_read == file_size) {
                buffer[file_size] = '\0';
                printf("\nContents of %s:\n%s\n", test_file, buffer);
            } else {
                printf("Failed to read file: ");
                print_physfs_error();
            }
            free(buffer);
        }
        PHYSFS_close(file);
    }

    // Cleanup
    PHYSFS_deinit();
    return 0;
}
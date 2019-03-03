#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <ctype.h>
#include <dirent.h>
#include <unistd.h>
#include <string.h>
#include "filelist.h"

#define BUFFER_SIZE 2048


/**
 * Each time when encounter a empty character, a word is counted.
 * @param buffer string buffer for 2048 bytes
 * @return word number in this buffer
 */
int count_word_in_buffer(const char *buffer) {
    int count = 0;
    int i;
    for (i = 0; buffer[i] != EOF && i < BUFFER_SIZE; i++) {
        if (isspace(buffer[i])) count++;
    }
    if (buffer[i] == EOF) count++;
    return count;
}

/**
 * Count words in file
 * @param filename
 * @return the number of words in a file.
 */
int count_word_in_file(char *filename) {
    FILE *fp = fopen(filename, "r");
    int wordsNum = 0;
    char *buf = malloc(sizeof(char) * BUFFER_SIZE);
    while (!feof(fp)) {
     int f_read = (int) fread(buf, sizeof(char), BUFFER_SIZE, fp);
     wordsNum += count_word_in_buffer(buf);
    }
    free(buf);
    fclose(fp);
    return wordsNum;
}

void ls(const char *dirname, const FileList fileList) {
    DIR *dir_ptr;
    struct dirent *direntp;
    if ((dir_ptr = opendir(dirname)) == NULL) {
        fprintf(stderr, "Cannot open %s \n", dirname);
    } else {
        while ((direntp = readdir(dir_ptr)) != NULL)
        {
            if (direntp->d_type == '\b')
                addFileTo(fileList, direntp->d_name);
        }

        close(dir_ptr);
    }
}

char *get_relative_filename(const char *dirname, const char *filename) {
    char *newname = malloc(sizeof(char) * (strlen(dirname) + strlen(filename) + 1));
    strcpy(newname, dirname);
    strcat(newname, "/");
    strcat(newname, filename);
    return newname;
}

int main(int argc, char** argv) {
    if (argc < 2) {
        printf("No directory inputed.");
        return 0;
    }
    const char *dirname = argv[1];
    FileList fileList = initFileList();
    ls(dirname, fileList);
    FileListNode node = fileList;
    /*
    while (node != NULL) {
        printf("%s\n", node->fname);
        char *filename = get_relative_filename(dirname, node->fname);
        printf("%d\n", count_word_in_file(filename));
        free(filename);
        node = nextFileListNode(node);
    }
    */
    char *fname = NULL;
    #pragma omp parallel shared(node) private(fname)
    {
        #pragma omp critical
        {
            node = nextFileListNode(node);
            if (node != NULL)
                fname = node->fname;
        }
        if (fname != NULL) {
            char *filename = get_relative_filename(dirname, fname);
            int count = count_word_in_file(filename);
            free(filename);
            printf("%d: %s counts %d words.\n", omp_get_thread_num(), fname, count);
        }
    }

    destroyFileList(fileList);
    return 0;
}

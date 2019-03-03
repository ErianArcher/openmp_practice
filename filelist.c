//
// Created by Administrator on 2019年3月3日.
//
#include "filelist.h"
#include <stdlib.h>
#include <string.h>

FileList initFileList() {
    return malloc(sizeof(FileList));
}

void destroyFileList(FileList fileList) {
    if (fileList == NULL) return ;
    FileListNode node = fileList, nextnode = NULL;
    while ((nextnode = nextFileListNode(node)) != NULL) {
        free(node);
        node = nextnode;
    }
}

void addFileTo(FileList fileList, char *fname) {
    int fname_len = strlen(fname);
    FileListNode node = malloc(sizeof(FileListNode));
    node->fname = malloc(sizeof(char) * fname_len);
    memcpy(node->fname, fname, fname_len);
    node->next = NULL;
    tail(fileList)->next = node;
}
FileListNode nextFileListNode(FileListNode node) {
    return node->next;
}

FileListNode tail(FileList fileList) {
    FileListNode fileListNode = fileList;
    while (nextFileListNode(fileListNode) != NULL) {
        fileListNode = nextFileListNode(fileListNode);
    }
    return fileListNode;
}


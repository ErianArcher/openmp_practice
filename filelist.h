//
// Created by Administrator on 2019年3月3日.
//

#ifndef DEMO_LIST_H
#define DEMO_LIST_H
typedef struct filelist {
    char *fname;
    struct filelist *next;
}* FileListNode;
typedef struct filelist *FileList;

FileList initFileList();
void destroyFileList(FileList fileList);
void addFileTo(FileList fileList, char *fname);
FileListNode nextFileListNode(FileListNode node);
FileListNode tail(FileList fileList);
#endif //DEMO_LIST_H

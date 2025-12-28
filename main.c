#include <stdio.h>
#include <string.h>
#include "id3v1.h"

int main(int argc, char const *argv[]){
    
    const char *file_path = "Bôa - 1998 - Duvet.mp3";


    printf("--- Tag Init ---\n"); // empty

    id3v1_tag tag = id3v1_tag_new();
    id3v1_tag *tag_ptr = &tag;
    
    id3v1_print(tag_ptr);


    printf("\n--- Read File Tag ---\n"); // empty
    id3v1_read(file_path, tag_ptr);
    id3v1_print(tag_ptr);


    printf("\n--- Write File Tag 1 ---\n"); // new_tag
    id3v1_tag new_tag = id3v1_tag_new();
    id3v1_tag *new_tag_ptr = &new_tag;

    id3v1_set_title(new_tag_ptr, "Duvet");
    id3v1_set_album(new_tag_ptr, "Test Album");
    id3v1_set_artist(new_tag_ptr, "Boa");
    id3v1_set_year(new_tag_ptr, "1998");

    id3v1_write(file_path, new_tag_ptr);
    id3v1_read(file_path, tag_ptr);
    id3v1_print(tag_ptr);


    printf("\n--- Delete File Tag ---\n"); // tag was deleted
    id3v1_delete(file_path);
    if(id3v1_exists(file_path) == 0){
        printf("Tag wasn't deleted.\n");
    }else if(id3v1_exists(file_path) == 1){
        printf("Tag was deleted.\n");
    }

    return 0;
} 
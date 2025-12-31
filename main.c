#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include "id3v1.h"



void print_help(void);
int find_match(int argc, char *argv[], char *match[]);



int main(int argc, char *argv[]){
    if(argc < 2){
        print_help();
        return 0;
    }

    int help_flag = 0;
    int delete_flag = 0;
    int new_tag_flag = 0;

    char *help[] = {"-h", "--help", NULL};
    if(find_match(argc, argv, help)){
        fprintf(stderr, "[DEBUG] Found -h | --help\n");
        help_flag++;
        return 0;
    }

    if(argv[1][0] != '-'){
        fprintf(stderr, "[DEBUG] No option found\n");
        id3v1_tag tag = id3v1_tag_new();
        id3v1_tag *tag_ptr = &tag;

        if(id3v1_read(argv[1], tag_ptr) != 0){
            return -1;
        }

        id3v1_print(tag_ptr);
        return 0;
    }

    fprintf(stderr, "Unknown option: %s\n", argv[1]);
    return 0;
}



void print_help(){
    printf("Usage:\n");
    printf("    id3v1 [options] <file>\n");
    printf("\n");
    printf("Options:\n");
    printf("    -t --title <title>      (Max. 30 bytes)     Set song title\n");
    printf("    -a --artist <artist>    (Max. 30 bytes)     Set song artist\n");
    printf("    -A --album <album>      (Max. 30 bytes)     Set song album\n");
    printf("    -y --year <year>        (Max. 4 bytes)      Set song year\n");
    printf("    -c --comment <comment>  (Max. 30 bytes)     Set song comment\n");
    printf("    -g --genre <genre>      (Range 0-255)       Set song genre\n");
    printf("    -d --delete                                 Delete the tag\n");
    printf("    -h --help                                   Show this help\n");
    printf("\n");
    printf("Notes:\n");
    printf("    The standard only supports ISO-8859-1 encoding.\n");
    printf("    If a field is larger than the max. allowed size it will exit without changes.\n");
    printf("\n");
    printf("Examples:\n");
    printf("    id3v1 --title 'Inner Light' --artist 'Shocking Lemon' --year '2000' 'Inner Light.mp3'\n");
    printf("    id3v1 -t 'Inner Light' -a 'Shocking Lemon' -y '2000' 'Inner Light.mp3'\n");
    printf("\n");
}



int find_match(int argc, char *argv[], char *match[]){
    if(!match){
        fprintf(stderr, "Match is invalid.");
        return -1;
    }

    for(int i = 1; i < argc; i++){
        for(int j = 0; match[j] != NULL; j++){
            if(strcmp(argv[i], match[j]) == 0) return 1;
        }
    }
    return 0;
}

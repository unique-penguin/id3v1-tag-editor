#include <stdio.h>
#include <string.h>
#include "cli_utils.h"

void print_help(void){
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
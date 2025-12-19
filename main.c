#include <stdio.h>
#include <string.h>
#include "id3v1.h"

int main(int argc, char const *argv[])
{
    id3v1_tag tag;
    id3v1_tag *tag_ptr = &tag;
    id3v1_init(tag_ptr);

    id3v1_print(tag_ptr);

    printf("\n--- Edited ---\n\n");

    char title[] = "Does this line have 31 bytesmm";
    char artist[] = "Joshua";
    char album[] = "Dying of dispair";
    char year[] = "2004";
    char comment[] = "Is this worth it?";
    unsigned char genre = 255;

    id3v1_set_title(tag_ptr, title);
    id3v1_set_artist(tag_ptr, artist);
    id3v1_set_album(tag_ptr, album);
    id3v1_set_year(tag_ptr, year);
    id3v1_set_comment(tag_ptr, comment);
    id3v1_set_genre(tag_ptr, genre);

    id3v1_print(tag_ptr);

    printf("\n");
    printf("Title size: %ld\n", sizeof(tag_ptr->title));
    printf("Artitst size: %ld\n", sizeof(tag_ptr->artist));
    printf("Album size: %ld\n", sizeof(tag_ptr->album));
    printf("Year size: %ld\n", sizeof(tag_ptr->year));
    printf("Comment size: %ld\n", sizeof(tag_ptr->comment));
    printf("Genre size: %ld\n", sizeof(tag_ptr->genre));
    
    return 0;
}

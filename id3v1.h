#ifndef ID3V1_H
#define ID3V1_H

typedef struct{
    char title[30];         // --title      -t
    char artist[30];        // --artist     -a
    char album[30];         // --album      -A
    char year[4];           // --year       -y
    char comment[30];       // --comment    -c
    unsigned char genre;    // --genre      -g
} id3v1_tag;

int id3v1_exists(const char *file_path);

int id3v1_print(const id3v1_tag *tag_ptr);

int id3v1_read(char *file_path, id3v1_tag *tag_ptr);

int id3v1_write(char *file_path, const id3v1_tag *tag_ptr);

int id3v1_delete(char *file_path);

#endif
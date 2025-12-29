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

id3v1_tag id3v1_tag_new(void);
void id3v1_tag_init(id3v1_tag *tag);

int is_file_valid(const char *file_path);
int id3v1_tag_exists(const char *file_path);

const char* id3v1_genre_name(unsigned char genre);

int id3v1_print(const id3v1_tag *tag_ptr);
int id3v1_read(const char *file_path, id3v1_tag *tag_ptr);
int id3v1_write(const char *file_path, const id3v1_tag *tag_ptr);
int id3v1_delete(const char *file_path);

int id3v1_set_title(id3v1_tag *tag_ptr, const char *title);
int id3v1_set_artist(id3v1_tag *tag_ptr, const char *artist);
int id3v1_set_album(id3v1_tag *tag_ptr, const char *album);
int id3v1_set_year(id3v1_tag *tag_ptr, const char *year);
int id3v1_set_comment(id3v1_tag *tag_ptr, const char *comment);
int id3v1_set_genre(id3v1_tag *tag_ptr, const unsigned char genre);

#endif
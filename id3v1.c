#include <stdio.h>
#include <string.h> // for memset
#include <ctype.h>
#include <sys/stat.h>
#include "id3v1.h"

#define ID3V1_TAG_SIZE 128
#define KEYWORD "TAG"

void id3v1_init(id3v1_tag *tag){
    if(!tag) return;
    
    memset(tag->title, 0, sizeof(tag->title));
    memset(tag->artist, 0, sizeof(tag->artist));
    memset(tag->album, 0, sizeof(tag->album));
    memset(tag->year, 0, sizeof(tag->year));
    memset(tag->comment, 0, sizeof(tag->comment));

    tag->genre = 255;
}

int id3v1_file_check(const char *file_path) {
    if (!file_path) return 1;

    // --- Check file extension ---
    size_t len = strlen(file_path);
    if (len < 4) return 2; // too short to be .mp3
    const char *ext = file_path + len - 4;
    if (!(tolower(ext[0]) == '.' && tolower(ext[1]) == 'm' && tolower(ext[2]) == 'p' && tolower(ext[3]) == '3')) {
        return 3; // wrong extension
    }

    // --- Check file existence and size ---
    struct stat st;
    if (stat(file_path, &st) != 0) return 4; // file does not exist
    if (st.st_size < ID3V1_TAG_SIZE) return 5; // too small for ID3v1

    // --- Check read access and tag presence ---
    FILE *file_ptr = fopen(file_path, "rb");
    if (!file_ptr) return 6; // cannot open for reading

    if (fseek(file_ptr, -ID3V1_TAG_SIZE, SEEK_END) != 0) {
        fclose(file_ptr);
        return 7; // cannot seek to tag
    }

    char keyword[3];
    if (fread(keyword, 3, 1, file_ptr) != 1) {
        fclose(file_ptr);
        return 8; // cannot read tag
    }

    fclose(file_ptr);

    // --- Check ID3v1 TAG keyword ---
    if (strncmp(keyword, KEYWORD, 3) != 0) {
        return 9; // no ID3v1 tag found
    }

    return 0; // all checks passed
}

static const char *id3v1_genres[] = {
    "Blues", "Classic Rock", "Country", "Dance", "Disco", "Funk",
    "Grunge", "Hip-Hop", "Jazz", "Metal", "New Age", "Oldies",
    "Other", "Pop", "R&B", "Rap", "Reggae", "Rock", "Techno",
    "Industrial", "Alternative", "Ska", "Death Metal", "Pranks",
    "Soundtrack", "Euro-Techno", "Ambient", "Trip-Hop", "Vocal",
    "Jazz+Funk", "Fusion", "Trance", "Classical", "Instrumental",
    "Acid", "House", "Game", "Sound Clip", "Gospel", "Noise",
    "AlternRock", "Bass", "Soul", "Punk", "Space", "Meditative",
    "Instrumental Pop", "Instrumental Rock", "Ethnic", "Gothic",
    "Darkwave", "Techno-Industrial", "Electronic", "Pop-Folk",
    "Eurodance", "Dream", "Southern Rock", "Comedy", "Cult",
    "Gangsta", "Top 40", "Christian Rap", "Pop/Funk", "Jungle",
    "Native American", "Cabaret", "New Wave", "Psychadelic",
    "Rave", "Showtunes", "Trailer", "Lo-Fi", "Tribal", "Acid Punk",
    "Acid Jazz", "Polka", "Retro", "Musical", "Rock & Roll",
    "Hard Rock", "Folk", "Folk-Rock", "National Folk", "Swing",
    "Fast Fusion", "Bebop", "Latin", "Revival", "Celtic", "Bluegrass",
    "Avantgarde", "Gothic Rock", "Progressive Rock", "Psychedelic Rock",
    "Symphonic Rock", "Slow Rock", "Big Band", "Chorus", "Easy Listening",
    "Acoustic", "Humour", "Speech", "Chanson", "Opera", "Chamber Music",
    "Sonata", "Symphony", "Booty Bass", "Primus", "Porn Groove", "Satire",
    "Slow Jam", "Club", "Tango", "Samba", "Folklore", "Ballad",
    "Power Ballad", "Rhythmic Soul", "Freestyle", "Duet", "Punk Rock",
    "Drum Solo", "A Cappella", "Euro-House", "Dance Hall"
};

const char* id3v1_genre_name(unsigned char genre){
    size_t num_genres = sizeof(id3v1_genres) / sizeof(id3v1_genres[0]);
    if(genre >= num_genres){
        return "Unknown";
    }
    return id3v1_genres[genre];
}

void id3v1_print(const id3v1_tag *tag_ptr){
    printf("Title   : %.30s\n", tag_ptr->title);
    printf("Artist  : %.30s\n", tag_ptr->artist);
    printf("Album   : %.30s\n", tag_ptr->album);
    printf("Year    : %.4s\n", tag_ptr->year);
    printf("Comment : %.30s\n", tag_ptr->comment);
    printf("Genre   : %u (%s)\n", tag_ptr->genre, id3v1_genre_name(tag_ptr->genre));
}

int id3v1_read(char *file_path, id3v1_tag *tag_ptr){
    if(id3v1_file_check(file_path) != 0) return 1;

    FILE *file_ptr = fopen(file_path, "rb");
    if(!file_ptr){
        perror("fopen");
        return 1;
    }

    if(fseek(file_ptr, -125, SEEK_END) != 0){
        perror("fseek");
        return 1;
    }

    if(fread(tag_ptr, sizeof(id3v1_tag), 1, file_ptr) != 1){
        perror("fread");
        return 1;
    }

    return 0;
}

int id3v1_write(char *file_path, const id3v1_tag *tag_ptr){
    if(id3v1_file_check(file_path) != 0) return 1;

    return 0;
}

int id3v1_set_title(id3v1_tag *tag_ptr, const char *title){
    if(!tag_ptr || !title) return 1;

    if(strlen(title) > 30) return 1;

    memset(tag_ptr->title, 0, sizeof(tag_ptr->title));
    memcpy(tag_ptr->title, title, 30);

    return 0;
}

int id3v1_set_artist(id3v1_tag *tag_ptr, const char *artist){
    if(!tag_ptr || !artist) return 1;

    if(strlen(artist) > 30) return 1;

    memset(tag_ptr->artist, 0, sizeof(tag_ptr->artist));
    memcpy(tag_ptr->artist, artist, 30);

    return 0;
}

int id3v1_set_album(id3v1_tag *tag_ptr, const char *album){
    if(!tag_ptr || !album) return 1;

    if(strlen(album) > 30) return 1;

    memset(tag_ptr->album, 0, sizeof(tag_ptr->album));
    memcpy(tag_ptr->album, album, 30);

    return 0;
}

int id3v1_set_year(id3v1_tag *tag_ptr, const char *year){
    if(!tag_ptr || !year) return 1;

    if(strlen(year) > 4) return 1;

    memset(tag_ptr->year, 0, sizeof(tag_ptr->year));
    memcpy(tag_ptr->year, year, 4);

    return 0;
}

int id3v1_set_comment(id3v1_tag *tag_ptr, const char *comment){
    if(!tag_ptr || !comment) return 1;

    if(strlen(comment) > 30) return 1;

    memset(tag_ptr->comment, 0, sizeof(tag_ptr->comment));
    memcpy(tag_ptr->comment, comment, 30);

    return 0;
}

int id3v1_set_genre(id3v1_tag *tag_ptr, const unsigned char genre){
    if(!tag_ptr || !genre) return 1;

    tag_ptr->genre = genre;

    return 0;
}
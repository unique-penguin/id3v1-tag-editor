#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "id3v1.h"



#define ID3V1_TAG_SIZE 128
#define KEYWORD "TAG"
#define VALID_FILE_EXTENSION ".mp3"
#define TEMP_FILE_EXTENSION ".tmp"

#define FILE_VALID 1
#define FILE_INVALID 0
#define FILE_ERROR -1



id3v1_tag id3v1_tag_new(void){
    id3v1_tag tag;

    memset(&tag, 0, sizeof(tag));
    tag.genre = 255;

    return tag;
}



void id3v1_tag_init(id3v1_tag *tag){
    if(!tag) return;

    *tag = id3v1_tag_new();
}



int is_file_valid(const char *file_path){
    if(!file_path){
        fprintf(
            stderr,
            "Invalid argument: file_path is NULL\n"
        );
        return FILE_ERROR;
    }

    size_t file_path_len = strlen(file_path);
    if(file_path_len < 4){
        fprintf(
            stderr, 
            "Invalid file '%s': expected file path size >=4, got %zu\n",
            file_path,
            file_path_len
        );
        return FILE_INVALID;
    }

    const char *file_extension = file_path + file_path_len - strlen(VALID_FILE_EXTENSION);
    if(strcmp(file_extension, VALID_FILE_EXTENSION) != 0){
        fprintf(
            stderr, 
            "Invalid file '%s': expected extension %s, got %s\n",
            file_path,
            VALID_FILE_EXTENSION,
            file_extension
        );
        return FILE_INVALID;
    }

    FILE *file_ptr = fopen(file_path, "rb");
    if(!file_ptr){
        int errornum = errno;
        fprintf(
            stderr, 
            "Cannot open file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
        return FILE_ERROR;
    } 

    if(fseek(file_ptr, 0, SEEK_END) != 0){
        fprintf(
            stderr, 
            "Cannot seek file '%s': %s\n",
            file_path,
            strerror(errno)
        );
        fclose(file_ptr);
        return FILE_ERROR;
    }
    
    long file_size = ftell(file_ptr);
    if(file_size == -1L){
        int errornum = errno;
        fprintf(
            stderr, 
            "Cannot tell size of file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
        fclose(file_ptr);
        return FILE_ERROR;
    }

    if(fclose(file_ptr) != 0){
        int errornum = errno;
        fprintf(
            stderr,
            "Warning: Cannot close file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
    }

    if((size_t)file_size < ID3V1_TAG_SIZE){
        fprintf(
            stderr,
            "Invalid file '%s': expected size >=%d, got %ld",
            file_path,
            ID3V1_TAG_SIZE,
            file_size
        );
        return FILE_INVALID;
    }

    return FILE_VALID;
}



int id3v1_tag_exists(const char *file_path){
    if(!file_path){
        fprintf(
            stderr,
            "Invalid argument: file_path is NULL\n"
        );
        return -1;
    }

    FILE *file_ptr = fopen(file_path, "rb");
    if(!file_ptr){
        int errornum = errno;
        fprintf(
            stderr, 
            "Cannot open file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
        return -1;
    }

    if(fseek(file_ptr, -128, SEEK_END) != 0){
        int errornum = errno;
        fprintf(
            stderr, 
            "Cannot seek file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
        fclose(file_ptr);
        return -1;
    }

    char keyword[3];

    if(fread(keyword, 3, 1, file_ptr) != 1){
        int errornum = errno;
        fprintf(
            stderr, 
            "Cannot read file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
        fclose(file_ptr);
        return -1;
    }

    if(fclose(file_ptr) != 0){
        int errornum = errno;
        fprintf(
            stderr,
            "Warning: Cannot close file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
    }

    if(strncmp(keyword, KEYWORD, 3) != 0){
        fprintf(stderr,
            "[DEBUG] File doens't have a ID3v1 Tag: %.3s != %s",
            keyword,
            KEYWORD
        );
        return 1;
    }

    return 0;
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



int id3v1_print(const id3v1_tag *tag_ptr){
    printf("Title   : %.30s\n", tag_ptr->title);
    printf("Artist  : %.30s\n", tag_ptr->artist);
    printf("Album   : %.30s\n", tag_ptr->album);
    printf("Year    : %.4s\n", tag_ptr->year);
    printf("Comment : %.30s\n", tag_ptr->comment);
    printf("Genre   : %u (%s)\n", tag_ptr->genre, id3v1_genre_name(tag_ptr->genre));

    return 0;
}



int id3v1_read(const char *file_path, id3v1_tag *tag_ptr){
    if(!file_path){
        fprintf(
            stderr,
            "Invalid argument: file_path is NULL\n"
        );
        return -1;
    };

    FILE *file_ptr = fopen(file_path, "rb");
    if(!file_ptr){
       int errornum = errno;
        fprintf(
            stderr, 
            "Cannot open file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
        return -1;
    }

    if(fseek(file_ptr, -125, SEEK_END) != 0){
        int errornum = errno;
        fprintf(
            stderr, 
            "Cannot seek file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
        fclose(file_ptr);
        return -1;
    }

    if(fread(tag_ptr, sizeof(id3v1_tag), 1, file_ptr) != 1){
        int errornum = errno;
        fprintf(
            stderr, 
            "Cannot read file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
        fclose(file_ptr);
        return -1;
    }

    if(fclose(file_ptr) != 0){
        int errornum = errno;
        fprintf(
            stderr,
            "Warning: Cannot close file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
    }

    return 0;
}



int id3v1_write(const char *file_path, const id3v1_tag *tag_ptr){
    if(!file_path){
        fprintf(
            stderr,
            "Invalid argument: file_path is NULL\n"
        );
        return -1;
    };
    if(!file_path){
        fprintf(
            stderr,
            "Invalid argument: tag_ptr is NULL\n"
        );
        return -1;
    };

    int exists = id3v1_tag_exists(file_path);
    
    if(exists == 0){
        
        FILE *file_ptr = fopen(file_path, "ab");
        if(!file_ptr){
            int errornum = errno;
            fprintf(
                stderr, 
                "Cannot open file '%s': %s\n",
                file_path,
                strerror(errornum)
            );
            return -1;
        }

        char buffer[ID3V1_TAG_SIZE];
        memcpy(buffer, KEYWORD, 3);
        memcpy(buffer + 3, tag_ptr, ID3V1_TAG_SIZE - 3);
        
        if(fwrite(buffer, 1, ID3V1_TAG_SIZE, file_ptr) != ID3V1_TAG_SIZE){
            int errornum = errno;
            fprintf(
                stderr, 
                "Cannot write to file '%s': %s\n",
                file_path,
                strerror(errornum)
            );
            return -1;
        }

        if(fclose(file_ptr) != 0){
            int errornum = errno;
            fprintf(
                stderr,
                "Cannot close file '%s': %s\n",
                file_path,
                strerror(errornum)
            );
            return -1;
        }

        return 0;
    }

    if(exists == 1){
        FILE *file_ptr = fopen(file_path, "r+b");
        if(!file_ptr){
            int errornum = errno;
            fprintf(
                stderr, 
                "Cannot open file '%s': %s\n",
                file_path,
                strerror(errornum)
            );
            return -1;
        }
        
        if(fseek(file_ptr, -ID3V1_TAG_SIZE, SEEK_END) != 0){
            int errornum = errno;
            fprintf(
                stderr, 
                "Cannot seek file '%s': %s\n",
                file_path,
                strerror(errornum)
            );
            fclose(file_ptr);
            return -1;
        }

        char buffer[ID3V1_TAG_SIZE];
        memcpy(buffer, KEYWORD, 3);
        memcpy(buffer + 3, tag_ptr, ID3V1_TAG_SIZE - 3);
        
        if(fwrite(buffer, 1, ID3V1_TAG_SIZE, file_ptr) != ID3V1_TAG_SIZE){
            int errornum = errno;
            fprintf(
                stderr, 
                "Cannot write to file '%s': %s\n",
                file_path,
                strerror(errornum)
            );
            return -1;
        }

        return 0;
    }

    return -1;
}


int get_file_size(const char *file_path, size_t *file_size_ptr){
    FILE *file_ptr = fopen(file_path, "rb");
    if(!file_ptr){
       int errornum = errno;
        fprintf(
            stderr, 
            "Cannot open file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
        return -1;
    }

    if(fseek(file_ptr, 0, SEEK_END) != 0){
       int errornum = errno;
        fprintf(
            stderr, 
            "Cannot seek file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
        fclose(file_ptr);
        return -1;
    }

    long int file_size = ftell(file_ptr);
    if(file_size == -1L){
        int errornum = errno;
        fprintf(
            stderr,
            "Cannot tell size of file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
        fclose(file_ptr);
        return -1;
    }

    if(fclose(file_ptr) != 0){
        int errornum = errno;
        fprintf(
            stderr,
            "Warning: Cannot close file '%s': %s\n",
            file_path,
            strerror(errornum)
        );
    }

    *file_size_ptr = (size_t)file_size;

    return 0;
}



int id3v1_delete(const char *file_path){
    if(!file_path){
        fprintf(
            stderr,
            "Invalid argument: file_path is NULL\n"
        );
        return -1;
    };

    int exists = id3v1_tag_exists(file_path);
    
    if(exists != 1){
        return -1;
    }

    size_t bytes_to_copy;
    get_file_size(file_path, &bytes_to_copy);

    char temp_file_path[strlen(file_path) + strlen(TEMP_FILE_EXTENSION)];
    strcpy(temp_file_path, file_path);
    strcat(temp_file_path, TEMP_FILE_EXTENSION);

    FILE *file_source_ptr = fopen(file_path, "rb");
    if(!file_source_ptr) return 1;
    FILE *file_result_ptr = fopen(temp_file_path, "wb");
    if(!file_result_ptr) return 1;

    int c;
    while(bytes_to_copy > 0 && (c = fgetc(file_source_ptr)) != EOF){
        fputc(c, file_result_ptr);
        bytes_to_copy--;
    }

    fclose(file_source_ptr);
    fclose(file_result_ptr);

    rename(temp_file_path, file_path);

    return 0;
}



int id3v1_set_title(id3v1_tag *tag_ptr, const char *title){
    if(!tag_ptr || !title) return 1;

    if(strlen(title) > 30) return 1;

    memset(tag_ptr->title, 0, sizeof(tag_ptr->title));
    memcpy(tag_ptr->title, title, strlen(title));

    return 0;
}



int id3v1_set_artist(id3v1_tag *tag_ptr, const char *artist){
    if(!tag_ptr || !artist) return 1;

    if(strlen(artist) > 30) return 1;

    memset(tag_ptr->artist, 0, sizeof(tag_ptr->artist));
    memcpy(tag_ptr->artist, artist, strlen(artist));

    return 0;
}



int id3v1_set_album(id3v1_tag *tag_ptr, const char *album){
    if(!tag_ptr || !album) return 1;

    if(strlen(album) > 30) return 1;

    memset(tag_ptr->album, 0, sizeof(tag_ptr->album));
    memcpy(tag_ptr->album, album, strlen(album));

    return 0;
}



int id3v1_set_year(id3v1_tag *tag_ptr, const char *year){
    if(!tag_ptr || !year) return 1;

    if(strlen(year) > 4) return 1;

    memset(tag_ptr->year, 0, sizeof(tag_ptr->year));
    memcpy(tag_ptr->year, year, strlen(year));

    return 0;
}



int id3v1_set_comment(id3v1_tag *tag_ptr, const char *comment){
    if(!tag_ptr || !comment) return 1;

    if(strlen(comment) > 30) return 1;

    memset(tag_ptr->comment, 0, sizeof(tag_ptr->comment));
    memcpy(tag_ptr->comment, comment, strlen(comment));

    return 0;
}



int id3v1_set_genre(id3v1_tag *tag_ptr, const unsigned char genre){
    if(!tag_ptr || !genre) return 1;

    tag_ptr->genre = genre;

    return 0;
}
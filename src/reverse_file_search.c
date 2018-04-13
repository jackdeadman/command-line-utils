#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Files
{
    char* basename;
    char* filename;
} File;

void file_move_up_dir(File* f)
{
    char ch = '.';
    int index = strlen(f->basename);
    while (ch != '/')
    {
        -- index;
        ch = f->basename[index];
    }

    f->basename[index] = '\0';
}

char* file_fullname(File* f)
{
    int base_len= strlen(f->basename);
    int file_len = strlen(f->filename);

    int size = base_len + file_len + 1;
    char* path = malloc(size);

    strcpy(path, f->basename);
    path[base_len] = '/';
    strcpy(path + base_len + 1, f->filename);
    return path;
}

int file_exists(File* f)
{
    char* fullname = file_fullname(f);
    int exists = 0 == access(fullname, 0);
    free(fullname);

    return exists;
}

int file_reverse_search(File* f)
{
    if (file_exists(f))
    {
        return 1;
    }

    if (*f->basename == '\0')
    {
        return 0;
    }

    file_move_up_dir(f);
    return file_reverse_search(f);
}

void print_help()
{
    printf("\nSearches if a file exists in a directory or one of its parents.\n\n");
    printf("Usage: reverse_file_search [FILE]\n");
}

int main(int argc, char *argv[])
{
    int not_enough_args = argc != 2;
    int called_helped = (strcmp(argv[1], "--help") == 0)
                            || (strcmp(argv[1], "-h") == 0);

    if (not_enough_args || called_helped)
    {
        print_help();
        return 1;
    }

    char cwd[1024];
    File* f;
    f->basename = getcwd(cwd, sizeof(cwd));
    f->filename = argv[1];

    int result = file_reverse_search(f);
    printf("%s\n", result ? "yes" : "no");
    return 0;
}

/* TotalJustice 2019 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>

#if defined _MSC_VER
    #include <direct.h>

#elif defined __GNUC__
    #include <unistd.h>
    #include <sys/stat.h>
    #include <sys/types.h>
#endif


size_t get_file_size_from_path(const char *file)
{
    size_t size = 0;
    FILE *f = fopen(file, "r");
    if (!f)
        return size;
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fclose(f);
    return size;
}

size_t get_file_size_from_file(FILE *file)
{
    size_t size = 0;
    if (!file)
        return size;
    uint64_t prev_seek = ftell(file);
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    fseek(file, prev_seek, SEEK_SET);
    return size;
}

bool write_to_file_cur(const void *buf, size_t size, FILE *file)
{
    if (!buf || !size || !file)
        return false;
    fwrite(buf, size, 1, file);
    return true;
}

bool write_to_file_offset(const void *buf, size_t size, uint64_t offset, FILE *file)
{
    if (!buf || !file)
        return false;
    fseek(file, offset, SEEK_SET);
    fwrite(buf, size, 1, file);
    return true;
}

bool write_to_file_end(const void *buf, size_t size, FILE *file)
{
    if (!buf || !file)
        return false;
    fseek(file, 0, SEEK_END);
    fwrite(buf, size, 1, file);
    return true;
}

bool write_to_file_start(const void *buf, size_t size, FILE *file)
{
    if (!buf || !file)
        return false;
    fseek(file, 0, SEEK_SET);
    fwrite(buf, size, 1, file);
    return true;
}

bool write_to_file_chunk(FILE *src, FILE *dest, uint64_t offset, size_t chunk_size, size_t size)
{
    if (!src || !dest || !chunk_size || !size)
        return false;
    
    for (uint64_t w_offset = 0; w_offset < size; w_offset += chunk_size)
    {
        if (w_offset + chunk_size > size)
            chunk_size = size - w_offset;
        
        void *buf = malloc(chunk_size);
        memset(buf, 0, chunk_size);

        fread(buf, chunk_size, 1, src);
        fwrite(buf, chunk_size, 1, dest);
        free(buf);
    }
    return true;
}

const char *get_cwd(char *buf, size_t size)
{
    return getcwd(buf, size);
}

FILE *open_file(const char *file, const char *mode)
{
    FILE *f = fopen(file, mode);
    if (f)
        return f;
    return NULL;
}

DIR *open_dir(const char *directory)
{
    DIR *dir = opendir(directory);
    if (dir)
        return dir;
    return NULL;
}

void read_file(void *out, size_t size, uint64_t offset, FILE *f)
{
    fseek(f, offset, SEEK_SET);
    fread(out, size, 1, f);
}

bool is_dir(const char *folder_to_check)
{
    DIR *dir = opendir(folder_to_check);
    if (!dir)
        return false;
    closedir(dir);
    return true;
}

bool check_if_file_exists(const char *file)
{
    FILE *f = fopen(file, "r");
    if (!f)
        return false;
    fclose(f);
    return true;
}

bool check_if_dir_exists(const char *directory)
{
    DIR *dir = opendir(directory);
    if (!dir)
        return false;
    closedir(dir);
    return true;
}

bool create_dir(const char *dir)
{
    if (check_if_dir_exists(dir))
        return true;
    int res = 0;
    #if defined _MSC_VER
        res = _mkdir(dir);
    #elif defined __GNUC__
        res = mkdir(dir, 0777);
    #endif
    if (res == 0)
        return true;
    return false;
}

bool change_dir(const char *path)
{
    if (!check_if_dir_exists(path))
        create_dir(path);
    int res = 0;
    #if defined _MSC_VER
        res = _chdir(path);
    #elif defined __GNUC__
        res = chdir(path);
    #endif
    if (res == 0)
        return true;
    return false;
}

size_t scan_dir(const char *directory)
{
    size_t num = 0;
    struct dirent *de;
    DIR *dir = open_dir(directory);
    if (!dir)
        return num;

    while ((de = readdir(dir)))
    {
        if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            continue;
        num++;
    }

    closedir(dir);
    return num;
}

size_t scan_dir_recursive(const char *directory)
{
    size_t num = 0;
    struct dirent *de;
    DIR *dir = open_dir(directory);
    if (!dir)
        return num;
        
    while ((de = readdir(dir)))
    {
        if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            continue;

        char full_path[512];
        snprintf(full_path, 512, "%s/%s", directory, de->d_name);

        if (is_dir(full_path))
            scan_dir_recursive(full_path);

        num++;
    }

    closedir(dir);
    return num;
}

size_t get_folder_size(const char *directory)
{
    size_t size = 0;
    struct dirent *de;
    DIR *dir = open_dir(directory);
    if (!dir)
        return size;

    while ((de = readdir(dir)))
    {
        if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            continue;

        char full_path[512];
        snprintf(full_path, 512, "%s/%s", directory, de->d_name);

        if (is_dir(full_path))
            size += get_folder_size(full_path);
        else
            size += get_file_size_from_path(full_path);
    }
    return size;
}

bool create_file(const char *file)
{
    if (check_if_file_exists(file))
        return true;
    FILE *f = fopen(file, "wb");
    if (!f)
        return false;
    fclose (f);
    return true;
}

bool delete_file(const char *file)
{
    if (!check_if_file_exists(file))
        return true;
    int res = remove(file);
    if (res == 0)
        return true;
    return false;
}

void delete_dir(const char *directory)
{
    struct dirent *de;
    DIR *dir = open_dir(directory);
    if (!dir)
        return;

    while ((de = readdir(dir)))
    {
        if (!strcmp(de->d_name, ".") || !strcmp(de->d_name, ".."))
            continue;

        char full_path[512];
        snprintf(full_path, 512, "%s/%s", directory, de->d_name);

        if (is_dir(full_path))
            delete_dir(full_path);
        else
            delete_file(full_path);
    }
    closedir(dir);
    rmdir(directory);
}

void copy_file(const char *src, char *dest)
{
    FILE *srcfile = fopen(src, "rb");
    FILE *newfile = fopen(dest, "wb");

    if (srcfile && newfile)
    {
        void *buf = malloc(0x800000);
        size_t bytes;           // size of the file to write (4MB or filesize max)

        while (0 < (bytes = fread(buf, 1, 0x800000, srcfile)))
            fwrite(buf, bytes, 1, newfile);
        free (buf);
    }
    fclose(srcfile);
    fclose(newfile);
}

bool copy_folder(const char *src, const char *dest)
{
    if (!create_dir(dest))
        return false;
    struct dirent *de;
    DIR *dir = open_dir(src);
    if (!dir)
        return false;

    while ((de = readdir(dir)))
    {
        char full_path[512];
        snprintf(full_path, 512, "%s/%s", dest, de->d_name);

        // check if the file is a directory.
        if (is_dir(de->d_name))
            create_dir(full_path);
        else
            copy_file(de->d_name, full_path);
    }
    closedir(dir);
    return true;
}

void move_file(const char *src, const char *dest)
{
    rename(src, dest);
}

bool move_folder(const char *src, char *dest)
{
    if (!create_dir(dest))
        return false;
    struct dirent *de;
    DIR *dir = open_dir(src);
    if (!dir)
        return false;

    while ((de = readdir(dir)))
    {
        char full_path[512];
        snprintf(full_path, 512, "%s/%s", dest, de->d_name);

        // check if the file is a directory.
        if (is_dir(de->d_name))
            create_dir(full_path);
        else
            move_file(de->d_name, full_path);
    }
    closedir(dir);
    return true;
}
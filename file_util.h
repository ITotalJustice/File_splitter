#ifndef _FILE_UTIL_H_
#define _FILE_UTIL_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <dirent.h>


//
size_t get_file_size_from_path(const char *file);

//
size_t get_file_size_from_file(FILE *file);

//
bool write_to_file_cur(const char *buf, size_t size, FILE *file);

//
bool write_to_file_offset(const char *buf, size_t size, uint64_t offset, FILE *file);

//
bool write_to_file_end(const char *buf, size_t size, FILE *file);

//
bool write_to_file_start(const char *buf, size_t size, FILE *file);

//
bool write_to_file_chunk(FILE *src, FILE *dest, uint64_t offset, size_t chunk_size, size_t size);

//
const char *get_cwd(char *buf, size_t size);

//
FILE *open_file(const char *file, const char *mode);

//
DIR *open_dir(const char *directory);

//
void read_file(void *out, size_t size, uint64_t offset, FILE *f);

//
bool is_dir(const char *folder_to_check);

//
bool check_if_file_exists(const char *file);

//
bool check_if_dir_exists(const char *directory);

//
bool create_dir(const char *dir);

//
bool change_dir(const char *path);

//
size_t scan_dir(const char *directory);

//
size_t scan_dir_recursive(const char *directory);

//
size_t get_folder_size(const char *directory);

//
bool create_file(const char *file);

//
bool delete_file(const char *file);

//
void delete_dir(const char *directory);

//
void copy_file(const char *src, char *dest);

//
bool copy_folder(const char *src, const char *dest);

//
void move_file(const char *src, const char *dest);

//
bool move_folder(const char *src, char *dest);

//




#endif

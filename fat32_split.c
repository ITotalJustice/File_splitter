/* TotalJustice 2019 */

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "file_util.h"
#include "string_util.h"


#define SPLIT_FILE_SIZE     0xFFFF0000  // 4,294,901,760 bytes
#define BUFFER_SIZE         0x800000    // 8 Mib
#define OUT_DIR             "out"


bool split_file(FILE *f, size_t f_size)
{
    for (uint64_t i = 0, offset = 0, split_size = SPLIT_FILE_SIZE; offset < f_size; i++, offset += split_size)
    {
        char new_f_name[3];
        if (i < 10)
            snprintf(new_f_name, 3, "0%ld", i);
        else
            snprintf(new_f_name, 3, "%ld", i);

        FILE *new_f = fopen(new_f_name, "wb");
        if (!new_f)
            return false;
        
        if (offset + split_size > f_size)
            split_size = f_size - offset;

        printf("writing to file %s - size %lu\n", new_f_name, split_size);
        bool res = write_to_file_chunk(f, new_f, offset, BUFFER_SIZE, split_size);
        fclose(new_f);

        if (!res)
            return false;
    }
    return true;
}

bool single_file_mode(const char *file)
{
    FILE *f = open_file(file, "rb");
    if (!f)
    {
        printf("failed to open file\n");
        return false;
    }

    size_t f_size = get_file_size_from_file(f);
    if (f_size <= SPLIT_FILE_SIZE)
    {
        fclose(f);
        printf("file size is %lu\n\nthis does not need to be split...\n", f_size);
        return false;
    }

    if (!create_dir(OUT_DIR))
    {
        fclose(f);
        printf("failed to create dir %s\n", OUT_DIR);
        return false;
    }

    const char *ptr = get_file_name_from_full_path(file);
    char dir_name[1024];
    snprintf(dir_name, 1024, "%s/%s", OUT_DIR, ptr);

    if (!create_dir(dir_name))
    {
        fclose(f);
        printf("failed to create dir %s\n", dir_name);
        return false;
    }

    if (!change_dir(dir_name))
    {
        fclose(f);
        printf("failed to change dir %s\n", dir_name);
        return false;
    }

    if (!split_file(f, f_size))
    {
        fclose(f);
        printf("failed to split file\n");
        return false;
    }
    
    fclose(f);
    return true;
}

bool multi_file_mode(char **files, int total)
{
    char pwd[1024];
    get_cwd(pwd, 1024);

    for (int i = 1; i < total; i++)
    {
        printf("trying to open file %s\n", files[i]);
        if (!single_file_mode(files[i]))
            return false;
        if (!change_dir(pwd))
        {
            printf("failed to change to dir %s\n", pwd);
            return false;
        }
    }
    return true;
}

bool ui_mode()
{
    printf("not implemented yet\nplease drag & drop the files to split onto the .exe");
    return true;
    /*printf("drag and drop the file to split OR enter the path of the file...\n");
    char str_input[1024];
    fgets(str_input, 1024, stdin);
    str_input[strlen (str_input) - 1] = '\0';
    printf("\nyou entered %s\n", str_input);

    return single_file_mode(str_input);*/
}

int main(int argc, char *argv[])
{
    switch (argc)
    {
        case 1:     return ui_mode();
        case 2:     return single_file_mode(argv[1]);
        default:    return multi_file_mode(argv, argc); 
    }
}
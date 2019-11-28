/* TotalJustice 2019 */

#include <string.h>


const char *get_file_name_ext(const char *file)
{
    const char *dot = strrchr(file, '.');
    if (!dot || dot == file)
        return "";
    return dot + 1;
}

const char *get_file_name_from_full_path(const char *path)
{
    // for linux.
    char c = '/';
    const char *ptr = strrchr(path, c);
    if (ptr && ptr != path)
        return ptr + 1;
    
    // for windows.
    c = '\\';
    ptr = strrchr(path, c);
    if (ptr && ptr != path)
        return ptr + 1;
    return "";
}
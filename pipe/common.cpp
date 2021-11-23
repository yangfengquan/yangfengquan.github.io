#include <stdio.h>
#include "common.h"

int reader(const char* path, struct CTYW* ctyw, int i)
{
    FILE *fp = fopen(path, "rb");
    if(!fp)
        return -1;
    fseek(fp, sizeof(*ctyw) * (i - 1), SEEK_SET);
    fread(ctyw, sizeof(*ctyw), 1, fp);
    fclose(fp);
    return 0;
}

int reader(const char* path, struct Insul* insul, int i)
{
    FILE *fp = fopen(path, "rb");
    if(!fp)
        return -1;
    fseek(fp, sizeof(*insul) * (i - 1), SEEK_SET);
    fread(insul, sizeof(*insul), 1, fp);
    fclose(fp);
    return 0;
}

int reader(const char* path, struct Material* material, int i)
{
    FILE *fp = fopen(path, "rb");
    if(!fp)
        return -1;
    fseek(fp, sizeof(*material) * (i - 1), SEEK_SET);
    fread(material, sizeof(*material), 1, fp);
    fclose(fp);
    return 0;
}


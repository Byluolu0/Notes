#include <stdio.h>

int main()
{
    FILE *in, *out;
    struct pirate
    {
        char name[100];
        unsigned long booty;
        unsigned int beard_len;
    } p, blackbeard = {"Edward Teach", 950, 48};

    out = fopen("data", "w");
    if (!out)
    {
        printf("fopen error\n");
        return 1;
    }

    if (!fwrite(&blackbeard, sizeof(struct pirate), 1, out)) {
        printf("fwrite error\n");
        return 1;
    }

    if (fclose(out)) {
        printf("fclose error\n");
        return 1;
    }

    in = fopen("data", "r");
    if (!in) {
        printf("fopen error\n");
        return 1;
    }

    if (!fread(&p, sizeof(struct pirate), 1, in)) {
        printf("fread error\n");
        return 1;
    }

    if (fclose(in)) {
        printf("close error\n");
        return 1;
    }

    printf("name = %s, booty = %lu, beard_len = %u\n", p.name, p.booty, p.beard_len);

    return 0;
}

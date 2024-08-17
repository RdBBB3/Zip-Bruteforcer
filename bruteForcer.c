#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zip.h>
#include <unistd.h>

void crackZip(const char * zipPath, const char * wordlistPath)
{
    zip_file_t *file;
    const char *zipName;
    int i = 0;
    char buffer[256];
    int errors;
    zip_t *zipArch = zip_open(zipPath, 0, &errors);

    if (zipArch == NULL)
    {
        printf("Le fichier ZIP n'a pas été trouvé\n");
        return;
    }

    zip_int64_t num_files = zip_get_num_entries(zipArch, 0);
    zipName = zip_get_name(zipArch, 0, 0);
    FILE *wordlist = fopen(wordlistPath, "r");
    if (wordlist == NULL) {
        printf("Erreur lors de l'ouverture de la wordlist\n");
        return;
    }
    while (fgets(buffer, sizeof(buffer), wordlist) != NULL)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        i++;
        file = zip_fopen_encrypted(zipArch, zipName, 0, buffer);
        
        if (file != NULL)
        {
            printf("\n");
            printf("Le mot de passe du fichier zip est : %s\n", buffer);
            fclose(wordlist);
            zip_close(zipArch);
            return;
        }
        printf("\rTry %d", i);
        fflush(stdout);
    }
    printf("\nNo password found for %s\n", zipPath);
    fclose(wordlist);
    zip_close(zipArch);
}


int main(int argc, char* argv[])
{

    if (argc < 3)
    {
        printf("Usage : %s <path_to_zip> <path_to_wordlist>\n", argv[0]);
        return -1;
    }
    const char *zipPath = argv[1];
    const char *wordlist = argv[2];

    crackZip(zipPath, wordlist);

    return 0;
}

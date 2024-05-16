DIR *dir;
            struct dirent *ent;
            if ((dir = opendir (diretorio)) != NULL) {
                while ((ent = readdir (dir)) != NULL) {
                    printf ("%s\n", ent->d_name);
                }
                closedir (dir);
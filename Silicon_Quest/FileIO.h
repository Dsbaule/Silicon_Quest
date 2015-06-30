void saveMap(ALLEGRO_DISPLAY *display, struct mapValues curMap)
{
    //==============================================
    //PROJECT VARIABLES
    //==============================================
    FILE *fp;
    ALLEGRO_FILECHOOSER *file;

    //==============================================
    //AUXILIAR VARIABLES
    //==============================================
    int j, i;

    file = al_create_native_file_dialog("", "Choose File location and name", "*.txt",ALLEGRO_FILECHOOSER_MULTIPLE);
    al_show_native_file_dialog(display, file);
    char mapNameTxt[100] = "";

    if(al_get_native_file_dialog_count(file) != 0)
    {
        const char *mapName = al_get_native_file_dialog_path(file, 0);

        strcpy(mapNameTxt, mapName);

        if((mapNameTxt[strlen(mapNameTxt)-1] != 't')||(mapNameTxt[strlen(mapNameTxt)-2] != 'x')||(mapNameTxt[strlen(mapNameTxt)-3] != 't')||(mapNameTxt[strlen(mapNameTxt)-4] != '.'))
            strcat(mapNameTxt, ".txt");

        // SAVE MAP TO FILE
        fp = fopen(mapNameTxt, "w");
        fprintf(fp, "%d %d\n", curMap.numLinhas, curMap.numColunas);
        for(i = 0; i < curMap.numLinhas; i++)
        {
            for(j = 0; j < curMap.numColunas; j++)
                fprintf(fp, "%d ", curMap.blocos[i][j]);
            fprintf(fp, "\n");
        }
        fclose(fp);
    }
}

void loadMap(ALLEGRO_DISPLAY *display, struct mapValues curMap)
{
    //==============================================
    //PROJECT VARIABLES
    //==============================================
    FILE *fp;
    ALLEGRO_FILECHOOSER *file;

    //==============================================
    //AUXILIAR VARIABLES
    //==============================================
    int j, i;

    file = al_create_native_file_dialog("", "Choose the map File", "*.txt",ALLEGRO_FILECHOOSER_MULTIPLE);
    al_show_native_file_dialog(display, file);
    char mapNameTxt[100] = "";

    if(al_get_native_file_dialog_count(file) != 0)
    {
        // SAVE MAP TO FILE
        fp = fopen(mapNameTxt, "r");
        fscanf(fp, "%d %d\n", curMap.numLinhas, curMap.numColunas);
        for(i = 0; i < curMap.numLinhas; i++)
        {
            for(j = 0; j < curMap.numColunas; j++)
                fscanf(fp, "%d ", curMap.blocos[i][j]);
        }
        fclose(fp);
    }
}

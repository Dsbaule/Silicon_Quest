

extern DISPLAY_WIDTH;
extern DISPLAY_HEIGHT;



float CheckDistance(int x1, int y1, int x2, int y2)
{
    return sqrt(pow((float)x1 - x2, 2) + pow((float)y1 - y2, 2));
}

int detectColisionLeft_Matriz(struct Object character, struct mapValues curMap)
{
    int linhaAtual, colunaAtual;
    struct Object blocoAtual;
    bool result = 0;

    for(linhaAtual = 0; linhaAtual < curMap.numLinhas; linhaAtual++)
    {
        blocoAtual.y = curMap.y + (curMap.blockHeight * linhaAtual);
        blocoAtual.y2 = blocoAtual.y + curMap.blockHeight - 1;

        for(colunaAtual = 0; colunaAtual < curMap.numColunas; colunaAtual++)
        {
            blocoAtual.x = curMap.x + (curMap.blockWidth * colunaAtual);
            blocoAtual.x2 = blocoAtual.x + curMap.blockWidth - 1;

            if((blocoAtual.x >= 0) && (blocoAtual.y >= 0) && ((blocoAtual.x + curMap.blockWidth) <= DISPLAY_WIDTH) && ((blocoAtual.y + curMap.blockHeight) <= DISPLAY_HEIGHT))
            {
                if((character.x2 >= blocoAtual.x) && (character.x <= (blocoAtual.x2 + 1)))
                {
                    if(((character.y >= blocoAtual.y) && (character.y <= blocoAtual.y2))
                            || ((character.y2 >= blocoAtual.y) && (character.y2 <= blocoAtual.y2))
                            || ((character.y <= blocoAtual.y) && (character.y2 >= blocoAtual.y2)))
                    {
                        switch(curMap.blocos[linhaAtual][colunaAtual])
                        {
                        case 0: // AR
                            result |= 0;
                            break;
                        case 1: // TERRA
                        case 2: // PEDRA
                        case 3: // SILICIO
                            result |= 1;
                            break;
                        case 4: // LAVA
                        case 5: // AGUA
                            result |= 0;
                            break;
                        }
                    }
                }
            }
        }
    }

    if(character.x <= curMap.x + 1)
        result = 1;

    return result;
}

int detectColisionRight_Matriz(struct Object character, struct mapValues curMap)
{
    int linhaAtual, colunaAtual;
    struct Object blocoAtual;
    bool result = 0;

    for(linhaAtual = 0; linhaAtual < curMap.numLinhas; linhaAtual++)
    {
        blocoAtual.y = curMap.y + (curMap.blockHeight * linhaAtual);
        blocoAtual.y2 = blocoAtual.y + curMap.blockHeight - 1;

        for(colunaAtual = 0; colunaAtual < curMap.numColunas; colunaAtual++)
        {
            blocoAtual.x = curMap.x + (curMap.blockWidth * colunaAtual);
            blocoAtual.x2 = blocoAtual.x + curMap.blockWidth - 1;

            if((blocoAtual.x >= 0) && (blocoAtual.y >= 0) && ((blocoAtual.x + curMap.blockWidth) <= DISPLAY_WIDTH) && ((blocoAtual.y + curMap.blockHeight) <= DISPLAY_HEIGHT))
            {
                if((character.x2 >= blocoAtual.x) && (character.x <= (blocoAtual.x2 + 1)))
                {
                    if(((character.y >= blocoAtual.y) && (character.y <= blocoAtual.y2))
                            || ((character.y2 >= blocoAtual.y) && (character.y2 <= blocoAtual.y2))
                            || ((character.y <= blocoAtual.y) && (character.y2 >= blocoAtual.y2)))
                    {
                        switch(curMap.blocos[linhaAtual][colunaAtual])
                        {
                        case 0: // AR
                            result |= 0;
                            break;
                        case 1: // TERRA
                        case 2: // PEDRA
                        case 3: // SILICIO
                            result |= 1;
                            break;
                        case 4: // LAVA
                        case 5: // AGUA
                            result |= 0;
                            break;
                        }
                    }
                }
            }
        }
    }

    if(character.x <= curMap.x + 1)
        result = 1;

    return result;
}

int detectColisionUp_Matriz(struct Object character, struct mapValues curMap)
{
    int linhaAtual, colunaAtual;
    struct Object blocoAtual;
    bool result = 0;

    for(linhaAtual = 0; linhaAtual < curMap.numLinhas; linhaAtual++)
    {
        blocoAtual.y = curMap.y + (curMap.blockHeight * linhaAtual);
        blocoAtual.y2 = blocoAtual.y + curMap.blockHeight - 1;

        for(colunaAtual = 0; colunaAtual < curMap.numColunas; colunaAtual++)
        {
            blocoAtual.x = curMap.x + (curMap.blockWidth * colunaAtual);
            blocoAtual.x2 = blocoAtual.x + curMap.blockWidth - 1;

            if((blocoAtual.x >= 0) && (blocoAtual.y >= 0) && ((blocoAtual.x + curMap.blockWidth) <= DISPLAY_WIDTH) && ((blocoAtual.y + curMap.blockHeight) <= DISPLAY_HEIGHT))
            {
                if((character.x2 >= blocoAtual.x) && (character.x <= (blocoAtual.x2 + 1)))
                {
                    if(((character.y >= blocoAtual.y) && (character.y <= blocoAtual.y2))
                            || ((character.y2 >= blocoAtual.y) && (character.y2 <= blocoAtual.y2))
                            || ((character.y <= blocoAtual.y) && (character.y2 >= blocoAtual.y2)))
                    {
                        switch(curMap.blocos[linhaAtual][colunaAtual])
                        {
                        case 0: // AR
                            result |= 0;
                            break;
                        case 1: // TERRA
                        case 2: // PEDRA
                        case 3: // SILICIO
                            result |= 1;
                            break;
                        case 4: // LAVA
                        case 5: // AGUA
                            result |= 0;
                            break;
                        }
                    }
                }
            }
        }
    }

    if(character.x <= curMap.x + 1)
        result = 1;

    return result;
}

int detectColisionDown_Matriz(struct Object character, struct mapValues curMap)
{
    int linhaAtual, colunaAtual;
    struct Object blocoAtual;
    bool result = 0;

    for(linhaAtual = 0; linhaAtual < curMap.numLinhas; linhaAtual++)
    {
        blocoAtual.y = curMap.y + (curMap.blockHeight * linhaAtual);
        blocoAtual.y2 = blocoAtual.y + curMap.blockHeight - 1;

        for(colunaAtual = 0; colunaAtual < curMap.numColunas; colunaAtual++)
        {
            blocoAtual.x = curMap.x + (curMap.blockWidth * colunaAtual);
            blocoAtual.x2 = blocoAtual.x + curMap.blockWidth - 1;

            if((blocoAtual.x >= 0) && (blocoAtual.y >= 0) && ((blocoAtual.x + curMap.blockWidth) <= DISPLAY_WIDTH) && ((blocoAtual.y + curMap.blockHeight) <= DISPLAY_HEIGHT))
            {
                if((character.x2 >= blocoAtual.x) && (character.x <= (blocoAtual.x2 + 1)))
                {
                    if(((character.y >= blocoAtual.y) && (character.y <= blocoAtual.y2))
                            || ((character.y2 >= blocoAtual.y) && (character.y2 <= blocoAtual.y2))
                            || ((character.y <= blocoAtual.y) && (character.y2 >= blocoAtual.y2)))
                    {
                        switch(curMap.blocos[linhaAtual][colunaAtual])
                        {
                        case 0: // AR
                            result |= 0;
                            break;
                        case 1: // TERRA
                        case 2: // PEDRA
                        case 3: // SILICIO
                            result |= 1;
                            break;
                        case 4: // LAVA
                        case 5: // AGUA
                            result |= 0;
                            break;
                        }
                    }
                }
            }
        }
    }

    if(character.x <= curMap.x + 1)
        result = 1;

    return result;
}

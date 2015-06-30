#define MAX_LINHAS  500
#define MAX_COLUNAS 500

struct Object
{
    int x;
    int y;

    int width;
    int height;

    int x2;
    int y2;

    float   force;
    bool    jump;
};

struct mapValues
{
    int x;
    int y;

    int numLinhas;
    int numColunas;

    int blockWidth;
    int blockHeight;

    int8_t blocos[MAX_LINHAS][MAX_COLUNAS];
};

struct mouseValues
{
    int x;
    int y;

    int mouseWheelNow;
    int mouseWheelBefore;

    int linha;
    int coluna;
};

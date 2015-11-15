#define MAX_COLUNAS 2500
#define MAX_LINHAS  2500

#define MAX_ENEMIES 100


struct Sprite_Animation
{
    ALLEGRO_BITMAP *Image;

    int maxFrame;
    int curFrame;
    int frameCount;
    int frameDelay;
    int frameWidth;
    int frameHeight;
};

struct Players
{
	int x;
	int y;
	int boundx;
	int boundy;
	int width;
	int height;
	int boundx2;
	int boundy2;

	bool jump;
	bool mine;
	bool initialized;
	float force;

	bool colisionUp;
	bool colisionDown;
	bool colisionLeft;
	bool colisionRight;

	int silicio;

	int tresHold;
	int miningSpeed;

	struct Sprite_Animation running;
    struct Sprite_Animation idle;
    struct Sprite_Animation dying;
    struct Sprite_Animation mining;
    struct Sprite_Animation standing;
    struct Sprite_Animation blockCracking;

	int direction;
	int state;
};

struct Enemies
{
	int x[MAX_ENEMIES];
	int y[MAX_ENEMIES];
	int boundx[MAX_ENEMIES];
	int boundy[MAX_ENEMIES];
	int centerx[MAX_ENEMIES];
	int centery[MAX_ENEMIES];

	bool direction[MAX_ENEMIES];
	bool state[MAX_ENEMIES];
	bool active[MAX_ENEMIES];

	struct Sprite_Animation idle;

	int animationRow;
};

/* A ser implementado
struct Bullets
{
    ;
};
*/

struct Maps
{
    int x;
    int y;

    int numColunas;
    int numLinhas;

    int numEnemies;
    int numSilicio;
    bool hasPlayer;

    bool loaded;

    int blockWidth;
    int blockHeight;

    int Blocos[MAX_LINHAS][MAX_COLUNAS];
};

struct Bloco
{
    int x;
    int y;
    int x2;
    int y2;
};

struct Explosions
{
	int x;
	int y;
	bool live;

	int maxFrame;
	int curFrame;
	int frameCount;
	int frameDelay;
	int frameWidth;
	int frameHeight;
	int animationColumns;
	int animationDirection;

	ALLEGRO_BITMAP *image;
};

struct Backgrounds
{
	float x;
	float y;

	int width;
	int height;

	ALLEGRO_BITMAP *image;
};

struct Mouses
{
    int x;
    int y;

    int wheelNow;
    int wheelBefore;

    int cursor;

    int linha;
    int coluna;

    int selectedBlock;
};

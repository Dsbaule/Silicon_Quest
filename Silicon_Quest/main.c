//--------------------------------------------------
// Inclusão das bibliotecas do Allegro 5
//--------------------------------------------------
#include <allegro5/allegro.h>   // Biblioteca padrão allegro
#include <allegro5/allegro_font.h>  // Biblioteca para utilização de fontes
#include <allegro5/allegro_ttf.h>   // Biblioteca para utilização de fontes
#include <allegro5/allegro_primitives.h>    // Biblioteca para utilização de primitivos
#include <allegro5/allegro_image.h>         // Biblioteca para utilização de bitmaps
#include <allegro5/allegro_native_dialog.h> // Biblioteca para utilização de caixas de texto do SO
#include <allegro5/allegro_audio.h>         // Biblioteca para utilização de audio
#include <allegro5\allegro_acodec.h>        // Biblioteca para utilização de audio
//--------------------------------------------------
// Inclusão das bibliotecas padrões C
//--------------------------------------------------
#include <stdio.h>      // Biblioteca utilizada para escrita em arquivo
#include <string.h>     // Biblioteca para a manipulação de Strings
#include <math.h>       // Biblioteca utilizada para calculo da distancia

//--------------------------------------------------
// Inclusão dos headers criados
//--------------------------------------------------
#include "Structs.h" // Header com as estruturas utilizadas

//--------------------------------------------------
// Definição dos valores para o compilador
//--------------------------------------------------
// Definições da tela
#define FULLSCREEN      0
#define DISPLAY_WIDTH   1600
#define DISPLAY_HEIGHT  900
// Definições dos Timers
#define FPS             60
#define MOVEMENT_SPEED  500
#define BOOST_SPEED     1000
#define MOVEMENT_STEP   1
#define MENU_SPEED      7
// Definições de desenho
#define SHOWMOUSE       0
#define SHOW_BORDER     1
#define SHOW_MAP_LIMITS 1
// Definições do mapa
#define MAX_ENEMIES 100
#define NUM_BLOCOS  6
#define PLAYER_SCALE 3
#define GRAVITY     0.6
#define JUMP_FORCE  2
#define JUMP_TIME   10

//--------------------------------------------------
// Definição das variaveis globais para o Allegro
//--------------------------------------------------
ALLEGRO_TIMER *drawTimer = NULL;
ALLEGRO_TIMER *movementTimer = NULL;
ALLEGRO_TIMER *movementBoostTimer = NULL;
ALLEGRO_TIMER *menuTimer = NULL;
ALLEGRO_TIMER *jumpTimer = NULL;

ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT ev;

ALLEGRO_DISPLAY *display = NULL;

//--------------------------------------------------
// Definição das variaveis para INPUT
//--------------------------------------------------
bool keys[62] = {false};
enum keys {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
           N0, N1, N2, N3, N4, N5, N6, N7, N8, N9,
           F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
           UP, DOWN, RIGHT, LEFT, TAB, SHIFT, CTRL, ALT, ENTER, BACKSPACE, SPACE, ESC, MOUSE_1, MOUSE_2
          };

struct Mouses mouse = {0};

//--------------------------------------------------
// Definição das variaveis de estado
//--------------------------------------------------
bool movement = false;
bool movementBoost = false;
bool readMenu = false;
bool jumpResistance = false;
bool draw = false;

//--------------------------------------------------
// Definição das variaveis gerais globais
//--------------------------------------------------
struct Maps Map = {0};

//--------------------------------------------------
// Definição das funções utilizadas
//--------------------------------------------------

//prototypes
void InitPlayer(struct Players *Player, struct Maps *curMap);
void ResetPlayerAnimation(struct Players *Player, int animation);
void DrawPlayer(struct Players *Player);
void MovePlayerLeft(struct Players *Player);
void MovePlayerRight(struct Players *Player);

/* To be added - Bullet
void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, SpaceShip &ship);
void UpdateBullet(Bullet bullet[], int size);
void CollideBullet(Bullet bullet[], int bSize, Comet comets[], int cSize, SpaceShip &ship, Explosion explosions[], int eSize);
*/

void InitEnemy(struct Enemies Enemy[], ALLEGRO_BITMAP *image);
void DrawEnemy(struct Enemies Enemy[]);
void StartEnemy(struct Enemies Enemy[]);
void UpdateEnemy(struct Enemies Enemy[]);
void CollideEnemy(struct Enemies Enemy[], struct Players *Player, struct Explosions Explosion[], int eSize);

void InitExplosions(struct Explosions Explosion, int size, ALLEGRO_BITMAP *image);
void DrawExplosions(struct Explosions Explosion, int size);
void StartExplosions(struct Explosions Explosion, int size, int x, int y);
void UpdateExplosions(struct Explosions Explosion, int size);

void InitBackground(struct Backgrounds *brack, float x, float y, float velX, float velY, int width, int height, int dirX, int dirY, ALLEGRO_BITMAP *image);
void UpdateBackground(struct Backgrounds *back);
void DrawBackground(struct Backgrounds *back);

void ChangeState(int *state, int newState);

int mainMenu();
int gameMenu();
int gamePause();
int mapCreatorMenu();
int mapCreatorPause();

int game();
int mapCreator();

int checkEvents();
void readInputs();
void saveMap();

int detectColisionRight_Matriz(struct Players *Player, struct Maps *curMap);
int detectColisionLeft_Matriz(struct Players *Player, struct Maps *curMap);
int detectColisionUp_Matriz(struct Players *Player, struct Maps *curMap);
int detectColisionDown_Matriz(struct Players *Player, struct Maps *curMap);

float CheckDistance(int x1, int y1, int x2, int y2);

int main()
{
    //Primitive variable
    int gameState = 2;
    int selectedOption = 0;

    bool done = false;
    bool redraw = true;

    //Allegro Bitmaps
    ALLEGRO_FONT *arial_18 = NULL;
    ALLEGRO_FONT *bankGothic_24 = NULL;

    ALLEGRO_BITMAP *enemyImage;
    ALLEGRO_BITMAP *expImage;

    ALLEGRO_BITMAP *mainMenuImage = NULL;

    ALLEGRO_BITMAP *gameMenuImage = NULL;
    ALLEGRO_BITMAP *gamePauseImage = NULL;

    ALLEGRO_BITMAP *mapCreatorMenu1Image = NULL;
    ALLEGRO_BITMAP *mapCreatorMenu2Image = NULL;
    ALLEGRO_BITMAP *mapCreatorPauseImage = NULL;

    ALLEGRO_BITMAP *gameOverImage = NULL;

    ALLEGRO_BITMAP *background = NULL;

    ALLEGRO_BITMAP *blocos = NULL;
    ALLEGRO_BITMAP *blockCracks = NULL;

    ALLEGRO_BITMAP *pickaxeCursor = NULL;
    ALLEGRO_BITMAP *idleCursor = NULL;

    //Initialization Functions
    if(!al_init())
    {
        al_show_native_message_box(NULL, "ERRO", "Incapaz de inicializar o Allegro", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    // Inicialização dos módulos adicionados
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();
    al_init_image_addon();
    al_install_audio();
    al_init_acodec_addon();
    al_install_keyboard();
    al_install_mouse();

    // Setup do Display e opções de visualização
    if(FULLSCREEN)
        al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display = al_create_display(DISPLAY_WIDTH, DISPLAY_HEIGHT);

    if(!SHOWMOUSE)
        al_hide_mouse_cursor(display);

    if(!display)
    {
        al_show_native_message_box(NULL, "ERRO", "Incapaz de inicializar o Display", NULL, NULL, ALLEGRO_MESSAGEBOX_ERROR);
        return -1;
    }

    struct Players Player = {0};
    struct Enemies Enemy[MAX_ENEMIES] = {{0}};
    struct Explosions Explosion = {0};

    //Setup dos timers
    drawTimer = al_create_timer(1.0 / FPS);
    movementTimer = al_create_timer(1.0 / MOVEMENT_SPEED);
    movementBoostTimer = al_create_timer(1.0 / BOOST_SPEED);
    jumpTimer = al_create_timer(1.0 / JUMP_TIME);
    menuTimer = al_create_timer(1.0 / MENU_SPEED);

    //Setup das Bitmaps
    Player.standing.Image = al_load_bitmap("Bitmaps/minerStanding.png");
    Player.running.Image = al_load_bitmap("Bitmaps/minerRunning.png");
    Player.mining.Image = al_load_bitmap("Bitmaps/minerMining.png");
    Player.idle.Image = al_load_bitmap("Bitmaps/minIdle.png");
    Player.dying.Image = al_load_bitmap("Bitmaps/minerDying.png");

    enemyImage = al_load_bitmap("Bitmaps/MineTurtle.png");

    blocos = al_load_bitmap("Bitmaps/Blocos.png");
    blockCracks = al_load_bitmap("Bitmaps/BlockCracks.png");

    mainMenuImage = al_load_bitmap("Bitmaps/MainMenu.png");

    gameMenuImage = al_load_bitmap("Bitmaps/GameMenu1.png");
    gamePauseImage = al_load_bitmap("Bitmaps/GamePause.png");

    mapCreatorMenu1Image = al_load_bitmap("Bitmaps/MapCreatorMenu1.png");
    mapCreatorMenu2Image = al_load_bitmap("Bitmaps/MapCreatorMenu2.png");
    mapCreatorPauseImage = al_load_bitmap("Bitmaps/MapCreatorPause.png");

    gameOverImage = al_load_bitmap("Bitmaps/GameOver.png");

    //Setup das Fontes
    arial_18 = al_load_font("arial.ttf", 18, 0);
    bankGothic_24 = al_load_font("bgothl.ttf", 24, 0);

    //Setup dos Eventos
    event_queue = al_create_event_queue();

    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_register_event_source(event_queue, al_get_display_event_source(display));

    al_register_event_source(event_queue, al_get_timer_event_source(drawTimer));
    al_register_event_source(event_queue, al_get_timer_event_source(movementTimer));
    al_register_event_source(event_queue, al_get_timer_event_source(movementBoostTimer));
    al_register_event_source(event_queue, al_get_timer_event_source(menuTimer));
    al_register_event_source(event_queue, al_get_timer_event_source(jumpTimer));

    // Inicialização dos Timers
    al_start_timer(drawTimer);
    al_start_timer(movementTimer);
    al_start_timer(movementBoostTimer);
    al_start_timer(menuTimer);
    al_start_timer(jumpTimer);

    InitPlayer(&Player, &Map);

    while(!done)
    {
        al_wait_for_event(event_queue, &ev);
        done = checkEvents();

        if(gameState == 0)
            gameState = mainMenu(&Player);
        else if(gameState == 1)
            gameState = gameMenu();
        else if(gameState == 2)
            gameState = game(&Player, &Map, &Enemy);
        else if(gameState == 3)
            gameState = gamePause();
        else if(gameState == 4)
            gameState = mapCreatorMenu();
        else if(gameState == 5)
            gameState = mapCreator();
        else if(gameState == 6)
            gameState = mapCreatorPause();
        else if(gameState == -1)
            done = true;
    }

	al_destroy_bitmap(Player.standing.Image);
	al_destroy_bitmap(Player.running.Image);
	al_destroy_bitmap(Player.mining.Image);
	al_destroy_bitmap(Player.idle.Image);
	al_destroy_bitmap(Player.dying.Image);
	al_destroy_bitmap(enemyImage);
	al_destroy_bitmap(blocos);
	al_destroy_bitmap(pickaxeCursor);
	al_destroy_bitmap(idleCursor);
	al_destroy_bitmap(mainMenuImage);
	al_destroy_bitmap(gameMenuImage);
	al_destroy_bitmap(gamePauseImage);
	al_destroy_bitmap(mapCreatorMenu1Image);
	al_destroy_bitmap(mapCreatorMenu2Image);
	al_destroy_bitmap(mapCreatorPauseImage);
	al_destroy_bitmap(gameOverImage);

	al_destroy_event_queue(event_queue);

	al_destroy_timer(drawTimer);
	al_destroy_timer(movementTimer);
	al_destroy_timer(movementBoostTimer);
	al_destroy_timer(jumpTimer);
	al_destroy_timer(menuTimer);

	al_destroy_font(arial_18);
	al_destroy_font(bankGothic_24);

	al_destroy_display(display);

    return 0;
}

void InitPlayer(struct Players *Player, struct Maps *curMap)
{
    int i, j;

    ResetPlayerAnimation(Player, 0);
    ResetPlayerAnimation(Player, 1);
    ResetPlayerAnimation(Player, 2);
    ResetPlayerAnimation(Player, 3);
    ResetPlayerAnimation(Player, 4);

    Player->boundx = (DISPLAY_WIDTH/2) - (Player->width/2);
    Player->boundy = (DISPLAY_HEIGHT/2) - (Player->height/2);

    Player->width = 46;
    Player->height = 100;

    for(i = 0; i < curMap->numLinhas; i++)
    {
        for(j = 0; j < curMap->numColunas; j++)
        {
            if(curMap->Blocos[i][j] == 5)
            {
                curMap->Blocos[i][j] == 0;
                Player->boundx = curMap->x + (j * curMap->blockWidth);
                Player->boundy = curMap->y + (i * curMap->blockHeight);
            }
        }
    }

    updatePlayer(Player);
}

void ResetPlayerAnimation(struct Players *Player, int animation)
{
    if(animation == 0)
    {
        Player->standing.maxFrame = 1;
        Player->standing.frameDelay = 0;
        Player->standing.frameCount = 0;
        Player->standing.curFrame = 0;
        Player->standing.frameHeight = 80;
        Player->standing.frameWidth = 80;
    }

    if(animation == 1)
    {
        Player->running.maxFrame = 6;
        Player->running.frameDelay = 6;
        Player->running.frameCount = 0;
        Player->running.curFrame = 0;
        Player->running.frameHeight = 80;
        Player->running.frameWidth = 80;
    }

    if(animation == 2)
    {
        Player->mining.maxFrame = 11;
        Player->mining.frameDelay = 3;
        Player->mining.frameCount = 0;
        Player->mining.curFrame = 0;
        Player->mining.frameHeight = 80;
        Player->mining.frameWidth = 80;
    }

    if(animation == 3)
    {
        Player->idle.maxFrame = 4;
        Player->idle.frameDelay = 25;
        Player->idle.frameCount = 0;
        Player->idle.curFrame = 0;
        Player->idle.frameHeight = 80;
        Player->idle.frameWidth = 80;
    }

    if(animation == 4)
    {
        Player->dying.maxFrame = 6;
        Player->dying.frameDelay = 25;
        Player->dying.frameCount = 0;
        Player->dying.curFrame = 0;
        Player->dying.frameHeight = 80;
        Player->dying.frameWidth = 80;
    }
}

void DrawPlayer(struct Players *Player)
{
    if((Player->state == 0) && (Player->direction == 1))
    {
        al_draw_scaled_bitmap(Player->standing.Image, Player->standing.curFrame * Player->standing.frameWidth, 0, Player->standing.frameWidth, Player->standing.frameHeight, Player->x, Player->y, 200, 200,0);
    }
    else if((Player->state == 0) && (Player->direction == 0))
    {
        al_draw_scaled_bitmap(Player->standing.Image, Player->standing.curFrame * Player->standing.frameWidth, 0, Player->standing.frameWidth, Player->standing.frameHeight, Player->x, Player->y, Player->standing.frameWidth * 2.5, Player->standing.frameHeight * 2.5, ALLEGRO_FLIP_HORIZONTAL);
    }
    else if((Player->state == 1) && (Player->direction == 0))
    {
        al_draw_scaled_bitmap(Player->running.Image, Player->running.curFrame * Player->running.frameWidth, 0, Player->running.frameWidth, Player->running.frameHeight, Player->x, Player->y, Player->running.frameWidth * 2.5, Player->running.frameHeight * 2.5,0);
    }
    else if((Player->state == 1) && (Player->direction == 1))
    {
        al_draw_scaled_bitmap(Player->running.Image, Player->running.curFrame * Player->running.frameWidth, 0, Player->running.frameWidth, Player->running.frameHeight, Player->x, Player->y, Player->running.frameWidth * 2.5, Player->running.frameHeight * 2.5, ALLEGRO_FLIP_HORIZONTAL);
    }
    else if((Player->state == 1) && (Player->direction == 0))
    {
        al_draw_scaled_bitmap(Player->mining.Image, Player->mining.curFrame * Player->mining.frameWidth, 0, Player->mining.frameWidth, Player->mining.frameHeight, Player->x, Player->y, Player->mining.frameWidth * 2.5, Player->mining.frameHeight * 2.5,0);
    }
    else if((Player->state == 1) && (Player->direction == 1))
    {
        al_draw_scaled_bitmap(Player->standing.Image, Player->standing.curFrame * Player->standing.frameWidth, 0, Player->standing.frameWidth, Player->standing.frameHeight, Player->x, Player->y, Player->standing.frameWidth * 2.5, Player->standing.frameHeight * 2.5, ALLEGRO_FLIP_HORIZONTAL);
    }
    else if((Player->state == 1) && (Player->direction == 0))
    {
        al_draw_scaled_bitmap(Player->idle.Image, Player->idle.curFrame * Player->idle.frameWidth, 0, Player->idle.frameWidth, Player->idle.frameHeight, Player->x, Player->y, Player->idle.frameWidth * 2.5, Player->idle.frameHeight * 2.5,0);
    }
    else if((Player->state == 1) && (Player->direction == 1))
    {
        al_draw_scaled_bitmap(Player->standing.Image, Player->standing.curFrame * Player->standing.frameWidth, 0, Player->standing.frameWidth, Player->standing.frameHeight, Player->x, Player->y, Player->standing.frameWidth * 2.5, Player->standing.frameHeight * 2.5, ALLEGRO_FLIP_HORIZONTAL);
    }
    else if((Player->state == 1) && (Player->direction == 0))
    {
        al_draw_scaled_bitmap(Player->dying.Image, Player->dying.curFrame * Player->dying.frameWidth, 0, Player->dying.frameWidth, Player->dying.frameHeight, Player->x, Player->y, Player->dying.frameWidth * 2.5, Player->dying.frameHeight * 2.5,0);
    }
    else if((Player->state == 1) && (Player->direction == 1))
    {
        al_draw_scaled_bitmap(Player->standing.Image, Player->standing.curFrame * Player->standing.frameWidth, 0, Player->standing.frameWidth, Player->standing.frameHeight, Player->x, Player->y, Player->standing.frameWidth * 2.5, Player->standing.frameHeight * 2.5, ALLEGRO_FLIP_HORIZONTAL);
    }
}

void animatePlayer(struct Players *Player)
{
    if(Player->state == 0)
    {
        if(++Player->standing.frameCount >= Player->standing.frameDelay)
        {
            if(++Player->standing.curFrame >= Player->standing.maxFrame)
                Player->standing.curFrame = 0;
            Player->standing.frameCount = 0;
        }
    }
    else
    {
        Player->standing.curFrame = 0;
    }

    if(Player->state == 1 || Player->state == 2)
    {
        if(++Player->running.frameCount >= Player->running.frameDelay)
        {
            if(++Player->running.curFrame >= Player->running.maxFrame)
                Player->running.curFrame = 0;
            Player->running.frameCount = 0;
        }
    }
    else
    {
        Player->running.curFrame = 0;
    }

    if(Player->state == 3)
    {
        if(++Player->idle.frameCount >= Player->idle.frameDelay)
        {
            if(++Player->idle.curFrame >= Player->idle.maxFrame)
                Player->idle.curFrame = 1;
            Player->idle.frameCount = 0;
        }
    }
    else
    {
        Player->idle.curFrame = 0;
    }

    if(Player->state == 4)
    {
        if(++Player->mining.frameCount >= Player->mining.frameDelay)
        {
            if(++Player->mining.curFrame >= Player->mining.maxFrame)
                Player->mining.curFrame = 0;
            Player->mining.frameCount = 0;
        }
    }
    else
    {
        Player->mining.curFrame = 0;
    }

    if(Player->state == 5)
    {
        if(++Player->dying.frameCount >= Player->dying.frameDelay)
        {
            if(++Player->dying.curFrame >= Player->dying.maxFrame)
                Player->dying.curFrame = 0;
            Player->dying.frameCount = 0;
        }
    }
    else
    {
        Player->dying.curFrame = 0;
    }
}

void MovePlayerLeft(struct Players *Player)
{
    Player->state = 1;
    Player->direction = 1;

    Player->boundx -= MOVEMENT_STEP;

    if(Player->boundx < 0)
        Player->boundx = 0;
}

void MovePlayerRight(struct Players *Player)
{
    Player->state = 1;
    Player->direction = 0;

    Player->boundx += MOVEMENT_STEP;

    if(Player->boundx > (DISPLAY_WIDTH - Player->width))
        Player->boundx = (DISPLAY_WIDTH - Player->width);
}

void playerJump(struct Players *Player)
{
    Player->jump = !detectColisionDown_Matriz(Player, &Map);

    if((keys[W] || keys[UP]) && (Player->colisionUp == 0) && (Player->y > 0) && !Player->jump)
    {
        Player->jump = true;
        Player->force = JUMP_FORCE;
    }

    if(Player->jump)
    {
        Player->boundy -= Player->force;
    }

    if(Player->force < -10)
        Player->force = -10;

    if(jumpResistance)
    {
        if(Player->jump)
        {
            Player->force -= GRAVITY;
        }

        jumpResistance = false;
    }
}

void updatePlayer(struct Players *Player)
{
    animatePlayer(Player);

    Player->boundx2 = Player->boundx + Player->width - 1;
    Player->boundy2 = Player->boundy + Player->height - 1;

    Player->x = Player->boundx - 78;
    Player->y = Player->boundy - 76;
}

void updateMapPosition(struct Players *Player, struct Maps *curMap)
{
    if((curMap->x <= 0) && (Player->boundx <= ((DISPLAY_WIDTH/2) - (Player->width/2))))
    {
        curMap->x += ((DISPLAY_WIDTH/2) - (Player->width/2)) - Player->boundx;
        Player->boundx = ((DISPLAY_WIDTH/2) - (Player->width/2));
    }

    if((curMap->y <= 0) && (Player->boundy <= ((DISPLAY_HEIGHT/2) - (Player->height/2))))
    {
        curMap->y += ((DISPLAY_HEIGHT/2) - (Player->height/2)) - Player->boundy;
        Player->boundy = ((DISPLAY_HEIGHT/2) - (Player->height/2));
    }

    if(((curMap->x + (curMap->numColunas * curMap->blockWidth)) > DISPLAY_WIDTH) && (Player->boundx >= ((DISPLAY_WIDTH/2) - (Player->width/2))))
    {
        curMap->x += ((DISPLAY_WIDTH/2) - (Player->width/2)) - Player->boundx;
        Player->boundx = ((DISPLAY_WIDTH/2) - (Player->width/2));
    }

    if(((curMap->y + (curMap->numLinhas * curMap->blockHeight)) > DISPLAY_HEIGHT) && (Player->boundy >= ((DISPLAY_HEIGHT/2) - (Player->height/2))))
    {
        curMap->y += ((DISPLAY_HEIGHT/2) - (Player->height/2)) - Player->boundy;
        Player->boundy = ((DISPLAY_HEIGHT/2) - (Player->height/2));
    }
}

void InitEnemy(struct Enemies Enemy[], ALLEGRO_BITMAP *image)
{

}

void DrawEnemy(struct Enemies Enemy[])
{

}

void StartEnemy(struct Enemies Enemy[])
{

}

void UpdateEnemy(struct Enemies Enemy[])
{

}

void CollideEnemy(struct Enemies Enemy[], struct Players *Player, struct Explosions Explosion[], int eSize)
{

}


void InitExplosions(struct Explosions Explosion, int size, ALLEGRO_BITMAP *image)
{

}

void DrawExplosions(struct Explosions Explosion, int size)
{

}

void StartExplosions(struct Explosions Explosion, int size, int x, int y)
{

}

void UpdateExplosions(struct Explosions Explosion, int size)
{

}


void InitBackground(struct Backgrounds *brack, float x, float y, float velX, float velY, int width, int height, int dirX, int dirY, ALLEGRO_BITMAP *image)
{

}

void UpdateBackground(struct Backgrounds *back)
{

}

void DrawBackground(struct Backgrounds *back)
{

}


void ChangeState(int *state, int newState)
{

}


int mainMenu(struct Players *Player)
{

}

int gameMenu()
{
    return 1;
}

int game(struct Players *Player, struct Maps *curMap, struct Enemies *Enemy[])
{
    Player->colisionDown = !detectColisionDown_Matriz(Player, curMap);
    Player->colisionUp = !detectColisionUp_Matriz(Player, curMap);

    if(keys[S] && Player->colisionDown)
        Player->boundy++;

    if(keys[W] && Player->colisionUp)
        Player->boundy--;

    if(keys[A])
        Player->direction = 1;

    if(keys[D])
        Player->direction = 0;

    Player->colisionDown = !detectColisionDown_Matriz(Player, curMap);
    Player->colisionUp = !detectColisionUp_Matriz(Player, curMap);

    updatePlayer(Player);
    al_draw_rectangle(Player->boundx, Player->boundy, Player->boundx2, Player->boundy2, al_map_rgb(255, 255, 255), 1);
    DrawPlayer(Player);

    al_flip_display();
    al_clear_to_color(al_map_rgb(0,0,0));

    return 2;
}

int gamePause()
{
    return 3;
}

int mapCreatorMenu()
{
    return 4;
}

int mapCreator()
{
    return 5;
}

int mapCreatorPause()
{
    return 6;
}

int checkEvents()
{
    switch(ev.type)
    {
    case ALLEGRO_EVENT_KEY_DOWN:
    case ALLEGRO_EVENT_KEY_UP:
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
    case ALLEGRO_EVENT_MOUSE_AXES:
        readInputs();
        break;
    case ALLEGRO_EVENT_TIMER:
        if(ev.timer.source == drawTimer)
            draw = true;
        else if(ev.timer.source == movementTimer)
            movement = true;
        else if(ev.timer.source == movementBoostTimer)
            movementBoost = true;
        else if(ev.timer.source == jumpTimer)
            jumpResistance = true;
        else if(ev.timer.source == menuTimer)
        {
            al_stop_timer(menuTimer);
            readMenu = true;
        }
        break;
    case ALLEGRO_EVENT_DISPLAY_CLOSE:
        return 1;
        break;
    }
    return 0;
}

void readInputs()
{
    // Leitura dos eixos do mouse
    if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
    {
        mouse.x = ev.mouse.x;
        mouse.y = ev.mouse.y;
        mouse.wheelNow = ev.mouse.z;
    }

    // Leitura dos botões do mouse
    if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
    {
        if(ev.mouse.button & 1)
            keys[MOUSE_1] = true;

        if(ev.mouse.button & 2)
            keys[MOUSE_2] = true;
    }
    else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP)
    {
        if(ev.mouse.button & 1)
            keys[MOUSE_1] = false;

        if(ev.mouse.button & 2)
            keys[MOUSE_2] = false;
    }
    // READ KEYBOARD INPUT
    if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
    {
        switch(ev.keyboard.keycode)
        {
        case ALLEGRO_KEY_A:
            keys[A] = true;
            break;
        case ALLEGRO_KEY_B:
            keys[B] = true;
            break;
        case ALLEGRO_KEY_C:
            keys[C] = true;
            break;
        case ALLEGRO_KEY_D:
            keys[D] = true;
            break;
        case ALLEGRO_KEY_E:
            keys[E] = true;
            break;
        case ALLEGRO_KEY_F:
            keys[F] = true;
            break;
        case ALLEGRO_KEY_G:
            keys[G] = true;
            break;
        case ALLEGRO_KEY_H:
            keys[H] = true;
            break;
        case ALLEGRO_KEY_I:
            keys[I] = true;
            break;
        case ALLEGRO_KEY_J:
            keys[J] = true;
            break;
        case ALLEGRO_KEY_K:
            keys[K] = true;
            break;
        case ALLEGRO_KEY_L:
            keys[L] = true;
            break;
        case ALLEGRO_KEY_M:
            keys[M] = true;
            break;
        case ALLEGRO_KEY_N:
            keys[N] = true;
            break;
        case ALLEGRO_KEY_O:
            keys[O] = true;
            break;
        case ALLEGRO_KEY_P:
            keys[P] = true;
            break;
        case ALLEGRO_KEY_Q:
            keys[Q] = true;
            break;
        case ALLEGRO_KEY_R:
            keys[R] = true;
            break;
        case ALLEGRO_KEY_S:
            keys[S] = true;
            break;
        case ALLEGRO_KEY_T:
            keys[T] = true;
            break;
        case ALLEGRO_KEY_U:
            keys[U] = true;
            break;
        case ALLEGRO_KEY_V:
            keys[V] = true;
            break;
        case ALLEGRO_KEY_W:
            keys[W] = true;
            break;
        case ALLEGRO_KEY_X:
            keys[X] = true;
            break;
        case ALLEGRO_KEY_Y:
            keys[Y] = true;
            break;
        case ALLEGRO_KEY_Z:
            keys[Z] = true;
            break;

        case ALLEGRO_KEY_0:
        case ALLEGRO_KEY_PAD_0:
            keys[N0] = true;
            break;
        case ALLEGRO_KEY_1:
        case ALLEGRO_KEY_PAD_1:
            keys[N1] = true;
            break;
        case ALLEGRO_KEY_2:
        case ALLEGRO_KEY_PAD_2:
            keys[N2] = true;
            break;
        case ALLEGRO_KEY_3:
        case ALLEGRO_KEY_PAD_3:
            keys[N3] = true;
            break;
        case ALLEGRO_KEY_4:
        case ALLEGRO_KEY_PAD_4:
            keys[N4] = true;
            break;
        case ALLEGRO_KEY_5:
        case ALLEGRO_KEY_PAD_5:
            keys[N5] = true;
            break;
        case ALLEGRO_KEY_6:
        case ALLEGRO_KEY_PAD_6:
            keys[N6] = true;
            break;
        case ALLEGRO_KEY_7:
        case ALLEGRO_KEY_PAD_7:
            keys[N7] = true;
            break;
        case ALLEGRO_KEY_8:
        case ALLEGRO_KEY_PAD_8:
            keys[N8] = true;
            break;
        case ALLEGRO_KEY_9:
        case ALLEGRO_KEY_PAD_9:
            keys[N9] = true;
            break;

        case ALLEGRO_KEY_F1:
            keys[F1] = true;
            break;
        case ALLEGRO_KEY_F2:
            keys[F2] = true;
            break;
        case ALLEGRO_KEY_F3:
            keys[F3] = true;
            break;
        case ALLEGRO_KEY_F4:
            keys[F4] = true;
            break;
        case ALLEGRO_KEY_F5:
            keys[F5] = true;
            break;
        case ALLEGRO_KEY_F6:
            keys[F6] = true;
            break;
        case ALLEGRO_KEY_F7:
            keys[F7] = true;
            break;
        case ALLEGRO_KEY_F8:
            keys[F8] = true;
            break;
        case ALLEGRO_KEY_F9:
            keys[F9] = true;
            break;
        case ALLEGRO_KEY_F10:
            keys[F10] = true;
            break;
        case ALLEGRO_KEY_F11:
            keys[F11] = true;
            break;
        case ALLEGRO_KEY_F12:
            keys[F12] = true;
            break;

        case ALLEGRO_KEY_UP:
            keys[UP] = true;
            break;
        case ALLEGRO_KEY_DOWN:
            keys[DOWN] = true;
            break;
        case ALLEGRO_KEY_RIGHT:
            keys[RIGHT] = true;
            break;
        case ALLEGRO_KEY_LEFT:
            keys[LEFT] = true;
            break;
        case ALLEGRO_KEY_TAB:
            keys[TAB] = true;
            break;
        case ALLEGRO_KEY_LSHIFT:
        case ALLEGRO_KEY_RSHIFT:
            keys[SHIFT] = true;
            break;
        case ALLEGRO_KEY_LCTRL:
        case ALLEGRO_KEY_RCTRL:
            keys[CTRL] = true;
            break;
        case ALLEGRO_KEY_ALT:
            keys[ALT] = true;
            break;
        case ALLEGRO_KEY_ENTER:
        case ALLEGRO_KEY_PAD_ENTER:
            keys[ENTER] = true;
            break;
        case ALLEGRO_KEY_BACKSPACE:
            keys[BACKSPACE] = true;
            break;
        case ALLEGRO_KEY_SPACE:
            keys[SPACE] = true;
            break;
        case ALLEGRO_KEY_ESCAPE:
            keys[ESC] = true;
            break;
        }
    }
    else if(ev.type == ALLEGRO_EVENT_KEY_UP)
    {
        switch(ev.keyboard.keycode)
        {
        case ALLEGRO_KEY_A:
            keys[A] = false;
            break;
        case ALLEGRO_KEY_B:
            keys[B] = false;
            break;
        case ALLEGRO_KEY_C:
            keys[C] = false;
            break;
        case ALLEGRO_KEY_D:
            keys[D] = false;
            break;
        case ALLEGRO_KEY_E:
            keys[E] = false;
            break;
        case ALLEGRO_KEY_F:
            keys[F] = false;
            break;
        case ALLEGRO_KEY_G:
            keys[G] = false;
            break;
        case ALLEGRO_KEY_H:
            keys[H] = false;
            break;
        case ALLEGRO_KEY_I:
            keys[I] = false;
            break;
        case ALLEGRO_KEY_J:
            keys[J] = false;
            break;
        case ALLEGRO_KEY_K:
            keys[K] = false;
            break;
        case ALLEGRO_KEY_L:
            keys[L] = false;
            break;
        case ALLEGRO_KEY_M:
            keys[M] = false;
            break;
        case ALLEGRO_KEY_N:
            keys[N] = false;
            break;
        case ALLEGRO_KEY_O:
            keys[O] = false;
            break;
        case ALLEGRO_KEY_P:
            keys[P] = false;
            break;
        case ALLEGRO_KEY_Q:
            keys[Q] = false;
            break;
        case ALLEGRO_KEY_R:
            keys[R] = false;
            break;
        case ALLEGRO_KEY_S:
            keys[S] = false;
            break;
        case ALLEGRO_KEY_T:
            keys[T] = false;
            break;
        case ALLEGRO_KEY_U:
            keys[U] = false;
            break;
        case ALLEGRO_KEY_V:
            keys[V] = false;
            break;
        case ALLEGRO_KEY_W:
            keys[W] = false;
            break;
        case ALLEGRO_KEY_X:
            keys[X] = false;
            break;
        case ALLEGRO_KEY_Y:
            keys[Y] = false;
            break;
        case ALLEGRO_KEY_Z:
            keys[Z] = false;
            break;

        case ALLEGRO_KEY_0:
        case ALLEGRO_KEY_PAD_0:
            keys[N0] = false;
            break;
        case ALLEGRO_KEY_1:
        case ALLEGRO_KEY_PAD_1:
            keys[N1] = false;
            break;
        case ALLEGRO_KEY_2:
        case ALLEGRO_KEY_PAD_2:
            keys[N2] = false;
            break;
        case ALLEGRO_KEY_3:
        case ALLEGRO_KEY_PAD_3:
            keys[N3] = false;
            break;
        case ALLEGRO_KEY_4:
        case ALLEGRO_KEY_PAD_4:
            keys[N4] = false;
            break;
        case ALLEGRO_KEY_5:
        case ALLEGRO_KEY_PAD_5:
            keys[N5] = false;
            break;
        case ALLEGRO_KEY_6:
        case ALLEGRO_KEY_PAD_6:
            keys[N6] = false;
            break;
        case ALLEGRO_KEY_7:
        case ALLEGRO_KEY_PAD_7:
            keys[N7] = false;
            break;
        case ALLEGRO_KEY_8:
        case ALLEGRO_KEY_PAD_8:
            keys[N8] = false;
            break;
        case ALLEGRO_KEY_9:
        case ALLEGRO_KEY_PAD_9:
            keys[N9] = false;
            break;

        case ALLEGRO_KEY_F1:
            keys[F1] = false;
            break;
        case ALLEGRO_KEY_F2:
            keys[F2] = false;
            break;
        case ALLEGRO_KEY_F3:
            keys[F3] = false;
            break;
        case ALLEGRO_KEY_F4:
            keys[F4] = false;
            break;
        case ALLEGRO_KEY_F5:
            keys[F5] = false;
            break;
        case ALLEGRO_KEY_F6:
            keys[F6] = false;
            break;
        case ALLEGRO_KEY_F7:
            keys[F7] = false;
            break;
        case ALLEGRO_KEY_F8:
            keys[F8] = false;
            break;
        case ALLEGRO_KEY_F9:
            keys[F9] = false;
            break;
        case ALLEGRO_KEY_F10:
            keys[F10] = false;
            break;
        case ALLEGRO_KEY_F11:
            keys[F11] = false;
            break;
        case ALLEGRO_KEY_F12:
            keys[F12] = false;
            break;

        case ALLEGRO_KEY_UP:
            keys[UP] = false;
            break;
        case ALLEGRO_KEY_DOWN:
            keys[DOWN] = false;
            break;
        case ALLEGRO_KEY_RIGHT:
            keys[RIGHT] = false;
            break;
        case ALLEGRO_KEY_LEFT:
            keys[LEFT] = false;
            break;
        case ALLEGRO_KEY_TAB:
            keys[TAB] = false;
            break;
        case ALLEGRO_KEY_LSHIFT:
        case ALLEGRO_KEY_RSHIFT:
            keys[SHIFT] = false;
            break;
        case ALLEGRO_KEY_LCTRL:
        case ALLEGRO_KEY_RCTRL:
            keys[CTRL] = false;
            break;
        case ALLEGRO_KEY_ALT:
            keys[ALT] = false;
            break;
        case ALLEGRO_KEY_ENTER:
        case ALLEGRO_KEY_PAD_ENTER:
            keys[ENTER] = false;
            break;
        case ALLEGRO_KEY_BACKSPACE:
            keys[BACKSPACE] = false;
            break;
        case ALLEGRO_KEY_SPACE:
            keys[SPACE] = false;
            break;
        case ALLEGRO_KEY_ESCAPE:
            keys[ESC] = false;
            break;
        }
    }
}

void saveMap()
{

}


int detectColisionRight_Matriz(struct Players *Player, struct Maps *curMap)
{
    return 0;
}

int detectColisionLeft_Matriz(struct Players *Player, struct Maps *curMap)
{
    return 0;
}

int detectColisionUp_Matriz(struct Players *Player, struct Maps *curMap)
{
    int linhaAtual, colunaAtual;
    struct Bloco curBlock;
    bool result = 0;

    updatePlayer(Player);

    for(linhaAtual = 0; linhaAtual < curMap->numLinhas; linhaAtual++)
    {
        curBlock.y = curMap->y + (curMap->blockHeight * linhaAtual);
        curBlock.y2 = curBlock.y + curMap->blockHeight - 1;

        for(colunaAtual = 0; colunaAtual < curMap->numColunas; colunaAtual++)
        {
            curBlock.x = curMap->x + (curMap->blockWidth * colunaAtual);
            curBlock.x2 = curBlock.x + curMap->blockWidth - 1;

            if((curBlock.x >= 0) && (curBlock.y >= 0) && ((curBlock.x + curMap->blockWidth) <= DISPLAY_WIDTH) && ((curBlock.y + curMap->blockHeight) <= DISPLAY_HEIGHT))
            {
                if((Player->boundy <= curBlock.y2) && (Player->boundy >= curBlock.y))
                {
                    if(((Player->boundx2 >= curBlock.x) && (Player->boundx <= curBlock.x2))
                            || ((Player->boundx2 >= curBlock.x) && (Player->boundx <= curBlock.x2))
                            || ((Player->boundx <= curBlock.x) && (Player->boundx2 >= curBlock.x2)))
                    {
                        switch(curMap->Blocos[linhaAtual][colunaAtual])
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

    if(Player->boundy < 0){
        Player->boundy = 0;
        result = 1;
    }

    return result;
}


int detectColisionDown_Matriz(struct Players *Player, struct Maps *curMap)
{
    int linhaAtual, colunaAtual;
    struct Bloco curBlock;
    bool result = 0;

    updatePlayer(Player);

    for(linhaAtual = 0; linhaAtual < curMap->numLinhas; linhaAtual++)
    {
        curBlock.y = curMap->y + (curMap->blockHeight * linhaAtual);
        curBlock.y2 = curBlock.y + curMap->blockHeight - 1;

        for(colunaAtual = 0; colunaAtual < curMap->numColunas; colunaAtual++)
        {
            curBlock.x = curMap->x + (curMap->blockWidth * colunaAtual);
            curBlock.x2 = curBlock.x + curMap->blockWidth - 1;

            if((curBlock.x >= 0) && (curBlock.y >= 0) && ((curBlock.x + curMap->blockWidth) <= DISPLAY_WIDTH) && ((curBlock.y + curMap->blockHeight) <= DISPLAY_HEIGHT))
            {
                if((Player->boundy2 >= (curBlock.y - 1)) && (Player->boundy2 <= curBlock.y2))
                {
                    if(((Player->boundx2 >= curBlock.x) && (Player->boundx <= curBlock.x2))
                            || ((Player->boundx2 >= curBlock.x) && (Player->boundx <= curBlock.x2))
                            || ((Player->boundx <= curBlock.x) && (Player->boundx2 >= curBlock.x2)))
                    {
                        switch(curMap->Blocos[linhaAtual][colunaAtual])
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

    if(Player->boundy2 > DISPLAY_HEIGHT){
        Player->boundy = DISPLAY_HEIGHT - Player->height + 1;
        result = 1;
    }

    return result;
}


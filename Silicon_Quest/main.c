//--------------------------------------------------
// Inclusão das bibliotecas do Allegro 5
//--------------------------------------------------
#include <allegro5/allegro.h>   // Biblioteca padrão allegro
#include <allegro5/allegro_font.h>  // Biblioteca para utilização de fontes
#include <allegro5/allegro_ttf.h>   // Biblioteca para utilização de fontes
#include <allegro5/allegro_primitives.h>    // Biblioteca para utilização de primitivos
#include <allegro5/allegro_image.h>         // Biblioteca para utilização de bitmaps
#include <allegro5/allegro_native_dialog.h> // Biblioteca para utilização de caixas de texto do SO

//--------------------------------------------------
// Inclusão das bibliotecas padrões C
//--------------------------------------------------
#include <stdio.h>      // Biblioteca utilizada para escrita em arquivo
#include <string.h>     // Biblioteca para a manipulação de Strings
#include <math.h>       // Biblioteca utilizada para calculo da distancia

//--------------------------------------------------
// Inclusão dos headers criados
//--------------------------------------------------
#include "Structs.h"
#include "Colision.h"
#include "Animation.h"
#include "FileIO.h"

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
#define MAX_COLUNAS 2500
#define MAX_LINHAS  2500
#define NUM_BLOCOS  6
#define GRAVITY     0.6
#define JUMP_FORCE  2
#define JUMP_TIME   10

// Definições dos Objetos
#define CHARACTER_WIDTH     50
#define CHARACTER_HEIGHT    100

// Definições das cores
#define COR_BORDAS  242, 210, 99
//#define COR_BORDAS  255, 255, 255
#define COR_LIMITS  255, 255, 255
#define COR_AR      0, 0, 0
#define COR_TERRA   94, 28, 13
#define COR_PEDRA   53, 53, 53
#define COR_SILICIO 249, 249, 249
#define COR_LAVA    255, 116, 21
#define COR_AGUA    0, 128, 255

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
// Definição das variaveis constantes globais
//--------------------------------------------------
const int blockHeight = 50;
const int blockWidth = 50;
const int threshold = 125;

//--------------------------------------------------
// Definição das structs globais
//--------------------------------------------------


//--------------------------------------------------
// Definição das variaveis para INPUT
//--------------------------------------------------
bool keys[62] = {false};
enum keys {A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
           N0, N1, N2, N3, N4, N5, N6, N7, N8, N9,
           F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
           UP, DOWN, RIGHT, LEFT, TAB, SHIFT, CTRL, ALT, ENTER, BACKSPACE, SPACE, ESC, MOUSE_1, MOUSE_2
          };

struct mouseValues mouse = {0, 0, 0, 0, 0, 0};

//--------------------------------------------------
// Definição das variaveis de estado
//--------------------------------------------------
bool done = false;
bool draw = false;
bool movement = false;
bool movementBoost = false;
bool readMenu = false;
bool jumpResistance = false;

int Player_State = 0;
int Player_Dir = 0; //0 = esquerda; 1 = direita

//--------------------------------------------------
// Definição das variaveis gerais globais
//--------------------------------------------------
struct mapValues mapa = {};
struct Object jogador = {((DISPLAY_WIDTH/2)-50), ((DISPLAY_HEIGHT/2)-50), CHARACTER_WIDTH - 2, CHARACTER_HEIGHT - 2,((DISPLAY_WIDTH/2)-50) + CHARACTER_WIDTH, ((DISPLAY_HEIGHT/2)-50) + CHARACTER_HEIGHT, 0, 0};

struct Sprite_Animation running;
struct Sprite_Animation idle;
struct Sprite_Animation dying;
struct Sprite_Animation mining;
struct Sprite_Animation standing;
struct Sprite_Animation blockCracking;

int numColunas = 100;
int numLinhas = 100;

int selectedBlock = 1;

int PickaxeCursor = 0;

//--------------------------------------------------
// Definição das funções utilizadas
//--------------------------------------------------
int checkEvents();
void readInputs();

int main()
{
    //--------------------------------------------------
    // Setup do Allegro
    //--------------------------------------------------

    // Inicialização do allegro
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

    // Setup das Fontes
    ALLEGRO_FONT *arial_24 = al_load_font("arial.ttf", 24, 0);

    // Setup dos Timers
    drawTimer = al_create_timer(1.0 / FPS);
    movementTimer = al_create_timer(1.0 / MOVEMENT_SPEED);
    movementBoostTimer = al_create_timer(1.0 / BOOST_SPEED);
    menuTimer = al_create_timer(1.0 / MENU_SPEED);
    jumpTimer = al_create_timer(1.0 / JUMP_TIME);


    // Setup dos Eventos
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

    //--------------------------------------------------
    // Definição das variaveis de estado
    //--------------------------------------------------
    char selectedOption = 0;
    int gameState = 0;

    //--------------------------------------------------
    // Definição das variaveis gerais
    //--------------------------------------------------
    ALLEGRO_BITMAP *blocoTerra = NULL;
    ALLEGRO_BITMAP *blocoGrama = NULL;
    ALLEGRO_BITMAP *blocoPedra = NULL;
    ALLEGRO_BITMAP *blocoSilicio = NULL;
    ALLEGRO_BITMAP *blocoLava = NULL;
    ALLEGRO_BITMAP *blocoAgua = NULL;

    ALLEGRO_BITMAP *blockCracks = NULL;

    ALLEGRO_BITMAP *RunningMiner = NULL;
    ALLEGRO_BITMAP *IdleMiner = NULL;
    ALLEGRO_BITMAP *StandingMiner = NULL;
    ALLEGRO_BITMAP *MiningMiner = NULL;
    ALLEGRO_BITMAP *DyingMiner = NULL;

    ALLEGRO_BITMAP *pickaxeCursor = NULL;
    ALLEGRO_BITMAP *idleCursor = NULL;

    blocoTerra = al_load_bitmap("Bitmaps/Terra.bmp");
    blocoGrama = al_load_bitmap("Bitmaps/Grama.bmp");
    blocoPedra = al_load_bitmap("Bitmaps/Pedra.bmp");
    blocoSilicio = al_load_bitmap("Bitmaps/Silicio.bmp");
    blocoLava = al_load_bitmap("Bitmaps/Lava.bmp");
    blocoAgua = al_load_bitmap("Bitmaps/Agua.bmp");

    blockCracks = al_load_bitmap("Bitmaps/blockCracks.png");

    RunningMiner = al_load_bitmap("Bitmaps/minerRunning.png");
    IdleMiner = al_load_bitmap("Bitmaps/minerIdle.png");
    StandingMiner = al_load_bitmap("Bitmaps/minerStanding.png");
    MiningMiner = al_load_bitmap("Bitmaps/minerMining.png");
    DyingMiner = al_load_bitmap("Bitmaps/mineDying.png");

    pickaxeCursor = al_load_bitmap("Bitmaps/MineCursor.png");
    idleCursor = al_load_bitmap("Bitmaps/NotMineCursor.png");

    running.maxFrame = 6;
    running.frameDelay = 6;
    running.frameCount = 0;
    running.curFrame = 0;
    running.frameHeight = 80;
    running.frameWidth = 80;

    idle.maxFrame = 4;
    idle.frameDelay = 25;
    idle.frameCount = 0;
    idle.curFrame = 0;
    idle.frameHeight = 80;
    idle.frameWidth = 80;

    dying.maxFrame = 6;
    dying.frameDelay = 25;
    dying.frameCount = 0;
    dying.curFrame = 0;
    dying.frameHeight = 80;
    dying.frameWidth = 80;

    mining.maxFrame = 10;
    mining.frameDelay = 6;
    mining.frameCount = 0;
    mining.curFrame = 0;
    mining.frameHeight = 80;
    mining.frameWidth = 80;

    standing.maxFrame = 1;
    standing.frameDelay = 0;
    standing.frameCount = 0;
    standing.curFrame = 0;
    standing.frameHeight = 80;
    standing.frameWidth = 80;

    blockCracking.maxFrame = 11;
    blockCracking.frameDelay = 150;
    blockCracking.frameCount = 0;
    blockCracking.curFrame = 0;
    blockCracking.frameHeight = 128;
    blockCracking.frameWidth = 128;

    //--------------------------------------------------
    // Definição das variaveis auxiliares
    //--------------------------------------------------
    int j, i;
    bool colisionLeft   = 0;
    bool colisionRight  = 0;
    bool colisionUp     = 0;
    bool colisionDown   = 0;
    bool mine = false;

    //--------------------------------------------------
    // Loop para o jogo
    //--------------------------------------------------
    while(!done)
    {
        al_wait_for_event(event_queue, &ev);
        done = checkEvents();

        if(keys[ESC])
            done = true;

        if(gameState == 0)
        {
            if(draw)
            {
                al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 100, 0, "        NEW MAP");
                al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 125, 0, "        QUIT  ");

                if(selectedOption == 0)
                {
                    al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 100, 0, ">                           <");
                }
                else
                {
                    al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 125, 0, ">                           <");
                }

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));

                draw = false;
            }

            if(readMenu)
            {
                if(keys[UP] | keys[DOWN] | keys[W] | keys[S])
                {
                    selectedOption = !selectedOption;
                    readMenu = false;
                }

                if(keys[ENTER])
                {
                    done = selectedOption;
                    selectedOption = 0;
                    gameState = 1;
                    readMenu = false;
                }
            }


        }

        if(gameState == 1)
        {
            if(draw)
            {
                if(numLinhas == 0)
                    al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 100, 0, "        LINHAS:     _");
                else
                    al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 100, 0, "        LINHAS:     %d_", numLinhas);
                if(numColunas == 0)
                    al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 125, 0, "        COLUNAS:    _");
                else
                    al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 125, 0, "        COLUNAS:    %d_", numColunas);

                if(selectedOption == 0)
                {
                    al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 100, 0, ">                                   <");
                }
                else if (selectedOption == 1)
                {
                    al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 125, 0, ">                                   <");
                }

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));

                draw = false;
            }

            if(readMenu)
            {
                readMenu = false;

                if(selectedOption)
                {
                    if(keys[BACKSPACE])
                        numColunas = (numColunas/10);
                    else if((keys[A] || keys[LEFT]) && (numColunas>1))
                        numColunas--;
                    else if((keys[D] | keys[RIGHT]) && (numColunas < MAX_COLUNAS))
                        numColunas++;
                    else if(keys[N0])
                        numColunas = (numColunas * 10) + 0;
                    else if(keys[N1])
                        numColunas = (numColunas * 10) + 1;
                    else if(keys[N2])
                        numColunas = (numColunas * 10) + 2;
                    else if(keys[N3])
                        numColunas = (numColunas * 10) + 3;
                    else if(keys[N4])
                        numColunas = (numColunas * 10) + 4;
                    else if(keys[N5])
                        numColunas = (numColunas * 10) + 5;
                    else if(keys[N6])
                        numColunas = (numColunas * 10) + 6;
                    else if(keys[N7])
                        numColunas = (numColunas * 10) + 7;
                    else if(keys[N8])
                        numColunas = (numColunas * 10) + 8;
                    else if(keys[N9])
                        numColunas = (numColunas * 10) + 9;
                    else if(keys[UP] | keys[DOWN] | keys[W] | keys[S])
                        selectedOption = !selectedOption;
                    else
                        readMenu = true;
                    if(numColunas > MAX_COLUNAS)
                        numColunas = MAX_COLUNAS;
                }
                else
                {
                    if(keys[BACKSPACE])
                        numLinhas = (numLinhas/10);
                    else if((keys[A] | keys[LEFT]) && (numLinhas > 1))
                        numLinhas--;
                    else if((keys[D] | keys[RIGHT]) && (numLinhas < MAX_LINHAS))
                        numLinhas++;
                    else if(keys[N0])
                        numLinhas = (numLinhas * 10) + 0;
                    else if(keys[N1])
                        numLinhas = (numLinhas * 10) + 1;
                    else if(keys[N2])
                        numLinhas = (numLinhas * 10) + 2;
                    else if(keys[N3])
                        numLinhas = (numLinhas * 10) + 3;
                    else if(keys[N4])
                        numLinhas = (numLinhas * 10) + 4;
                    else if(keys[N5])
                        numLinhas = (numLinhas * 10) + 5;
                    else if(keys[N6])
                        numLinhas = (numLinhas * 10) + 6;
                    else if(keys[N7])
                        numLinhas = (numLinhas * 10) + 7;
                    else if(keys[N8])
                        numLinhas = (numLinhas * 10) + 8;
                    else if(keys[N9])
                        numLinhas = (numLinhas * 10) + 9;
                    else if(keys[UP] | keys[DOWN] | keys[W] | keys[S])
                        selectedOption = !selectedOption;
                    else
                        readMenu = true;
                    if(numLinhas > MAX_LINHAS)
                        numLinhas = MAX_LINHAS;
                }

                if(keys[ENTER])
                {
                    mapa.x = ((DISPLAY_WIDTH/2) - ((numColunas/2) * blockWidth));
                    mapa.y = ((DISPLAY_HEIGHT/2) - ((numLinhas/2) * blockHeight));
                    gameState = 2;
                }
            }

        }

        if(gameState == 2)
        {
            // READ MOUSE MOVEMENT (TO BLOCK LIMITS)
            if(mouse.x < 0)
                mouse.x = 0;
            if(mouse.y < 0)
                mouse.y = 0;

            if(mouse.x < mapa.x)
                mouse.x = mapa.x;
            if(mouse.y < mapa.y)
                mouse.y = mapa.y;

            if(mouse.x > DISPLAY_WIDTH)
                mouse.x = DISPLAY_WIDTH;
            if(mouse.y > DISPLAY_HEIGHT)
                mouse.y = DISPLAY_HEIGHT;

            if(mouse.x >= (mapa.x + (numColunas * blockWidth)))
                mouse.x = mapa.x + (numColunas * blockWidth) - 1;
            if(mouse.y >= (mapa.y + (numLinhas * blockHeight)))
                mouse.y = mapa.y + (numLinhas * blockHeight) - 1;

            mouseBlock.coluna = (mouse.x - mapa.x)/blockWidth;
            mouseBlock.linha = (mouse.y - mapa.y)/blockHeight;

            // READ MOUSE WHEEL MOVEMENT
            if(mouse.z > mouseWheelBefore)
            {
                mouseWheelBefore = mouse.z;
                selectedBlock++;
                if(selectedBlock >= NUM_BLOCOS)
                    selectedBlock = 1;
            }
            else if(mouse.z < mouseWheelBefore)
            {
                mouseWheelBefore = mouse.z;
                selectedBlock--;
                if(selectedBlock < 1)
                    selectedBlock = NUM_BLOCOS - 1;
            }

            if(keys[MOUSE_1])
                blocos[mouseBlock.linha][mouseBlock.coluna] = selectedBlock;
            if(keys[MOUSE_2])
                blocos[mouseBlock.linha][mouseBlock.coluna] = 0;

            if(movement)
            {
                // READ MOVEMENT KEYS (WASD + ARROWS)
                if((keys[LEFT] || keys[A]) && (mapa.x < 0))
                    mapa.x += MOVEMENT_STEP + (2 * keys[SHIFT]);
                if((keys[RIGHT] || keys[D]) && (((mapa.x + (numColunas * blockWidth)) > DISPLAY_WIDTH)))
                    mapa.x -= MOVEMENT_STEP + (2 * keys[SHIFT]);

                if((keys[UP] || keys[W]) && (mapa.y < 0))
                    mapa.y += MOVEMENT_STEP + (2 * keys[SHIFT]);
                if((keys[DOWN] || keys[S]) && (((mapa.y + (numLinhas * blockHeight)) > DISPLAY_HEIGHT)))
                    mapa.y -= MOVEMENT_STEP + (2 * keys[SHIFT]);

                movement = false;
            }

            if(keys[P])
                gameState = 3;
            if(draw)
            {
                draw = false;
                for(i = 0; i < numLinhas; i++)
                {
                    for(j = 0; j < numColunas; j++)
                        if(((mapa.x + (j * blockWidth) + blockWidth) >= 0)&&((mapa.y + (i * blockHeight) + blockHeight)>= 0)&&((mapa.x + j * blockWidth) < DISPLAY_WIDTH)&&((mapa.y + i * blockHeight) < DISPLAY_HEIGHT))
                        {
                            switch(blocos[i][j])
                            {
                            case 0: // AR
                                al_draw_filled_rectangle(mapa.x + j * blockWidth, mapa.y + i * blockHeight, mapa.x + (j * blockWidth) + blockWidth, mapa.y + (i * blockHeight) + blockHeight, al_map_rgb(COR_AR));
                                break;
                            case 1: // TERRA
                                if(blocos[i-1][j] == 0)
                                    al_draw_scaled_bitmap(blocoGrama, 0, 0, source.x, source.y, mapa.x + j * blockWidth, mapa.y + i * blockHeight, blockWidth, blockHeight, 0);
                                else
                                    al_draw_scaled_bitmap(blocoTerra, 0, 0, source.x, source.y, mapa.x + j * blockWidth, mapa.y + i * blockHeight, blockWidth, blockHeight, 0);
                                break;
                            case 2: // PEDRA
                                al_draw_scaled_bitmap(blocoPedra, 0, 0, source.x, source.y, mapa.x + j * blockWidth, mapa.y + i * blockHeight, blockWidth, blockHeight, 0);
                                break;
                            case 3: // SILICIO
                                al_draw_scaled_bitmap(blocoSilicio, 0, 0, source.x, source.y, mapa.x + j * blockWidth, mapa.y + i * blockHeight, blockWidth, blockHeight, 0);
                                break;
                            case 4: // LAVA
                                al_draw_scaled_bitmap(blocoLava, 0, 0, source.x, source.y, mapa.x + j * blockWidth, mapa.y + i * blockHeight, blockWidth, blockHeight, 0);
                                break;
                            case 5: // AGUA
                                al_draw_scaled_bitmap(blocoAgua, 0, 0, source.x, source.y, mapa.x + j * blockWidth, mapa.y + i * blockHeight, blockWidth, blockHeight, 0);
                                break;
                            }
                        }
                }

                // DRAW BORDERS
                if(SHOW_BORDER)
                {
                    al_draw_rectangle(mapa.x + mouseBlock.coluna * blockWidth, mapa.y + mouseBlock.linha * blockHeight, mapa.x + (mouseBlock.coluna * blockWidth) + blockWidth, mapa.y + (mouseBlock.linha * blockHeight) + blockHeight, al_map_rgb(COR_BORDAS), 1);
                    al_draw_rectangle(1, 1, DISPLAY_WIDTH, DISPLAY_HEIGHT, al_map_rgb(COR_BORDAS), 1);
                    if(SHOW_MAP_LIMITS)
                        al_draw_rectangle(mapa.x, mapa.y, (mapa.x + (numColunas * blockWidth)), (mapa.y + (numLinhas * blockHeight)), al_map_rgb(COR_LIMITS), 1);
                }

                // DRAW SELECTED BLOCK PREVIEW
                switch(selectedBlock)
                {
                case 1: // TERRA
                    if(blocos[i-1][j] == 0)
                        al_draw_scaled_bitmap(blocoGrama, 0, 0, source.x, source.y, DISPLAY_WIDTH - (10 + blockWidth), 10, blockWidth, blockHeight, 0);
                    else
                        al_draw_scaled_bitmap(blocoTerra, 0, 0, source.x, source.y, DISPLAY_WIDTH - (10 + blockWidth), 10, blockWidth, blockHeight, 0);
                    break;
                case 2: // PEDRA
                    al_draw_scaled_bitmap(blocoPedra, 0, 0, source.x, source.y, DISPLAY_WIDTH - (10 + blockWidth), 10, blockWidth, blockHeight, 0);
                    break;
                case 3: // SILICIO
                    al_draw_scaled_bitmap(blocoSilicio, 0, 0, source.x, source.y, DISPLAY_WIDTH - (10 + blockWidth), 10, blockWidth, blockHeight, 0);
                    break;
                case 4: // LAVA
                    al_draw_scaled_bitmap(blocoLava, 0, 0, source.x, source.y, DISPLAY_WIDTH - (10 + blockWidth), 10, blockWidth, blockHeight, 0);
                    break;
                case 5: // AGUA
                    al_draw_scaled_bitmap(blocoAgua, 0, 0, source.x, source.y, DISPLAY_WIDTH - (10 + blockWidth), 10, blockWidth, blockHeight, 0);
                    break;
                }

                if(SHOW_BORDER)
                    al_draw_rectangle(DISPLAY_WIDTH - (10 + blockWidth), 10, DISPLAY_WIDTH - 10, 10 + blockHeight, al_map_rgb(COR_BORDAS), 1);

                //FLIP BUFFERS========================
                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }
        }

        if(gameState == 3)
        {
            if(draw)
            {
                al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 100, 0, "        CONTINUE");
                al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 125, 0, "        SAVE MAP");
                al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 150, 0, "        TEST MAP");
                al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 175, 0, "        QUIT  ");

                if(selectedOption == 0)
                {
                    al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 100, 0, ">                           <");
                }
                else if(selectedOption == 1)
                {
                    al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 125, 0, ">                           <");
                }
                else if(selectedOption == 2)
                {
                    al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 150, 0, ">                           <");
                }
                else
                {
                    al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 100, 175, 0, ">                           <");
                }

                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));

                draw = false;
            }

            if(readMenu)
            {
                readMenu = false;

                if(keys[UP] | keys[W])
                    selectedOption--;
                else if(keys[DOWN] | keys[S])
                    selectedOption++;
                else if(keys[ENTER])
                {
                    if(selectedOption == 0)
                        gameState = 2;
                    else if(selectedOption == 1)
                        saveMap();
                    else if(selectedOption == 2)
                        gameState = 4;
                    else
                        done = true;
                    selectedOption = 0;
                    keys[ENTER] = false;
                }
                else
                    readMenu = true;

                if(selectedOption > 3)
                    selectedOption = 0;

                if(selectedOption < 0)
                    selectedOption = 3;
            }
        }

        if(gameState == 4)  // Gamestate de teste
        {
            colisionLeft = detectColisionLeft_Matriz(jogador, mapa, blocos);
            colisionRight = detectColisionRight_Matriz(jogador, mapa, blocos);
            colisionUp = detectColisionUp_Matriz(jogador, mapa, blocos);
            colisionDown = detectColisionDown_Matriz(jogador, mapa, blocos);

            // READ MOUSE MOVEMENT (TO BLOCK LIMITS)
            if(mouse.x < 0)
                mouse.x = 0;
            if(mouse.y < 0)
                mouse.y = 0;

            if(mouse.x < mapa.x)
                mouse.x = mapa.x;
            if(mouse.y < mapa.y)
                mouse.y = mapa.y;

            if(mouse.x > DISPLAY_WIDTH)
                mouse.x = DISPLAY_WIDTH;
            if(mouse.y > DISPLAY_HEIGHT)
                mouse.y = DISPLAY_HEIGHT;

            if(mouse.x >= (mapa.x + (numColunas * blockWidth)))
                mouse.x = mapa.x + (numColunas * blockWidth) - 1;
            if(mouse.y >= (mapa.y + (numLinhas * blockHeight)))
                mouse.y = mapa.y + (numLinhas * blockHeight) - 1;

            mouseBlock.coluna = (mouse.x - mapa.x)/blockWidth;
            mouseBlock.linha = (mouse.y - mapa.y)/blockHeight;

            // READ MOUSE WHEEL MOVEMENT
            if(mouse.z > mouseWheelBefore)
            {
                mouseWheelBefore = mouse.z;
                selectedBlock++;
                if(selectedBlock >= NUM_BLOCOS)
                    selectedBlock = 1;
            }
            else if(mouse.z < mouseWheelBefore)
            {
                mouseWheelBefore = mouse.z;
                selectedBlock--;
                if(selectedBlock < 1)
                    selectedBlock = NUM_BLOCOS - 1;
            }


            if((movement && !keys[SHIFT])||(movementBoost && keys[SHIFT]))
            {
                if(!(keys[LEFT] || keys[A]) && !(keys[RIGHT] || keys[D]) && !(keys[UP] || keys[W]) && !(keys[DOWN] || keys[S]))
                {
                    Player_State=0;
                }
                else if((keys[LEFT] || keys[A]) && (keys[RIGHT] || keys[D]))
                {
                    Player_State=0;
                }
                else if((keys[A] || keys[LEFT]) && (colisionLeft == 0) && (jogador.x > 0))
                {
                    Player_State = 1;

                    Player_Dir = 0;

                    jogador.x -= MOVEMENT_STEP;
                }
                else if((keys[D] || keys[RIGHT]) && (colisionRight == 0) && ((jogador.x + jogador.width) < DISPLAY_WIDTH))
                {
                    Player_State = 2;

                    Player_Dir = 1;

                    jogador.x += MOVEMENT_STEP;
                }

                jogador.jump = !colisionDown;

                if((keys[W] || keys[UP]) && (colisionUp == 0) && (jogador.y > 0) && !jogador.jump)
                {
                    jogador.jump = true;
                    jogador.force = JUMP_FORCE;
                }

                if(jogador.jump)
                {
                    jogador.y -= jogador.force;
                }

                colisionDown = detectColisionDown_Matriz(jogador, mapa, blocos);
                colisionUp = detectColisionUp_Matriz(jogador, mapa, blocos);

                if(colisionDown || colisionUp)
                    jogador.force = 0;


                if(jogador.force < -10)
                    jogador.force = -10;

                //if((keys[W] || keys[UP]) && (colisionUp == 0) && (jogador.y > 0))
                //jogador.y -= MOVEMENT_STEP;
                if((keys[S] || keys[DOWN]) && (colisionDown == 0) && ((jogador.y + jogador.height) < DISPLAY_HEIGHT))
                    jogador.y += MOVEMENT_STEP;
            }

            if(colisionLeft)
            {
                Player_Dir = 0;
                Player_State = 0;
            }
            if(colisionRight)
            {
                Player_Dir = 1;
                Player_State = 0;
            }


            if(threshold > CheckDistance((jogador.x + 23), (jogador.y + 50), (mapa.x + (mouseBlock.coluna * blockWidth)), (mapa.y + (mouseBlock.linha * blockHeight)))
                    || (threshold > CheckDistance((jogador.x + 23), (jogador.y + 50), (mapa.x + blockWidth + (mouseBlock.coluna * blockWidth)), (mapa.y + (mouseBlock.linha * blockHeight))))
                    || (threshold > CheckDistance((jogador.x + 23), (jogador.y + 50), (mapa.x + (mouseBlock.coluna * blockWidth)), (mapa.y + blockHeight + (mouseBlock.linha * blockHeight))))
                    || (threshold > CheckDistance((jogador.x + 23), (jogador.y + 50), (mapa.x + blockWidth + (mouseBlock.coluna * blockWidth)), (mapa.y + blockHeight + (mouseBlock.linha * blockHeight)))))
            {
                PickaxeCursor = 1;
                if(keys[MOUSE_1] && !(keys[LEFT] || keys[A] || keys[RIGHT] || keys[D] || keys[UP] || keys[W] || keys[DOWN] || keys[S]))
                {
                    if(blocos[mouseBlock.linha][mouseBlock.coluna] != 0)
                    {
                        if(++blockCracking.frameCount >= blockCracking.frameDelay)
                        {
                            blockCracking.frameCount = 0;
                            blockCracking.curFrame++;

                            if(blockCracking.curFrame >= blockCracking.maxFrame)
                                blockCracking.curFrame = 0;

                            if(blockCracking.curFrame == 0)
                                blocos[mouseBlock.linha][mouseBlock.coluna] = 0;
                        }
                        Player_State = 4;

                        if((mapa.x + (mouseBlock.coluna * blockWidth)) < jogador.x)
                            Player_Dir = 0;
                        else if((mapa.x + (mouseBlock.coluna * blockWidth)) > jogador.x)
                            Player_Dir = 1;
                    }
                }else
                {
                    blockCracking.frameCount = 0;
                    blockCracking.curFrame = 0;
                }
            }
            else
            {
                PickaxeCursor = 0;

            }
            if(keys[MOUSE_2])
                blocos[mouseBlock.linha][mouseBlock.coluna] = selectedBlock;


            if(jumpResistance)
            {
                if(jogador.jump)
                {
                    jogador.force -= GRAVITY;
                }

                jumpResistance = false;
            }

            if(jogador.y < 0)
                jogador.y = 0;
            if(jogador.x < 0)
                jogador.x = 0;
            if((jogador.y + jogador.height) > DISPLAY_HEIGHT)
                jogador.y = DISPLAY_HEIGHT - jogador.height;
            if((jogador.x + jogador.width) > DISPLAY_WIDTH)
                jogador.x = DISPLAY_WIDTH - jogador.width;


            colisionLeft = detectColisionLeft_Matriz(jogador, mapa, blocos);
            colisionRight = detectColisionRight_Matriz(jogador, mapa, blocos);
            colisionUp = detectColisionUp_Matriz(jogador, mapa, blocos);
            colisionDown = detectColisionDown_Matriz(jogador, mapa, blocos);

            colisionUp = detectColisionUp_Matriz(jogador, mapa, blocos);
            colisionDown = detectColisionDown_Matriz(jogador, mapa, blocos);

            if(colisionUp)
                jogador.y = (((jogador.y - mapa.y)/blockHeight) * blockHeight) + mapa.y + blockHeight;
            if(colisionDown)
                jogador.y -= 1 + ((jogador.y - mapa.y) % blockHeight) - (blockHeight - (jogador.height % blockHeight));

            if((mapa.x <= 0) && (jogador.x <= ((DISPLAY_WIDTH/2) - (jogador.width/2))))
            {
                mapa.x += ((DISPLAY_WIDTH/2) - (jogador.width/2)) - jogador.x;
                jogador.x = ((DISPLAY_WIDTH/2) - (jogador.width/2));
            }

            if((mapa.y <= 0) && (jogador.y <= ((DISPLAY_HEIGHT/2) - (jogador.height/2))))
            {
                mapa.y += ((DISPLAY_HEIGHT/2) - (jogador.height/2)) - jogador.y;
                jogador.y = ((DISPLAY_HEIGHT/2) - (jogador.height/2));
            }

            if(((mapa.x + (numColunas * blockWidth)) > DISPLAY_WIDTH) && (jogador.x >= ((DISPLAY_WIDTH/2) - (jogador.width/2))))
            {
                mapa.x += ((DISPLAY_WIDTH/2) - (jogador.width/2)) - jogador.x;
                jogador.x = ((DISPLAY_WIDTH/2) - (jogador.width/2));
            }

            if(((mapa.y + (numLinhas * blockHeight)) > DISPLAY_HEIGHT) && (jogador.y >= ((DISPLAY_HEIGHT/2) - (jogador.height/2))))
            {
                mapa.y += ((DISPLAY_HEIGHT/2) - (jogador.height/2)) - jogador.y;
                jogador.y = ((DISPLAY_HEIGHT/2) - (jogador.height/2));
            }

            movement = false;
            movementBoost = false;

            if(keys[P])
                gameState = 3;
            if(draw)
            {
                draw = false;

                for(i = 0; i < numLinhas; i++)
                {
                    for(j = 0; j < numColunas; j++)
                        if(((mapa.x + (j * blockWidth) + blockWidth) >= 0)&&((mapa.y + (i * blockHeight) + blockHeight)>= 0)&&((mapa.x + j * blockWidth) < DISPLAY_WIDTH)&&((mapa.y + i * blockHeight) < DISPLAY_HEIGHT))
                        {
                            switch(blocos[i][j])
                            {
                            case 0: // AR
                                al_draw_filled_rectangle(mapa.x + j * blockWidth, mapa.y + i * blockHeight, mapa.x + (j * blockWidth) + blockWidth, mapa.y + (i * blockHeight) + blockHeight, al_map_rgb(COR_AR));
                                break;
                            case 1: // TERRA
                                if(blocos[i-1][j] == 0)
                                    al_draw_scaled_bitmap(blocoGrama, 0, 0, source.x, source.y, mapa.x + j * blockWidth, mapa.y + i * blockHeight, blockWidth, blockHeight, 0);
                                else
                                    al_draw_scaled_bitmap(blocoTerra, 0, 0, source.x, source.y, mapa.x + j * blockWidth, mapa.y + i * blockHeight, blockWidth, blockHeight, 0);
                                break;
                            case 2: // PEDRA
                                al_draw_scaled_bitmap(blocoPedra, 0, 0, source.x, source.y, mapa.x + j * blockWidth, mapa.y + i * blockHeight, blockWidth, blockHeight, 0);
                                break;
                            case 3: // SILICIO
                                al_draw_scaled_bitmap(blocoSilicio, 0, 0, source.x, source.y, mapa.x + j * blockWidth, mapa.y + i * blockHeight, blockWidth, blockHeight, 0);
                                break;
                            case 4: // LAVA
                                al_draw_scaled_bitmap(blocoLava, 0, 0, source.x, source.y, mapa.x + j * blockWidth, mapa.y + i * blockHeight, blockWidth, blockHeight, 0);
                                break;
                            case 5: // AGUA
                                al_draw_scaled_bitmap(blocoAgua, 0, 0, source.x, source.y, mapa.x + j * blockWidth, mapa.y + i * blockHeight, blockWidth, blockHeight, 0);
                                break;
                            }
                        }
                }

                Animation(Player_State);

                //al_draw_filled_rectangle(jogador.x, jogador.y, jogador.x + jogador.width, jogador.y + jogador.height, al_map_rgb(COR_LIMITS));

                // DRAW BORDERS
                if(SHOW_BORDER)
                {
                    /*al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 50, 50, 0, "jogador.jump = %d", jogador.jump);
                    al_draw_textf(arial_24, al_map_rgb(COR_BORDAS), 50, 75, 0, "Colision Down = %d", colisionDown);*/


                    al_draw_rectangle(mapa.x + mouseBlock.coluna * blockWidth, mapa.y + mouseBlock.linha * blockHeight, mapa.x + (mouseBlock.coluna * blockWidth) + blockWidth, mapa.y + (mouseBlock.linha * blockHeight) + blockHeight, al_map_rgb(COR_BORDAS), 1);
                    al_draw_rectangle(1, 1, DISPLAY_WIDTH, DISPLAY_HEIGHT, al_map_rgb(COR_BORDAS), 1);
                    if(SHOW_MAP_LIMITS)
                        al_draw_rectangle(mapa.x, mapa.y, (mapa.x + (numColunas * blockWidth)), (mapa.y + (numLinhas * blockHeight)), al_map_rgb(COR_LIMITS), 1);
                }

                // DRAW SELECTED BLOCK PREVIEW
                switch(selectedBlock)
                {
                case 1: // TERRA
                    if(blocos[i-1][j] == 0)
                        al_draw_scaled_bitmap(blocoGrama, 0, 0, source.x, source.y, DISPLAY_WIDTH - (10 + blockWidth), 10, blockWidth, blockHeight, 0);
                    else
                        al_draw_scaled_bitmap(blocoTerra, 0, 0, source.x, source.y, DISPLAY_WIDTH - (10 + blockWidth), 10, blockWidth, blockHeight, 0);
                    break;
                case 2: // PEDRA
                    al_draw_scaled_bitmap(blocoPedra, 0, 0, source.x, source.y, DISPLAY_WIDTH - (10 + blockWidth), 10, blockWidth, blockHeight, 0);
                    break;
                case 3: // SILICIO
                    al_draw_scaled_bitmap(blocoSilicio, 0, 0, source.x, source.y, DISPLAY_WIDTH - (10 + blockWidth), 10, blockWidth, blockHeight, 0);
                    break;
                case 4: // LAVA
                    al_draw_scaled_bitmap(blocoLava, 0, 0, source.x, source.y, DISPLAY_WIDTH - (10 + blockWidth), 10, blockWidth, blockHeight, 0);
                    break;
                case 5: // AGUA
                    al_draw_scaled_bitmap(blocoAgua, 0, 0, source.x, source.y, DISPLAY_WIDTH - (10 + blockWidth), 10, blockWidth, blockHeight, 0);
                    break;
                }

                if(Player_State == 0 && Player_Dir == 0)
                {
                    al_draw_scaled_bitmap(StandingMiner, standing.curFrame * standing.frameWidth, 0, standing.frameWidth, standing.frameHeight, jogador.x - 78, jogador.y - 75, standing.frameWidth * 2.5, standing.frameHeight * 2.5,0);
                }

                if(Player_State == 0 && Player_Dir == 1)
                {
                    al_draw_scaled_bitmap(StandingMiner, standing.curFrame * standing.frameWidth, 0, standing.frameWidth, standing.frameHeight, jogador.x - 72, jogador.y - 75, standing.frameWidth * 2.5, standing.frameHeight * 2.5, ALLEGRO_FLIP_HORIZONTAL);
                }

                if(Player_State == 1)
                {
                    al_draw_scaled_bitmap(RunningMiner, running.curFrame * running.frameWidth, 0, running.frameWidth, running.frameHeight, jogador.x - 70, jogador.y - 75, running.frameWidth * 2.5, running.frameHeight * 2.5,0);
                }

                if(Player_State == 2)
                {
                    al_draw_scaled_bitmap(RunningMiner, running.curFrame * running.frameWidth, 0, running.frameWidth, running.frameHeight, jogador.x - 70, jogador.y - 75, running.frameWidth * 2.5, running.frameHeight * 2.5, ALLEGRO_FLIP_HORIZONTAL);
                }

                if(Player_State == 3)
                {
                    al_draw_scaled_bitmap(IdleMiner, idle.curFrame * idle.frameWidth, 0, idle.frameWidth, idle.frameHeight, jogador.x - 70, jogador.y - 75, idle.frameWidth * 2.5, idle.frameHeight * 2.5,0);
                }

                if(Player_State == 4 && Player_Dir == 0)
                {
                    al_draw_scaled_bitmap(MiningMiner, mining.curFrame * mining.frameWidth, 0, mining.frameWidth, mining.frameHeight, jogador.x - 70, jogador.y - 75, mining.frameWidth * 2.5, mining.frameHeight * 2.5,0);
                }

                if(Player_State == 4 && Player_Dir == 1)
                {
                    al_draw_scaled_bitmap(MiningMiner, mining.curFrame * mining.frameWidth, 0, mining.frameWidth, mining.frameHeight, jogador.x - 70, jogador.y - 75, mining.frameWidth * 2.5, mining.frameHeight * 2.5, ALLEGRO_FLIP_HORIZONTAL);
                }

                if(Player_State == 5)
                {
                    al_draw_scaled_bitmap(DyingMiner, dying.curFrame * dying.frameWidth, 0, dying.frameWidth, dying.frameHeight, jogador.x - 70, jogador.y - 75, dying.frameWidth * 2.5, dying.frameHeight * 2.5,0);
                }

                if(mine = true)
                {
                    al_draw_scaled_bitmap(blockCracks, blockCracking.curFrame * blockCracking.frameWidth, 0, blockCracking.frameWidth, blockCracking.frameHeight, (mapa.x + (mouseBlock.coluna * blockWidth)), (mapa.y + (mouseBlock.linha * blockHeight)), blockWidth, blockHeight,0);
                }

                if(PickaxeCursor == 1)
                    al_draw_bitmap(pickaxeCursor, mouse.x, (mouse.y + 6), ALLEGRO_FLIP_HORIZONTAL);
                if(PickaxeCursor == 0)
                    al_draw_bitmap(idleCursor, (mouse.x - 16), (mouse.y - 6), 0);

                if(SHOW_BORDER)
                    al_draw_rectangle(DISPLAY_WIDTH - (10 + blockWidth), 10, DISPLAY_WIDTH - 10, 10 + blockHeight, al_map_rgb(COR_BORDAS), 1);

                //FLIP BUFFERS========================
                al_flip_display();
                al_clear_to_color(al_map_rgb(0,0,0));
            }
        }

        // Reset timers
        if(!readMenu)
            al_start_timer(menuTimer);
    }

//--------------------------------------------------
// Finalização do Allegro
//--------------------------------------------------

    al_destroy_bitmap(blocoTerra);
    al_destroy_bitmap(blocoGrama);
    al_destroy_bitmap(blocoPedra);
    al_destroy_bitmap(blocoSilicio);
    al_destroy_bitmap(blocoLava);
    al_destroy_bitmap(blocoAgua);
    al_destroy_bitmap(RunningMiner);
    al_destroy_bitmap(StandingMiner);
    al_destroy_bitmap(MiningMiner);
    al_destroy_bitmap(DyingMiner);

    al_destroy_event_queue(event_queue);
    al_destroy_timer(drawTimer);
    al_destroy_timer(movementTimer);
    al_destroy_timer(movementBoostTimer);
    al_destroy_timer(menuTimer);
    al_destroy_timer(jumpTimer);
    al_destroy_display(display);

    return 0;
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
        mouse.z = ev.mouse.z;
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

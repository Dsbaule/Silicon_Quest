/*
    Projeto: Silicon Quest Map Creator
    Alunos: Daniel de Souza Baulé
            Stefano Bergamini Poletto
*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define WIDTH   800
#define HEIGHT  600

#define COLUNAS 250
#define LINHAS  100

#define COR_AR      0, 0, 0
#define COR_TERRA   94, 28, 13
#define COR_PEDRA   53, 53, 53
#define COR_SILICIO 249, 249, 249
#define COR_LAVA    255, 116, 21

//==============================================
//GLOBALS
//==============================================

bool keys[] = {false, false, false, false, false};
enum KEYS {UP, DOWN, LEFT, RIGHT, SPACE};

char matriz[LINHAS][COLUNAS] = {{0}};

const int blockHeight = 50;
const int blockWidth = 50;

int pos_matriz_x = 0;
int pos_matriz_y = 0;

bool done = false;
bool draw = true;
int pos_mouse_x = WIDTH / 2;
int pos_mouse_y = HEIGHT / 2;
int coluna;
int linha;

int main(void)
{
    //==============================================
    //SHELL VARIABLES
    //==============================================
    bool done = false;
    bool render = false;

    float gameTime = 0;
    int frames = 0;
    int gameFPS = 0;

    //==============================================
    //PROJECT VARIABLES
    //==============================================

    //==============================================
    //ALLEGRO VARIABLES
    //==============================================
    ALLEGRO_DISPLAY *display = NULL;
    ALLEGRO_EVENT_QUEUE *event_queue = NULL;
    ALLEGRO_TIMER *timer;
    ALLEGRO_FONT *font18;

    //==============================================
    //ALLEGRO INIT FUNCTIONS
    //==============================================
    if(!al_init())										//initialize Allegro
        return -1;

    display = al_create_display(WIDTH, HEIGHT);			//create our display object

    if(!display)										//test display object
        return -1;

    //==============================================
    //ADDON INSTALL
    //==============================================
    al_install_keyboard();pos_matriz_x -= keys[RIGHT] * 10;
    al_install_mouse();
    al_init_image_addon();
    al_init_font_addon();
    al_init_ttf_addon();
    al_init_primitives_addon();

    //==============================================
    //PROJECT INIT
    //==============================================
    font18 = al_load_font("arial.ttf", 18, 0);

    //==============================================
    //TIMER INIT AND STARTUP
    //==============================================
    event_queue = al_create_event_queue();
    timer = al_create_timer(1.0 / 60);

    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());

    al_start_timer(timer);
    gameTime = al_current_time();
    //al_hide_mouse_cursor(display);
    while(!done)
    {
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);

        //==============================================
        //INPUT
        //==============================================
        if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
        {
            done = true;
        }
        if(ev.type == ALLEGRO_EVENT_MOUSE_AXES)
        {
            pos_mouse_x = ev.mouse.x;
            pos_mouse_y = ev.mouse.y;
        }
        if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
        {
            if(ev.mouse.button & 1)
            {
                coluna = (pos_mouse_x - pos_matriz_x) / blockWidth;
                linha = (pos_mouse_y - pos_matriz_y) / blockHeight;
                matriz[linha][coluna]++;
                if(matriz[linha][coluna] > 4)
                {
                    matriz[linha][coluna] = 1;
                }
            }

            if(ev.mouse.button & 2)
            {
                coluna = (pos_mouse_x - pos_matriz_x) / blockWidth;
                linha = (pos_mouse_y - pos_matriz_y) / blockHeight;
                matriz[linha][coluna] = 0;
            }
        }

        if(ev.type == ALLEGRO_EVENT_KEY_DOWN)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = true;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = true;
                break;
            case ALLEGRO_KEY_UP:
                keys[UP] = true;
                break;
            case ALLEGRO_KEY_DOWN:
                keys[DOWN] = true;
                break;
            case ALLEGRO_KEY_SPACE:
                keys[SPACE] = true;
                break;
            }
        }
        if(ev.type == ALLEGRO_EVENT_KEY_UP)
        {
            switch(ev.keyboard.keycode)
            {
            case ALLEGRO_KEY_ESCAPE:
                done = true;
                break;
            case ALLEGRO_KEY_LEFT:
                keys[LEFT] = false;
                break;
            case ALLEGRO_KEY_RIGHT:
                keys[RIGHT] = false;
                break;
            case ALLEGRO_KEY_UP:
                keys[UP] = false;
                break;
            case ALLEGRO_KEY_DOWN:
                keys[DOWN] = false;
                break;
            case ALLEGRO_KEY_SPACE:
                keys[SPACE] = false;
                break;
            }

        }

        //==============================================
        //GAME UPDATE
        //==============================================
        if(ev.type == ALLEGRO_EVENT_TIMER)
        {
            render = true;

            //UPDATE FPS===========
            frames++;
            if(al_current_time() - gameTime >= 1)
            {
                gameTime = al_current_time();
                gameFPS = frames;
                frames = 0;
            }
            //=====================

            if(!(pos_matriz_x >= 0))
				pos_matriz_x += keys[LEFT] * 10;
			if(!(pos_matriz_y >= 0))
				pos_matriz_y += keys[UP] * 10;
			if(!(pos_matriz_x <= ((-COLUNAS * blockWidth) + WIDTH)))
				pos_matriz_x -= keys[RIGHT] * 10;
			if(!(pos_matriz_y <= ((-LINHAS * blockHeight) + HEIGHT)))
				pos_matriz_y -= keys[DOWN] * 10;
        }

        //==============================================
        //RENDER
        //==============================================
        if(render && al_is_event_queue_empty(event_queue))
        {
            render = false;


            //BEGIN PROJECT RENDER================
            int j, i;
            for(i = 0; i < LINHAS; i++)
            {
                for(j = 0; j < COLUNAS; j++)
                    switch(matriz[i][j])
                    {
                    case 0: // AR
                        al_draw_filled_rectangle(pos_matriz_x + j * blockWidth, pos_matriz_y + i * blockHeight, pos_matriz_x + (j * blockWidth) + blockWidth, pos_matriz_y + (i * blockHeight) + blockHeight, al_map_rgb(COR_AR));
                        break;
                    case 1: // TERRA
                        al_draw_filled_rectangle(pos_matriz_x + j * blockWidth, pos_matriz_y + i * blockHeight, pos_matriz_x + (j * blockWidth) + blockWidth, pos_matriz_y + (i * blockHeight) + blockHeight, al_map_rgb(COR_TERRA));
                        break;
                    case 2: // PEDRA
                        al_draw_filled_rectangle(pos_matriz_x + j * blockWidth, pos_matriz_y + i * blockHeight, pos_matriz_x + (j * blockWidth) + blockWidth, pos_matriz_y + (i * blockHeight) + blockHeight, al_map_rgb(COR_PEDRA));
                        break;
                    case 3: // SILICIO
                        al_draw_filled_rectangle(pos_matriz_x + j * blockWidth, pos_matriz_y + i * blockHeight, pos_matriz_x + (j * blockWidth) + blockWidth, pos_matriz_y + (i * blockHeight) + blockHeight, al_map_rgb(COR_SILICIO));
                        break;
                    case 4: // PEDRA INQUEBRAVEL
                        al_draw_filled_rectangle(pos_matriz_x + j * blockWidth, pos_matriz_y + i * blockHeight, pos_matriz_x + (j * blockWidth) + blockWidth, pos_matriz_y + (i * blockHeight) + blockHeight, al_map_rgb(COR_LAVA));
                        break;
                    }
            }
            if(draw)
            {
                al_draw_rectangle(pos_mouse_x, pos_mouse_y, pos_mouse_x + blockWidth, pos_mouse_y + blockHeight, al_map_rgb(0, 0, 0), 1);
            }
            al_draw_textf(font18, al_map_rgb(255, 0, 255), 5, 5, 0, "FPS: %i", gameFPS);	//display FPS on screen

            //FLIP BUFFERS========================
            al_flip_display();
            al_clear_to_color(al_map_rgb(0,0,0));
        }
    }

    //==============================================
    //DESTROY PROJECT OBJECTS
    //==============================================

    //SHELL OBJECTS=================================
    al_destroy_font(font18);
    al_destroy_timer(timer);
    al_destroy_event_queue(event_queue);
    al_destroy_display(display);

    return 0;
}

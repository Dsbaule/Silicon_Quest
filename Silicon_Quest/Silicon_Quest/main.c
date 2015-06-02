/*
	Jogo Silicon_Quest feito em Allegro 5.

	Feito por Stefano Bergamini e Daniel Baule.
*/

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define WIDTH 800
#define HEIGHT 600

//==============================================
//GLOBALS
//==============================================

bool keys[] = {false, false, false, false, false, false, false, false, false};
enum KEYS {W, S, A, D, UP, DOWN, LEFT, RIGHT, SPACE};

int colunas = 20;
int linhas = 20;

const int tam_bloco_hor = 80;
const int tam_bloco_ver = 60;
const int tam_player_hor = 40;
const int tam_player_ver = 30;

int matriz[20][20]=
{
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1}
};

bool done = false;

int pos_mouse_x = WIDTH / 2;
int pos_mouse_y = HEIGHT / 2;

int coluna, linha;

typedef struct
{
	int x;
	int y;
}s_Object;

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
	int xOff = 0;
	int yOff = 0;

	s_Object Player;
	Player.x = WIDTH / 2 - 60 ;
	Player.y = HEIGHT / 2 - 15 ;

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
    al_install_keyboard();
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
                coluna = (pos_mouse_x - xOff) / tam_bloco_hor;
                linha = (pos_mouse_y - yOff) / tam_bloco_ver;
            }
            if(matriz[linha][coluna] == 1)
			{
				matriz[linha][coluna] = 0;
			}

            if(ev.mouse.button & 2)
            {
                done = true;
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
			case ALLEGRO_KEY_A:
				keys[A] = true;
				break;
			case ALLEGRO_KEY_D:
				keys[D] = true;
				break;
			case ALLEGRO_KEY_W:
				keys[W] = true;
				break;
			case ALLEGRO_KEY_S:
				keys[S] = true;
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
			case ALLEGRO_KEY_A:
				keys[A] = false;
				break;
			case ALLEGRO_KEY_D:
				keys[D] = false;
				break;
			case ALLEGRO_KEY_W:
				keys[W] = false;
				break;
			case ALLEGRO_KEY_S:
				keys[S] = false;
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


            if(!(xOff >= 0))
				xOff += (keys[LEFT] | keys[A]) * 10;
			if(!(yOff >= 0))
				yOff += (keys[UP] | keys[W]) * 10;
			if(!(xOff <= ((-colunas * tam_bloco_hor) + WIDTH)))
				xOff -= (keys[RIGHT] | keys[D]) * 10;
			if(!(yOff <= ((-linhas * tam_bloco_ver) + HEIGHT)))
				yOff -= (keys[DOWN] | keys[S]) * 10;
        }

        //==============================================
        //RENDER
        //==============================================
        if(render && al_is_event_queue_empty(event_queue))
        {
            render = false;


            //BEGIN PROJECT RENDER================
            int j, i;
            for(i=0; i<20; i++)
            {
                for(j=0; j<20; j++)
                    switch(matriz[i][j])
                    {
                    case 0:
                        al_draw_filled_rectangle(xOff + j * tam_bloco_hor, yOff + i * tam_bloco_ver, xOff + (j * tam_bloco_hor) + tam_bloco_hor, yOff + (i*tam_bloco_ver) + tam_bloco_ver, al_map_rgb(122, 0, 0));
                        break;
                    case 1:
                        al_draw_filled_rectangle(xOff + j * tam_bloco_hor, yOff + i * tam_bloco_ver, xOff + (j * tam_bloco_hor) + tam_bloco_hor, yOff + (i * tam_bloco_ver) + tam_bloco_ver, al_map_rgb(122, 0, 122));
                        break;
                    }
            }
			al_draw_filled_rectangle(Player.x, Player.y, Player.x + tam_player_hor, Player.y + tam_player_ver, al_map_rgb(255, 255, 255));
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

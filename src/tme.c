#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>

#define ERR(MSG) fprintf(stderr, MSG);
#define FATALERR(MSG) {fprintf(stderr, "FATAL ERROR!\n"); fprintf(stderr, MSG); done(); exit(-1);}
#define ERROR_IF(VAR,MSG) if(VAR){fprintf(stderr, "ERROR: "); fprintf(stderr, MSG);}
#define MID(min, value, max) (min < value) ? ((max > value) ? value : max) : (min)
 
const float FPS = 30;
const int SCREEN_W = 640;
const int SCREEN_H = 480;
const int TILE_W = 32;
const int TILE_H = 32;
const int BOUNCER_SIZE = 32;
const char* mapFileName = "level.map";
enum MYKEYS { KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT };
bool key[4] = {false, false, false, false};
bool gameover = false;

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *bouncer = NULL;
ALLEGRO_BITMAP *bouncer1 = NULL;
ALLEGRO_BITMAP *bouncer2 = NULL; 
ALLEGRO_BITMAP *tileset = NULL; 
ALLEGRO_BITMAP *border = NULL;
ALLEGRO_BITMAP *tileMap = NULL;
ALLEGRO_FONT   *font = NULL;
ALLEGRO_CONFIG *cfg = NULL;
float bouncer_x;// = SCREEN_W / 2.0 - BOUNCER_SIZE / 2.0;
float bouncer_y;// = SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0;
bool redraw_time = true;
bool redraw_needed = true;
const int VIEW_ABLE_TILES_X = 15;
const int VIEW_ABLE_TILES_Y = 15;

int init(void);
int done(void);

//////////////////////////////////////////////////////////////////////////////////////
int loadTiles(const char* dateiname ){
    int tx = 0; /// Anzahl Tiles in X-Richtung
    int ty = 0; /// Anzahl Tiles in Y-Richtung
    int cx = 0; /// Counter für Tile-Spalte
    int cy = 0; /// Counter für Tile-Zeile
    int tilecounter = 0; /// Counter für Tiles im Tileset
    ALLEGRO_BITMAP *temp = NULL;

    if(! (tilesources  = al_load_bitmap("data/tiles.tga"))         ){FATALERR("failed to load data/tiles.tga!\n")}
    ty = al_get_bitmap_height(tilesources);
    ty = ty / TILE_H;
    tx = al_get_bitmap_width(tilesources);
    tx = tx / TILE_W;

    if(! (tileset = al_create_bitmap(TILE_W*tx*ty, TILE_H)) ){FATALERR("failed to create tile bitmap!\n")}

    al_set_target_bitmap(bouncer2);
//    al_clear_to_color(al_map_rgb(0, 128, 128));
    for(cx = 0; cx < tx; cx++){
        for(cy = 0; cy < ty; cy++){
            void al_draw_bitmap(ALLEGRO_BITMAP *bitmap, float dx, float dy, int flags)
        }//end for(cx...
    }//end for(cx...
    al_set_target_bitmap(al_get_backbuffer(display));
    al_destroy_bitmap(tilesource);
}

//////////////////////////////////////////////////////////////////////////////////////
int init(){
    bouncer_x = SCREEN_W / 2.0 - BOUNCER_SIZE / 2.0;
    bouncer_y = SCREEN_H / 2.0 - BOUNCER_SIZE / 2.0;  

    if(! al_init()             ){FATALERR("failed to initialize allegro!\n")}
    if(! al_install_mouse()    ){FATALERR("failed to initialize the mouse!\n")}
    if(! al_install_keyboard() ){FATALERR("failed to initialize the keyboard!\n")}
    if(! al_init_image_addon() ){FATALERR("failed to initialize allegro_image_addon!\n")}    
/*if(!*/ al_init_font_addon();//  ){FATALERR("failed to initialize allegro_font_addon!\n")}     // initialize the font addon
    if(! al_init_ttf_addon()   ){FATALERR("failed to initialize allegro_ttf_addon!\n")}    // initialize the ttf (True Type Font) addon
    if(! (font = al_load_ttf_font("data/pirulen.ttf",12,0 )) ){FATALERR("failed to load Font: pirulen.ttf!\n")} // load the font | Font from Larabie Free Fonts

    if(! (timer   = al_create_timer(1.0 / FPS))                   ){FATALERR("failed to create timer!\n")} 
    if(! (display = al_create_display(SCREEN_W, SCREEN_H))        ){FATALERR("failed to create display!\n")}
    if(! (border  = al_load_bitmap("data/border.png"))                 ){FATALERR("failed to load border bitmap!\n")}

    //FIXME: Programm-Abbruch wenn Icon nicht gefunden wird! -> Bitmap vorher Laden mit Fehlerprüfung!
    al_set_display_icon(display, al_load_bitmap("data/tme.png"));// ){ERR("Can't set Window-Icon!\n")}
    al_set_window_title(display, "TileMapEditor")          ;// ){ERR("Can't set Window-Titel!\n")}

    if(! (cfg = al_load_config_file("tme.cfg"))  ){ERR("failed to load tme config file!\n")}

    if(! (bouncer1 = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE)) ){FATALERR("failed to create bouncer bitmap!\n")}
    al_set_target_bitmap(bouncer1);
    al_clear_to_color(al_map_rgb(255, 0, 0));
    al_set_target_bitmap(al_get_backbuffer(display));
 
    if(! (bouncer2 = al_create_bitmap(BOUNCER_SIZE, BOUNCER_SIZE)) ){FATALERR("failed to create bouncer bitmap!\n")}
    al_set_target_bitmap(bouncer2);
    al_clear_to_color(al_map_rgb(0, 0, 255));
    al_set_target_bitmap(al_get_backbuffer(display));

    bouncer = bouncer1;
 
    if(! (tileMap = al_create_bitmap(SCREEN_W, SCREEN_H)) ){FATALERR("failed to create tileMap bitmap!\n")}
 
    if(! (event_queue = al_create_event_queue())                  ){FATALERR("failed to create event_queue!\n")}
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
	al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
 
    al_clear_to_color(al_map_rgb(0,0,0));
    al_flip_display();
 
    al_start_timer(timer); 
    return 0;
}//end init() 

//////////////////////////////////////////////////////////////////////////////////////
int done(){ 
    if(cfg)         al_destroy_config(cfg);
    if(bouncer1)     al_destroy_bitmap(bouncer1);
    if(bouncer2)     al_destroy_bitmap(bouncer2);
    if(border)      al_destroy_bitmap(border);
    if(timer)       al_destroy_timer(timer);
    if(display)     al_destroy_display(display);
    if(event_queue) al_destroy_event_queue(event_queue);
    return 0;
}//end done()

//////////////////////////////////////////////////////////////////////////////////////
int event_handling(ALLEGRO_EVENT ev){
    if(ev.type == ALLEGRO_EVENT_TIMER) {
        redraw_time = true;

    }else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
        gameover = true;

    }else if(ev.type == ALLEGRO_EVENT_MOUSE_AXES || ev.type == ALLEGRO_EVENT_MOUSE_ENTER_DISPLAY) {
        bouncer_x = ev.mouse.x;
        bouncer_y = ev.mouse.y;
        redraw_needed = true;

    }else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN){
        bouncer = bouncer2;
        redraw_needed = true;

    }else if(ev.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP){
        bouncer = bouncer1;
        redraw_needed = true;
        
    }else if (ev.type == ALLEGRO_EVENT_KEY_DOWN){
		switch(ev.keyboard.keycode)			{
		case ALLEGRO_KEY_UP:
			key[KEY_UP] = true;
			break;

		case ALLEGRO_KEY_DOWN:
			key[KEY_DOWN] = true;
			break;

		case ALLEGRO_KEY_LEFT:
			key[KEY_LEFT] = true;
			break;

		case ALLEGRO_KEY_RIGHT:
			key[KEY_RIGHT] = true;
			break;
		}//end switch

	}else if (ev.type == ALLEGRO_EVENT_KEY_UP){
		switch(ev.keyboard.keycode){
		case ALLEGRO_KEY_UP:
			key[KEY_UP] = false;
			break;

		case ALLEGRO_KEY_DOWN:
			key[KEY_DOWN] = false;
			break;

		case ALLEGRO_KEY_LEFT:
			key[KEY_LEFT] = false;
			break;

		case ALLEGRO_KEY_RIGHT:
			key[KEY_RIGHT] = false;
			break;

		case ALLEGRO_KEY_ESCAPE:
			gameover = true;
			break;
		}//end switch
	}//end if(ev.type...
	return 0;
}//end event_handling(ALLEGRO_EVENT ev)

//////////////////////////////////////////////////////////////////////////////////////
void drawMap(int wx, int wy){  //sichtbarer bereich b=480 h=450
/*    int pos = 0;
    int x, y;
//    const int lineSize         
    wx = MID(0, wx, maxX);
    wy = MID(0, wy, maxY);
    
    int ox = wx % tileWidth;
    int oy = wy % tileHeight;
    wx /= tileWidth;
    wy /= tileHeight;
    
    al_set_target_bitmap(tileMap);
    for(y=0; y < VIEW_ABLE_TILES_Y+1; ++y) {
        pos = wx * d + (y+wy) * lineSize;
        for (x=0; x < VIEW_ABLE_TILES_Y+1; ++x) {
            for (int z=0; z < d; ++z) {
                al_clear_to_color(al_map_rgb(255, 0, 0));
//                drawTile(dest, x*tileWidth-ox, y*tileHeight-oy, data[pos]); 

                ++pos;                

            }//end for(z...

        }//end for(x...

    }//end for(y...    

    al_set_target_bitmap(al_get_backbuffer(display));
/////////////////////////////////7
        int x, y;
        y = START_Y;
        for(int yField = 0; yField < MAP_H; ++yField) {
            x = START_X;
            if(yField & 1){
                x += OFS_X;
            }//end if(yField & 1)
            for(int xField = 0; xField < MAP_W; ++xField) {
                int tile = getTileAt(xField, yField);
                drawTile(doubleBuffer, tiles, x, y, tile);
                x += TILE_W;            
            }//end for(xField...) Zeile Zeichnen
            y += OFS_Y;                        
        }//end for(xField... Karte Zeichnen
/////////////////////////////////////7
*/

}//end drawMap()

//////////////////////////////////////////////////////////////////////////////////////
int draw(){
    redraw_time = false;
    redraw_needed = false;
    al_clear_to_color(al_map_rgb(0xCC,0,0));
//    int x = MID(0, focus_x, map_breite * TILE_W - 480);
//    int y = MID(0, focus_y, map_hoehe * TILE_H - 450);
//    drawMap(x, y);
    drawMap(0, 0);
    al_draw_bitmap(border, 0, 0, 0);
    al_draw_text(font, al_map_rgb(255,255,255), (100), ( 5),ALLEGRO_ALIGN_CENTRE, "Your Titel Here!");// draw the text
    al_draw_text(font, al_map_rgb(255,255,255), (SCREEN_W/2), (SCREEN_H-17),ALLEGRO_ALIGN_CENTRE, "Statuszeilen-texte hier her :) ...");// draw the text
    al_draw_bitmap(bouncer, bouncer_x, bouncer_y, 0);
    al_flip_display();
    return 0;
}//end draw()

//////////////////////////////////////////////////////////////////////////////////////
void loadMap(const char* filename){

}//end loadMap(const char*)

//////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv){
    init();
    while(!gameover){
        ALLEGRO_EVENT ev;
        al_wait_for_event(event_queue, &ev);
        event_handling(ev);

        if(redraw_time && redraw_needed && al_is_event_queue_empty(event_queue)){
            draw();
        }
    }//end while
    done();
    return 0;
}



#include <allegro.h>
#include <math.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstring>

using namespace std;
///////////////////////////////////////
//GLOBAL VARIABLES//
int map[24][32];
BITMAP* buffer;
BITMAP* man;
BITMAP* flames;
BITMAP* ground;
BITMAP* movetile;
BITMAP* box;
BITMAP* teleport;
BITMAP* door;
BITMAP* klucz;

SAMPLE* Jump;
SAMPLE* Teleport;
SAMPLE* Door;
SAMPLE* Chest;
SAMPLE* Win;
MIDI* background;
///////////////////////////////////////
//FUNCTION INITIALIZATIONS//
int getWhole(float a) {return (int)trunc(a);}
void split(const string& s, char c, vector<string>& v);
bool LoadMap(int map[][32]);
void init();
void deinit();
///////////////////////////////////////
//CLASSES//
class character
{
    public:
      int x, y, time, timeMem, xMem, yMem, lvl;
      bool win, jump, klucz;
      character() {x = 10; y = 440; xMem = yMem= 0; win=klucz=false; lvl=1;} //initialization of starting point and reseting memory
      void move();
      bool fall();
      bool afterjump() {if (time-timeMem<=5) return true; else {jump=false; return false;}}
      void TimeReset() {time=0;}
      void TimeMemory() {timeMem=time;}
      void CoordsMemory() {xMem=x; yMem=y;}
} player;

void character::move()
{
     int mx=getWhole(x/20); //tile in which player stays
     int my=getWhole(y/20);
     if(!fall()&&key[KEY_SPACE]) //jumping if not falling down
          {
              if (y>40) //checking borders
                 if(x==mx*20) //the case of one tile
                    {
                      if (y!=my*20)
                      {
                          TimeMemory();
                          CoordsMemory();
                          y-=40;
                          jump=true;
                      }
                      else if(map[my-1][mx]!=2)
                      {
                          TimeMemory();
                          CoordsMemory();
                          y-=40;
                          jump=true;
                      }
                    }
                 else //two tiles
                 {
                     if (y!=my*20)
                     {
                          TimeMemory();
                          CoordsMemory();
                          y-=40;
                          jump=true;
                     }
                     else if ((map[my-1][mx]!=2)&&(map[my-1][mx+1]!=2))
                     {
                          TimeMemory();
                          CoordsMemory();
                          y-=40;
                          jump=true;
                     }
                 }
     }
     else if (key[KEY_DOWN])
     {
          if (y+20<479)
             if(x==mx*20) //the case of one tile
             {
                  if (y!=my*20) y++;
                  else if(map[my+1][mx]==3) y++; 
             }
             else //two tiles
             {
                  if (y!=my*20) y++;
                  else if ((map[my+1][mx]==3)||(map[my+1][mx+1]==3)) y++;
             }
     }
     else if (key[KEY_RIGHT])
     {
             if (x+20<639) //checking borders
                 if(y==my*20) //the case of one tile
                 {
                      if (x!=mx*20) x++;
                      else if(map[my][mx+1]!=2&&map[my][mx+1]!=6) x++; 
                 }
                 else //two tiles
                 {
                     if (x!=mx*20) x++;
                     else if ((map[my][mx+1]!=2&&map[my][mx+1]!=6)&&(map[my+1][mx+1]!=2&&map[my+1][mx+1]!=6)) x++;
                 }   
     }
     else if (key[KEY_LEFT]) 
     {
            if (x>0) //checking borders
                 if(y==my*20) //the case of one tile
                    {
                      if (x!=mx*20) x--;
                      else if(map[my][mx-1]!=2&&map[my][mx-1]!=6) x--; 
                    }
                 else //two tiles
                 {
                     if (x!=mx*20) x--;
                     else if ((map[my][mx-1]!=2&&map[my][mx-1]!=6)&&(map[my+1][mx-1]!=2&&map[my+1][mx-1]!=6)) x--;
                 }   
     }
     else if (key[KEY_ENTER])
     {
          if (map[my][mx]==4||map[my][mx+1]==4) //player opens box
          {
                player.win=true;
                player.lvl++;
                Chest=load_sample("music/chest.wav");
                play_sample(Chest, 255, 128, 1000, false);
          }
          else if (map[my][mx]==5||map[my][mx+1]==5) //player uses teleport
          {
                  if(time-timeMem>10) 
                  for (int i=0; i<24; i++)
                  {
                      for(int t=0; t<32; t++)
                      {
                          if(map[i][t]==5)
                             if ((i!=my)||((t!=mx)&&(t!=mx+1)))
                             {
                               y=i*20;
                               x=t*20;
                               TimeMemory();
                               Teleport=load_sample("music/teleport.wav");
                               play_sample(Teleport, 255, 128, 1000, false);
                             }
                      }
                  }
          }
          else if (map[my][mx-1]==6) //player opens door
          {
               if (player.klucz==true)
               {
                  map[my][mx-1]=1;
                  player.klucz=false;
                  Door=load_sample("music/door.wav");
                  play_sample(Door, 255, 128, 1000, false);
               }
          }
          else if (map[my][mx+1]==6) //player opens door
          {
               if (player.klucz==true)
               {
                    map[my][mx+1]=1;
                    player.klucz=false;
                    Door=load_sample("music/door.wav");
                    play_sample(Door, 255, 128, 1000, false);
               }
          }
     }
}

bool character::fall()
{
     int mx=getWhole(x/20);
     int my=getWhole(y/20);
     if (y+20<479) //checking borders
         if(x==mx*20) //the case of one tile
         {
              if (y!=my*20) {y++; return true;}
              else if(map[my+1][mx]==1||map[my+1][mx]==4||map[my+1][mx]==5) {y++; return true;}
              else return false; //player stands still
         }
         else //two tiles
         {
              if (y!=my*20) {y++; return true;}
              else if ((map[my+1][mx]==1||map[my+1][mx]==4||map[my+1][mx]==5)&&(map[my+1][mx+1]==1||map[my+1][mx+1]==4||map[my+1][mx+1]==5)) {y++; return true;}
              else return false; //player stands still
         }
     else return false; //player stands on the bottom
}
///////////////////////////////////////
//MAIN FUNCTION//
int main()
{
    LoadMap(map);
	init();
	buffer = create_bitmap(640,480);
    flames = load_bitmap("graphics/flames.bmp",NULL);
    ground = load_bitmap("graphics/ground.bmp",NULL);
    movetile = load_bitmap("graphics/movetile.bmp",NULL);
    box = load_bitmap("graphics/box.bmp",NULL);
    teleport = load_bitmap("graphics/teleport.bmp",NULL);
    door = load_bitmap("graphics/door.bmp",NULL);
    player.TimeReset();
    player.TimeMemory();
    
    BITMAP* start;
    start=load_bitmap("graphics/start.bmp",NULL);
    draw_sprite(screen, start, 0, 0);
    while (!key[KEY_ENTER]);
    background=load_midi("music/back.mid");
    play_midi(background, true);
    Jump=load_sample("music/hop.wav");
    
	while (!key[KEY_ESC]) //game loop
    {
//plane drawing
        rectfill(buffer,0,0,640,480,makecol(128,255,255));
        for (int i=0; i<=24; i++)
        {
                for(int t=0; t<=32; t++)
                {
                     if(map[i][t]==2) draw_sprite(buffer, ground, t*20, i*20);
                     else if(map[i][t]==3) draw_sprite(buffer, movetile, t*20, i*20);
                     else if( map[i][t]==4) draw_sprite(buffer, box, t*20, i*20);
                     else if( map[i][t]==5) draw_sprite(buffer, teleport, t*20, i*20);
                     else if( map[i][t]==6) draw_sprite(buffer, door, t*20, i*20);
                     else if( map[i][t]==7)
                     {
                          if(player.time%2!=0) klucz = load_bitmap("graphics/key.bmp",NULL);
                          else klucz = load_bitmap("graphics/key_glitter.bmp",NULL);
                          draw_sprite(buffer, klucz, t*20, i*20);
                     }
                }       
        }
        //////////moving graphics
        if(player.klucz==false)
        {
          if (key[KEY_LEFT])
          {
             if (player.fall()) man = load_bitmap("graphics/man_left3.bmp",NULL);
             else if(player.time%2==0) man = load_bitmap("graphics/man_left1.bmp",NULL);
             else man = load_bitmap("graphics/man_left2.bmp",NULL);
          }
          else if (key[KEY_RIGHT])
          {
             if (player.fall()) man = load_bitmap("graphics/man_right3.bmp",NULL);
             else if(player.time%2==0) man = load_bitmap("graphics/man_right1.bmp",NULL);
             else man = load_bitmap("graphics/man_right2.bmp",NULL);
          }
          else if(player.fall()) man = load_bitmap("graphics/man_fall.bmp",NULL);
          else if (map[getWhole(player.y/20)][getWhole(player.x/20)-1]==7)
          {
               map[getWhole(player.y/20)][getWhole(player.x/20)-1]=1;
               player.klucz=true;
          }
          else if (map[getWhole(player.y/20)][getWhole(player.x/20)+1]==7)
          {
               map[getWhole(player.y/20)][getWhole(player.x/20)+1]=1;
               player.klucz=true;
          }
          else man = load_bitmap("graphics/man.bmp",NULL);
        }
        else man = load_bitmap("graphics/key.bmp",NULL);
        
        
        if(player.win)
        {
           if (LoadMap(map))
           {
                 
                 player.win=false;
                 player.x = 10; player.y = 440;
           }
           else
           {
               BITMAP* WIN;
               WIN=load_bitmap("graphics/WIN.bmp", NULL);
               draw_sprite(screen, WIN, 300,150);
               stop_midi();
               stop_sample(Chest);
               Win=load_sample("music/win.wav");
               play_sample(Win, 255, 128, 1000, false);
               while(!key[KEY_ESC]);
               break;
           }
        }
        player.move();
        if(player.afterjump()&&player.jump) draw_sprite(buffer, flames, player.xMem, player.yMem);
        if(player.jump) play_sample(Jump, 255, 128, 1000, false);
        
        player.time++;
        
        if (key[KEY_UP])
        {
                        char ac[10];
                        sprintf(ac, "x=%d, y=%d", player.x, player.y);
                        textout_ex(buffer,font,ac,200,0,makecol(255,0,0),makecol(128,255,255));
        }
        
        draw_sprite(buffer, man, player.x, player.y);
        
        char lvl[10];
        sprintf(lvl, "LEVEL %d", player.lvl);
        textout_ex(buffer,font,lvl,0,0,makecol(255,0,0),makecol(128,255,255));
        
        draw_sprite(screen, buffer, 0, 0);
        rest(10);
	}
	deinit();
	return 0;
}
END_OF_MAIN();
///////////////////////////////////////
//FUNCTIONS//
void split(const string& s, char c, vector<string>& v)
{
   string::size_type i = 0;
   string::size_type j = s.find(c);
   while (j != string::npos) {
      v.push_back(s.substr(i, j-i));
      i = ++j;
      j = s.find(c, j);
      if (j == string::npos)
         v.push_back(s.substr(i, s.length( )));
   }
}

bool LoadMap(int map[][32])
{
    ifstream mapfile;
    string line;
    vector<string> v;
    
    char path[16];
    sprintf(path, "maps/map_%d.txt", player.lvl);
    mapfile.open(path, ios::in);
    
    if(mapfile)
    {
               int i=0;
               while ( mapfile.good() )
               {
                     getline (mapfile,line);
                     split(line, ',', v);
                     for (int k = 0; k < v.size( ); ++k) 
                     {
                         istringstream iss(v[k]);
                         iss >> map[i][k];
                     }
                     v.clear();
                     i++;
               }
               mapfile.close();
               return true;
    }
    else return false;
}

void init() 
{
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 640, 480, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}
	if(install_sound(DIGI_AUTODETECT, MIDI_AUTODETECT, NULL))
    { 
        allegro_message("Sound Error: %s", allegro_error); 
    } 
    set_volume(255, 255); //digital and music to max levels 
	install_timer();
	install_keyboard();
	install_mouse();
}

void deinit()
{
     clear_keybuf();
}

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_timer.h>
#include <stdbool.h>
#include <SDL_image.h>

#include "texture.h"


const int SCREEN_WIDTH = 1280, SCREEN_HEIGHT = 640; //wymiary okna gry
int score = 0; //licznik pokonanych przeciwników
int LOOSE = 0;
int WIN = 0;

struct sll_node     //lista do przechowywawnia wynikow 
{
    int score;
    struct sll_node* next;
};

//enemies struktura z  hitboxem przeciwników kwadratem i ich stanem 
struct enemy_t
{
    SDL_Rect hitbox;
    int alive;
    int hp;

}enemy[5];



struct bullet_t  //struktura z  hitboxem pocisku kwadratem i jego stanem 
{
    SDL_Rect hitbox;
    int state;
    int dmg;
}bullet1;


void int_bullet()       //inicjacja pocisku bazowy stan i wspó?rz?dne 
{
    bullet1.state = 0;
    bullet1.dmg = 50;
    bullet1.hitbox.x = 0;
    bullet1.hitbox.y = 0;
    bullet1.hitbox.w = 10;
    bullet1.hitbox.h = 10;
}


void int_enemies()   //inicjacja przeciwników bazowy stan i wspólrz?dne 
{
    int a = 240;
    int y = 10;
    for (int j = 0; j < 4; j++) {
        // for (int y = 0; y < 5; y++) {
             //for (int i = 0, a = 45; i < 7; i++, a += 90) {
        enemy[j].alive = 1;
        enemy[j].hp = 100;
        enemy[j].hitbox.x = a;
        enemy[j].hitbox.y = y;
        enemy[j].hitbox.w = 10 * 3;
        enemy[j].hitbox.h = 27 * 3;

        a += 90;
    }
    a = 240;
}
int collision(SDL_Rect a, SDL_Rect b) {     //funkcja wykrywaj?ca nak?adanie si? 2 kwadratów
    //warunki kiedy si? nie nak?adaj?
    if (a.y + a.h < b.y) {

        return 0;
    }

    if (a.y > b.y + b.h) {

        return 0;
    }

    if (a.x > b.x + b.w) {

        return 0;
    }

    if (a.x + a.w < b.x) {

        return 0;
    }

    return 1;       //w domy?le nak?adaj? si? 
}
void enemy_hit_collision() {    //funkcja wykrywaj?ca koljzje pocisku i przeciwnika

    int i, j, c;

    for (i = 5; i >= 0; i--) {

        //for (j = 5; j >= 0; j--) {

        if ((enemy[i].alive) == 1) {

            if (bullet1.state == 1) {

                c = collision(bullet1.hitbox, enemy[i].hitbox);

                if (c == 1) {       //jezli wystepuje kolizja to zabij przeciwnika i zniszcz pocisk

                    enemy[i].hp -= bullet1.dmg;
                    bullet1.state = 0;
                    bullet1.hitbox.x = 0;
                    bullet1.hitbox.y = 0;

                    if (enemy[i].hp <= 0) {
                        enemy[i].alive = 0;
                        score++;    //tymczasowa zmnienna na punkty co zabujstwo zwiekszane
                    }
                }

            }
            //}
        }
    }
}

void loop_enemy()   //nowa fala przeciwników
{
    int i;
    int dead_enemies = 0;
    for (i = 0; i < 5; i++)
    {
        if (enemy[i].alive == 0)
        {
            dead_enemies++;
            if (dead_enemies == 5) {     //je?li wszyscy zabici
                int_enemies();
            }
        }
    }
}

void lose_conditions(SDL_Texture* Przegrana, SDL_Renderer* renderer) //warunki pora?ki, parametr wspó?rz?dnej przeciwników
{
    int i;
    for (i = 0; i < 5; i++)
    {
        if (enemy[i].alive == 1)
            if (enemy[i].hitbox.y >= 600) {
                SDL_RenderCopy(renderer, Przegrana, NULL, &(SDL_Rect) {0, 0, 1280, 640});   //show loss  
                LOOSE = 1;
            }
    }

}

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        printf("JAKIS BLAD\n");

    SDL_Window* window = SDL_CreateWindow("Silenzio", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (NULL == window)
    {
        printf("BLAD TWORZENIA OKNA");
        return EXIT_FAILURE;
    }

    SDL_Event event;

    bool StartGame = true;
    int x = 570, y = 350, h = 50, w = 50;   //koordynaty spawn postaci
    float old_time = SDL_GetTicks();

    //clean with black
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    //show what was drawn
    SDL_RenderPresent(renderer);




    //wczytywanie wszystkich tekstur

    SDL_Texture* ModelBkMv = LoadTexture("Sprite/CharacterMovement/BackMv/BackMv0.png", renderer);

    SDL_Texture* ModelBkWK = LoadTexture("Sprite/CharacterMovement/Backwalking/BkWk0.png", renderer);

    SDL_Texture* background = LoadTexture("Sprite/proba2.png", renderer);

    SDL_Texture* background1 = LoadTexture("Sprite/proba1.png", renderer);

    SDL_Texture* bron1 = LoadTexture("Sprite/Ramka1.png", renderer);

    SDL_Texture* bron2 = LoadTexture("Sprite/Ramka2.png", renderer);

    SDL_Texture* CwiercNuta = LoadTexture("Sprite/BulletsAndEnemies/CwiercNuta.png", renderer);

    SDL_Texture* PolNuta = LoadTexture("Sprite/BulletsAndEnemies/PolNuta.png", renderer);

    SDL_Texture* CalaNuta = LoadTexture("Sprite/BulletsAndEnemies/CalaNuta.png", renderer);

    SDL_Texture* PauzaCwierc = LoadTexture("Sprite/BulletsAndEnemies/PauzaCwierc.png", renderer);

    SDL_Texture* PauzaOsem = LoadTexture("Sprite/BulletsAndEnemies/PauzaOsem.png", renderer);

    SDL_Texture* Menu = LoadTexture("Sprite/menu.png", renderer);

    SDL_Texture* Resume = LoadTexture("Sprite/resume.png", renderer);

    SDL_Texture* Wygrana = LoadTexture("Sprite/Wygrana.png", renderer);

    SDL_Texture* Przegrana = LoadTexture("Sprite/Przegrana.png", renderer);

    SDL_Texture* NapisScore = LoadTexture("Sprite/NapisScore.png", renderer);

    SDL_Texture** AnimationBkMv = LoadAnimation("Sprite/CharacterMovement/BackMv/BackMv%d.png", renderer, 8);

    SDL_Texture** AnimationBkWk = LoadAnimation("Sprite/CharacterMovement/Backwalking/BkWk%d.png", renderer, 8);

    SDL_Texture** AnimationRWk = LoadAnimation("Sprite/CharacterMovement/RightWalking/RWk%d.png", renderer, 8);

    SDL_Texture** AnimationLWk = LoadAnimation("Sprite/CharacterMovement/LeftWalking/LWk%d.png", renderer, 8);

    SDL_Texture** AnimationFWk = LoadAnimation("Sprite/CharacterMovement/FrontWalking/FWk%d.png", renderer, 8);

    SDL_Texture** AnimationScore = LoadAnimation("Sprite/Score/%d.png", renderer, 151);




    const int FPS = 30;
    const int frameDelay = 1000 / FPS;
    Uint32 frameStart;
    int frameTime;

    //zmienne do:
    int Model = 0, Gun = 0;         //wybor kierunku poruszania, wybor posisku
    int AniFrame = 0;               //aktualna klatka animacji postaci
    int scene = 0, enemies = 0, poziom=0;        //wybor aktywnej sceny oraz wybor przeciwnikow

                                    //zmiana wygl?du przeciwnikow



    int_enemies();                  //inicjowanie przeciwników i pocisku
    int_bullet();


    while (StartGame)
    {

        //start time
        frameStart = SDL_GetTicks();


        SDL_PollEvent(&event);

        frameStart = SDL_GetTicks();
        switch (event.type)
        {
        case SDL_QUIT:
            StartGame = false;
            break;
        }

        //zmienna  wygl?du przeciwników
        if (score > 49)
        {
            enemies = 2;
        }

        if (score >= 25)
        {
            enemies = 1;
            scene = 2;      //jaki? ekran gdy wygranie

            WIN = 1;    
        }




        //aktywacja klawiszy tj. zmiana scen, movement postaci, strzelanie
        if (SDL_KEYUP == event.type)
        {
            if (SDLK_UP == event.key.keysym.sym)
            {
                AniFrame = 0;
            }

            if (SDLK_DOWN == event.key.keysym.sym)
            {

                AniFrame = 0;
            }
            if (SDLK_LEFT == event.key.keysym.sym)
            {

                AniFrame = 0;
            }

            if (SDLK_RIGHT == event.key.keysym.sym)
            {
                AniFrame = 0;
            }

            if (SDLK_TAB == event.key.keysym.sym)
            {
                scene = 1;
            }

            if (SDLK_1 == event.key.keysym.sym)
            {
                Gun = 0;
            }

            if (SDLK_2 == event.key.keysym.sym)
            {
                Gun = 1;
            }

            if (SDLK_ESCAPE == event.key.keysym.sym)
            {
                scene = 3;

            }

            if (scene == 3)     //exit game if pause
                if (SDLK_SPACE == event.key.keysym.sym)
                {
                    StartGame = false;
                }

            if (LOOSE == 1)
                if (SDLK_ESCAPE == event.key.keysym.sym)
                {
                    int_enemies();
                    LOOSE = 0;
                    score = 0;
                }

            if (WIN == 1)
                if (SDLK_ESCAPE == event.key.keysym.sym)
                {
                    int_enemies();
                    WIN = 0;
                    score = 0;
                    poziom = 1;
                }

        }


        if (SDL_KEYDOWN == event.type)
        {

            if (SDLK_TAB == event.key.keysym.sym)
            {
                scene = 1;
            }



            if (SDLK_UP == event.key.keysym.sym && y >= 350)
            {
                y -= (10);
                if (AniFrame == 7)
                {
                    AniFrame = 0;
                }
                else AniFrame++;

                Model = 1;
            }

            if (SDLK_DOWN == event.key.keysym.sym && y <= (639 - 224))
            {

                y += (10);
                if (AniFrame == 7)
                {
                    AniFrame = 0;
                }
                else AniFrame++;

                Model = 2;
            }
            if (SDLK_LEFT == event.key.keysym.sym && x >= (130))
            {

                x -= (10);
                if (AniFrame == 7)
                {
                    AniFrame = 0;
                }
                else AniFrame++;

                Model = 3;
            }

            if (SDLK_RIGHT == event.key.keysym.sym && x <= (670))
            {
                x += (10);
                if (AniFrame == 7)
                {
                    AniFrame = 0;
                }
                else AniFrame++;

                Model = 4;
            }

            if (SDLK_SPACE == event.key.keysym.sym)
            {


                //create bullet give bullet player position if
                if (bullet1.state == 0)
                {
                    bullet1.state = 1;
                    bullet1.hitbox.x = (75 + x);
                    bullet1.hitbox.y = y + 10;
                }

                if (AniFrame == 7)
                {
                    AniFrame = 0;
                }
                else
                {
                    AniFrame++;
                }
                Model = 0;
            }


        }

        SDL_Rect rect = { x,y,h,w };

        SDL_RenderFillRect(renderer, &rect);

        switch (scene)
        {
        case 0:

           
            SDL_RenderCopy(renderer, Menu, NULL, &(SDL_Rect) {0, 0, 1280, 640});                                    //show menu
            break;

        case 1:
            frameStart = SDL_GetTicks();
            switch (poziom)
            {
            case 0:
                SDL_RenderCopy(renderer, background, NULL, &(SDL_Rect) {0, 0, 1280, 640});
                break;

            case 1:
                SDL_RenderCopy(renderer, background1, NULL, &(SDL_Rect) {0, 0, 1280, 640});
                                                                                                         //show background
            }
            
                

            switch (Model) {
            case 0:
                SDL_RenderCopy(renderer, AnimationBkMv[AniFrame], NULL, &(SDL_Rect) {x, y, 36 * 3.5, 64 * 3.5});    //show character model back when moving baton
                break;
            case 1:
                SDL_RenderCopy(renderer, AnimationBkWk[AniFrame], NULL, &(SDL_Rect) {x, y, 36 * 3.5, 64 * 3.5});    //show character model walking up
                break;
            case 2:
                SDL_RenderCopy(renderer, AnimationFWk[AniFrame], NULL, &(SDL_Rect) {x, y, 36 * 3.5, 64 * 3.5});     //show character model walking down
                break;
            case 3:
                SDL_RenderCopy(renderer, AnimationLWk[AniFrame], NULL, &(SDL_Rect) {x, y, 36 * 3.5, 64 * 3.5});     //show character model walking left
                break;
            case 4:
                SDL_RenderCopy(renderer, AnimationRWk[AniFrame], NULL, &(SDL_Rect) {x, y, 36 * 3.5, 64 * 3.5});     //show character model walking right
                break;
            }


            SDL_RenderCopy(renderer, NapisScore, NULL, &(SDL_Rect) {950, 125, 176, 42});                            //show "Score"

            SDL_RenderCopy(renderer, AnimationScore[score], NULL, &(SDL_Rect) {1145, 120, 108, 50});               //show real score




            lose_conditions(Przegrana, renderer);
            loop_enemy();



            bullet1.hitbox.y -= 10; //pocisk leci do góry


            enemy_hit_collision();


            if (bullet1.hitbox.y <= 0)
                bullet1.state = 0;

            //je?li przeciwnicy istniej? to narysuj 
            int a = 240;
            for (int i = 0; i < 5; i++) {

                {
                    if (enemy[i].alive == 1)
                    {

                        switch (enemies)                //zmiana wygl?du przeciwników
                        {
                        case 0:
                            SDL_RenderCopy(renderer, CwiercNuta, NULL, &(SDL_Rect) {a, enemy[i].hitbox.y, 1.5 * 18, 1.5 * 32});
                            break;

                        case 1:
                            SDL_RenderCopy(renderer, PolNuta, NULL, &(SDL_Rect) {a, enemy[i].hitbox.y, 1.5 * 18, 1.5 * 32});
                            break;

                        case 2:
                            SDL_RenderCopy(renderer, CalaNuta, NULL, &(SDL_Rect) {a, enemy[i].hitbox.y, 1.5 * 18, 1.5 * 32});
                            break;

                        }
                    }

                    a += 90;

                }


            }
            //draw bullet if exists
            if (bullet1.state == 1)
            {

                //show bullet model

                switch (Gun)
                {
                case 0:
                    SDL_RenderCopy(renderer, PauzaOsem, NULL, &(SDL_Rect) {bullet1.hitbox.x, bullet1.hitbox.y, 19, 28});
                    SDL_RenderCopy(renderer, bron1, NULL, &(SDL_Rect) {920, 479, 114, 114});
                    break;

                case 1:
                    SDL_RenderCopy(renderer, PauzaCwierc, NULL, &(SDL_Rect) {bullet1.hitbox.x, bullet1.hitbox.y, 1.5 * 19, 1.5 * 28});
                    SDL_RenderCopy(renderer, bron2, NULL, &(SDL_Rect) { 1040, 479, 114, 114});
                    break;
                }
            }

            //move enemies down

            for (int c = 0; c < 5; c++)
                enemy[c].hitbox.y += 1;


            break;

        case 2:

            SDL_RenderCopy(renderer, Wygrana, NULL, &(SDL_Rect) {0, 0, 1280, 640});                   //show win
            break;

        case 3:
            SDL_RenderCopy(renderer, Resume, NULL, &(SDL_Rect) {0, 0, 1280, 640 });
            break;
        }




        //show what was drawn
        SDL_RenderPresent(renderer);

        SDL_RenderClear(renderer);

        //time function 
        frameTime = SDL_GetTicks() - frameStart;

        if (frameDelay > frameTime)
        {
            SDL_Delay(frameDelay - frameTime);
        }
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}

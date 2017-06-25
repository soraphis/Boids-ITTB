//
// Created by oliver on 07.06.17.
//

#include <SDL2/SDL.h>
#include <iostream>
#include "BoidView.h"
#include "../core/Boid.h"
#include "../core/Vector3D.h"
#include "../core/Vector2D.h"

// #define Vector3D Vector2D

/* ----------------------- */
void draw_quad(SDL_Renderer* &ren, int x, int y){
    SDL_RenderDrawPoint(ren, x, y);
    SDL_RenderDrawPoint(ren, x+1, y);
    SDL_RenderDrawPoint(ren, x, y+1);
    SDL_RenderDrawPoint(ren, x+1, y+1);
}

void draw_digit(int i, SDL_Renderer* &ren, int &x, int y){
    switch(i % 10){
        case 0:
        {
            int arr[5][3] = { {1, 1, 1}, {1, 0, 1}, {1, 0, 1}, {1, 0, 1}, {1, 1, 1}};
            for(int r = 0; r < 5; ++r){
                for(int c = 0; c < 3; ++c) {
                    if(arr[r][c] == 0) continue;
                    draw_quad(ren, x+c*2, y+r*2);
                }
            }
            x += 4*2;
            break;
        }
        case 1:
        {
            int arr[5] = { 1, 1, 1, 1, 1};
            for(int r = 0; r < 5; ++r){
                draw_quad(ren, x, y+r*2);
            }
            x += 2*2;
            break;
        }
        case 2:
        {
            int arr[5][3] = { {1, 1, 1}, {0, 0, 1}, {0, 1, 0}, {1, 0, 0}, {1, 1, 1}};
            for(int r = 0; r < 5; ++r){
                for(int c = 0; c < 3; ++c) {
                    if(arr[r][c] == 0) continue;
                    draw_quad(ren, x+c*2, y+r*2);
                }
            }
            x += 4*2;
            break;
        }
        case 3:
        {
            int arr[5][3] = { {1, 1, 1}, {0, 0, 1}, {1, 1, 1}, {0, 0, 1}, {1, 1, 1}};
            for(int r = 0; r < 5; ++r){
                for(int c = 0; c < 3; ++c) {
                    if(arr[r][c] == 0) continue;
                    draw_quad(ren, x+c*2, y+r*2);
                }
            }
            x += 4*2;
            break;
        }
        case 4:
        {
            int arr[5][3] = { {1, 0, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 1}, {0, 0, 1}};
            for(int r = 0; r < 5; ++r){
                for(int c = 0; c < 3; ++c) {
                    if(arr[r][c] == 0) continue;
                    draw_quad(ren, x+c*2, y+r*2);
                }
            }
            x += 4*2;
            break;
        }
        case 5:
        {
            int arr[5][3] = { {1, 1, 1}, {1, 0, 0}, {1, 1, 0}, {0, 0, 1}, {1, 1, 0}};
            for(int r = 0; r < 5; ++r){
                for(int c = 0; c < 3; ++c) {
                    if(arr[r][c] == 0) continue;
                    draw_quad(ren, x+c*2, y+r*2);
                }
            }
            x += 4*2;
            break;
        }
        case 6:
        {
            int arr[5][3] = { {0, 0, 1}, {0, 1, 0}, {1, 1, 0}, {1, 0, 1}, {0, 1, 0}};
            for(int r = 0; r < 5; ++r){
                for(int c = 0; c < 3; ++c) {
                    if(arr[r][c] == 0) continue;
                    draw_quad(ren, x+c*2, y+r*2);
                }
            }
            x += 4*2;
            break;
        }
        case 7:
        {
            int arr[5][3] = { {1, 1, 1}, {0, 0, 1}, {0, 1, 0}, {0, 1, 0}, {0, 1, 0}};
            for(int r = 0; r < 5; ++r){
                for(int c = 0; c < 3; ++c) {
                    if(arr[r][c] == 0) continue;
                    draw_quad(ren, x+c*2, y+r*2);
                }
            }
            x += 4*2;
            break;
        }
        case 8:
        {
            int arr[5][3] = { {1, 1, 1}, {1, 0, 1}, {1, 1, 1}, {1, 0, 1}, {1, 1, 1}};
            for(int r = 0; r < 5; ++r){
                for(int c = 0; c < 3; ++c) {
                    if(arr[r][c] == 0) continue;
                    draw_quad(ren, x+c*2, y+r*2);
                }
            }
            x += 4*2;
            break;
        }
        case 9:
        {
            int arr[5][3] = { {1, 1, 1}, {1, 0, 1}, {1, 1, 1}, {0, 0, 1}, {1, 1, 1}};
            for(int r = 0; r < 5; ++r){
                for(int c = 0; c < 3; ++c) {
                    if(arr[r][c] == 0) continue;
                    draw_quad(ren, x+c*2, y+r*2);
                }
            }
            x += 4*2;
            break;
        }
    }
}


void draw_number(int number, SDL_Renderer* &ren, int &x, int y){
    if(number < 0) return;
    if(number < 10){
        draw_digit(number, ren, x, y);
    }else{
        draw_number(number/10, ren, x, y);
        draw_digit(number, ren, x, y);
    }
}

/* ----------------------- */


void BoidView::init() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_CreateWindowAndRenderer(900, 900, SDL_WINDOW_OPENGL, &window, &ren);
    this->surface = SDL_GetWindowSurface( window );

    // SDL_FillRect( surface, NULL, SDL_MapRGB( surface->format, 0x00, 0x00, 0x00 ) );


    // this->ren = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SDL_RenderSetLogicalSize(this->ren, 225, 225);
    SDL_SetRenderDrawBlendMode(this->ren, SDL_BLENDMODE_BLEND);
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" );

    this->sw = Stopwatch();
    sw.start();
}

bool received_quit(){
    SDL_Event e;
    SDL_PollEvent(&e);
    return e.type == SDL_QUIT;
}

inline void draw_horizontal_line_at(SDL_Renderer* &ren, int y, int width){
    SDL_RenderDrawLine(ren, 0, y, width, y);
}

inline void draw_vertical_line_at(SDL_Renderer* &ren, int x, int height){
    SDL_RenderDrawLine(ren, x, 0, x, height);
}

void draw_grid(SDL_Renderer* &ren){
    int width, height;
    SDL_RenderGetLogicalSize(ren, &width, &height);

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 25);

    draw_horizontal_line_at(ren, height/2, width);
    draw_vertical_line_at(ren, width/2, height);


    for(int y = -50; y <= 50; y += 10){
        draw_horizontal_line_at(ren, height/2 + y*2, width);
    }

    for(int x = -50; x <= 50; x += 10){
        draw_vertical_line_at(ren, width/2 + x*2, height);
    }
}


int BoidView::update(int ms, maptype &swarm) {
    if(received_quit()) return 1;

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren);
    // ----------------------------------------
    int width, height;
    SDL_RenderGetLogicalSize(this->ren, &width, &height);

    draw_grid(ren);


    SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0xFF, 255);
    int x = 3;
    draw_number(ms, ren, x, 3);


    for(auto s = swarm.begin(); s != swarm.end(); ++s){

        for(auto b : s->second){
            //*
            uint8_t red = (uint8_t)( (1- (float)std::get<2>(s->first) / 5) * 255);
            uint8_t blue = (uint8_t)(((float)std::get<2>(s->first) / 5) * 255);
            uint8_t green = (uint8_t)(((float)std::get<0>(s->first) / 5) * 255);
            /*/
            uint8_t red = (uint8_t)( (1- (float)b.pos.z() / height*8) * 255);
            uint8_t blue = (uint8_t)(((float)b.pos.z() /    height*8) * 255);
            uint8_t green = (uint8_t)(((float)b.pos.x() /   width *8) * 255);
            //*/

            SDL_SetRenderDrawColor(ren, red, green, blue, 255);
            SDL_RenderDrawPoint(ren, (int)(b.pos.x()*2) + width/2, (int)(b.pos.z()*2) + height/2);
        }
    }



    // ----------------------------------------
    SDL_RenderPresent(ren);
    return 0;
}

int BoidView::update(int ms, Boid** swarm, int SWARM_SIZE) {
    if(received_quit()) return 1;

    sw.stop();

    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren);

    SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0xFF, 255);
    int x = 3;
    draw_number(ms, ren, x, 3);

    for(int x = 0; x < SWARM_SIZE; ++x){
        for(int y = 0; y < SWARM_SIZE; ++y){
            for(int z = 0; z < SWARM_SIZE; ++z) {
                Boid* s = swarm[x*SWARM_SIZE*SWARM_SIZE + y*SWARM_SIZE + z];
                if(! s) continue;
                //*
                uint8_t red = (uint8_t)( (1- (float)z / SWARM_SIZE) * 255);
                uint8_t blue = (uint8_t)(((float)z / SWARM_SIZE) * 255);
                uint8_t green = (uint8_t)(((float)x / SWARM_SIZE) * 255);
                /*/
                uint8_t red = (uint8_t)( (1- (float)s->pos.z() / SWARM_SIZE) * 255);
                uint8_t blue = (uint8_t)(((float)s->pos.z() / SWARM_SIZE) * 255);
                uint8_t green = (uint8_t)(((float)s->pos.x() / SWARM_SIZE) * 255);
                //*/

                SDL_SetRenderDrawColor(ren, red, green, blue, 255);
                draw_quad(ren, (int)s->pos.x() + 400, (int)s->pos.z() + 225);
            }   }   }

    SDL_RenderPresent(ren);

    sw.start();
    return 0;
}

int BoidView::update(int ms, std::vector<Boid> &swarm) {
    if(received_quit()) return 1;
//    if(sw.elapsedMilliseconds() < 30){
//        return 0;
//    }
    sw.stop();

    //SDL_SetRenderDrawColor( ren, 0x00, 0x00, 0x00, 0xFF );
    //SDL_RenderClear( ren );

    int width, height;
    SDL_RenderGetLogicalSize(this->ren, &width, &height);


    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
    SDL_RenderClear(ren);

    // SDL_SetRenderDrawColor(ren, 0xCC, 0xCC, 0xCC, 180);
    // SDL_RenderDrawPoint(ren, width/2, height/2);

    draw_grid(ren);

    SDL_SetRenderDrawColor(ren, 0x00, 0x00, 0xFF, 255);
    int x = 3;
    draw_number(ms, ren, x, 3);
//    while(ms > 0){
//        draw_digit(ms%10, ren, x, 3);
//        ms /= 10;
//    }


    int i = 0;
    for(auto b : swarm){
        uint8_t red =   (uint8_t)(255 - ((float(i))/swarm.size()) *255 );
        uint8_t blue =  (uint8_t)(255 - red);
        uint8_t green =  (uint8_t)(max(0, (max(red, blue)) - 245)* 20);


        SDL_SetRenderDrawColor(ren, red, green, blue, 200);

        SDL_RenderDrawPoint(ren, (int)(b.pos.x()*2) + width/2, (int)(b.pos.z()*2) + height/2);
        // draw_quad(ren, (int)b.pos.x() + width/2, (int)b.pos.z() + height/2);
        ++i;
    }


    SDL_RenderPresent(ren);


    sw.start();
    return 0;
}

void BoidView::close() {
    SDL_DestroyWindow(window);
    SDL_Quit();
}


// #undef Vector3D
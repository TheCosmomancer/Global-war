/*
MIT License

Copyright (c) 2025 Farbod Khalili Khoshehmehr

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <string>
#include <algorithm>
#include <chrono>
#include <random>
#include <array>
#include <fstream>
#include "raylib.h"
//raylib is needed to run this code which can be downloaded at raylib.com
struct playerstruct {
    char color;
    int troop = 0;
    int star = 0;
    bool alive = 1;
};
struct countrystruct {
    int controller = -1;
    int troops = 0;
    int starnumber = 1;
};
enum screen { menu, game, scoreboard, tutorial };
enum gamephase { start, deploytroops, attackdefend, movetroops, end };
playerstruct player[3];
// players are R/red , B/blue and G/green. amount of troops a player can delpoy at the first phase and the stars they have is tracked
countrystruct country[42];
// countries contain a number to know which one they are , track their controller , troops deployed on them , and the number of starts their card gives
int temp;
int temp2;
int temp3;
const int screenWidth = 1920;
const int screenHeight = 950;
screen currentscreen = menu;
gamephase currentgamephase = start;
int currentgamephasephase = 0;
int currentplayer = 0;
bool gamestartup = 1;
bool gameover = 0;
int countryinput1;
int countryinput2;
int numberinput;
bool countrycapturnedthisturn = 0;
int carddrawtracker = 0;
bool moveaftercapture = 0;
bool fastcombat = 0;
bool musicison = 0;
std::fstream scoretxt;
std::string scorelinetxt[10];
char winner ;
bool calcwinner = 0;
char currentcountrycolor = 'R';
char currentcountrytroopcount[3] = { '0','0','0'};
long long int timedump;
long long int lasttimenotingame;
char timerchar[7] = { '0',':','0','0',':','0','0' };
const int starexchangemap[11]{ 0,0,2,4,7,10,13,17,21,25,30 };
std::array<int, 43> shufflehelp{ 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42 };
std::array<int, 18> attackerdice{ 1,1,1,2,2,2,3,3,3,4,4,4,5,5,5,6,6,6 };
std::array<int, 12> defenderdice{ 1,1,2,2,3,3,4,4,5,5,6,6 };
std::array<int, 3> playershufflehelp{ 0,1,2 };
const bool relationmap[42][42] = {
0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,
1,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	1,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
1,	1,	0,	0,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	1,	1,	1,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	1,	1,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	1,	0,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	1,	1,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	1,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	0,	0,	0,	0,	0,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	1,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	1,	0,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	0,	1,	1,	1,	0,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	1,	0,	1,	1,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,	1,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	0,	1,	0,	0,	0,	0,	0,
1,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	1,	0,	1,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	1,	0,	0,	0,	0,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	0,	1,	1,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0,	1,
0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	0,	1,	1,	0
};
bool alreadytreversed[42];
bool relationfinder(int, int);
int main(void)
{
    // initialization
    scoretxt.open("scoreboard.txt", std::fstream::in);
    for (int i = 0; i < 10; i++) {
        std::getline(scoretxt, scorelinetxt[i]);
    }
    scoretxt.close();
    for (int i = 0; i < 42; i++)
        alreadytreversed[i] = 0;
    InitWindow(screenWidth, screenHeight, "globalwar");
    InitAudioDevice();
    Music music = LoadMusicStream("game.mp3");
    music.looping = true;
    PlayMusicStream(music);
    PauseMusicStream(music);
    SetTargetFPS(60);
    //main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        UpdateMusicStream(music);
        //main game cycle
        switch (currentscreen)
        {
        case tutorial:
        {
            if (IsKeyPressed(KEY_B) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
                currentscreen = menu;
        }break;
        case scoreboard:
        {
            if (IsKeyPressed(KEY_B) || IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
                currentscreen = menu;
        }break;
        case menu:
        {
            lasttimenotingame = GetTime();
            //these change the screen if we are on the main menu
            if (IsKeyPressed(KEY_P)) {
                currentscreen = game;
            }
            if (IsKeyPressed(KEY_T)) {
                currentscreen = tutorial;
            }
            if (IsKeyPressed(KEY_S)) {
                currentscreen = scoreboard;
            }
            if (IsKeyPressed(KEY_F)) {
                fastcombat = !fastcombat;
            }
            if (IsKeyPressed(KEY_M)) {
                if (musicison) PauseMusicStream(music);
                else ResumeMusicStream(music);
                musicison = !musicison;
            }
        }break;
        case game:
        {
            if (!gameover)
                switch (gamestartup)
                {
                case 1: {
                    //assigne players countries and give them starting troops
                    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                    shuffle(shufflehelp.begin(), shufflehelp.end(), std::default_random_engine(seed));
                    shuffle(playershufflehelp.begin(), playershufflehelp.end(), std::default_random_engine(seed));
                    shuffle(defenderdice.begin(), defenderdice.end(), std::default_random_engine(seed));
                    temp = 0;
                    while (shufflehelp[temp] != 42) { temp++; }
                    temp2 = shufflehelp[11 + defenderdice[0]];
                    shufflehelp[11 + defenderdice[0]] = 42;
                    shufflehelp[temp] = temp2;
                    player[playershufflehelp[0]].color = 'R';
                    player[playershufflehelp[1]].color = 'B';
                    player[playershufflehelp[2]].color = 'G';
                    temp = 0;
                    for (int i = 0; i < 42; i++) {
                        if (shufflehelp[temp] == 42)
                            temp++;
                        country[shufflehelp[temp]].controller = (i / 14);
                        if (i == 1 || i == 7 || i == 9 || i == 10 || i == 15 || i == 19 || i == 24 || i == 26 || i == 31 || i == 33 || i == 41)
                            country[i].starnumber++;
                        country[i].troops = country[i].starnumber;
                        temp++;
                    }
                    gamestartup = 0;
                }
                case 0: {
                    timedump = GetTime() - lasttimenotingame;
                    temp = timedump % 60;
                    timedump /= 60;
                    temp2 = timedump % 60;
                    timedump /= 60;
                    timerchar[6] = (temp % 10) + '0';
                    timerchar[5] = (temp / 10) + '0';
                    timerchar[3] = (temp2 % 10) + '0';
                    timerchar[2] = (temp2 / 10) + '0';
                    timerchar[0] = (timedump)+'0';
                    if (currentgamephasephase == 0 && currentgamephase != end && currentgamephase != start) {
                        //1
                        if (IsKeyDown(KEY_A))
                            if (IsKeyPressed(KEY_S)) {
                                countryinput1 = 0;
                                currentgamephasephase++;
                            }
                        //2
                        if (IsKeyDown(KEY_N))
                            if (IsKeyPressed(KEY_W)) {
                                countryinput1 = 1;
                                currentgamephasephase++;
                            }
                        //3
                        if (IsKeyDown(KEY_G))
                            if (IsKeyPressed(KEY_R)) {
                                countryinput1 = 2;
                                currentgamephasephase++;
                            }
                        //4
                        if (IsKeyDown(KEY_A))
                            if (IsKeyPressed(KEY_B)) {
                                countryinput1 = 3;
                                currentgamephasephase++;
                            }
                        //5
                        if (IsKeyDown(KEY_O))
                            if (IsKeyPressed(KEY_N)) {
                                countryinput1 = 4;
                                currentgamephasephase++;
                            }
                        //6
                        if (IsKeyDown(KEY_Q))
                            if (IsKeyPressed(KEY_U)) {
                                countryinput1 = 5;
                                currentgamephasephase++;
                            }
                        //7
                        if (IsKeyDown(KEY_W))
                            if (IsKeyPressed(KEY_U)) {
                                countryinput1 = 6;
                                currentgamephasephase++;
                            }
                        //8
                        if (IsKeyDown(KEY_E))
                            if (IsKeyPressed(KEY_U)) {
                                countryinput1 = 7;
                                currentgamephasephase++;
                            }
                        //9
                        if (IsKeyDown(KEY_C))
                            if (IsKeyPressed(KEY_U)) {
                                countryinput1 = 8;
                                currentgamephasephase++;
                            }
                        //10
                        if (IsKeyDown(KEY_V))
                            if (IsKeyPressed(KEY_N)) {
                                countryinput1 = 9;
                                currentgamephasephase++;
                            }
                        //11
                        if (IsKeyDown(KEY_B))
                            if (IsKeyPressed(KEY_R)) {
                                countryinput1 = 10;
                                currentgamephasephase++;
                            }
                        //12
                        if (IsKeyDown(KEY_P))
                            if (IsKeyPressed(KEY_R)) {
                                countryinput1 = 11;
                                currentgamephasephase++;
                            }
                        //13
                        if (IsKeyDown(KEY_A))
                            if (IsKeyPressed(KEY_G)) {
                                countryinput1 = 12;
                                currentgamephasephase++;
                            }
                        //14
                        if (IsKeyDown(KEY_I))
                            if (IsKeyPressed(KEY_C)) {
                                countryinput1 = 13;
                                currentgamephasephase++;
                            }
                        //15
                        if (IsKeyDown(KEY_G))
                            if (IsKeyPressed(KEY_B)) {
                                countryinput1 = 14;
                                currentgamephasephase++;
                            }
                        //16
                        if (IsKeyDown(KEY_S))
                            if (IsKeyPressed(KEY_C)) {
                                countryinput1 = 15;
                                currentgamephasephase++;
                            }
                        //17
                        if (IsKeyDown(KEY_N))
                            if (IsKeyPressed(KEY_E)) {
                                countryinput1 = 16;
                                currentgamephasephase++;
                            }
                        //18
                        if (IsKeyDown(KEY_W))
                            if (IsKeyPressed(KEY_E)) {
                                countryinput1 = 17;
                                currentgamephasephase++;
                            }
                        //19
                        if (IsKeyDown(KEY_S))
                            if (IsKeyPressed(KEY_E)) {
                                countryinput1 = 18;
                                currentgamephasephase++;
                            }
                        //20
                        if (IsKeyDown(KEY_U))
                            if (IsKeyPressed(KEY_R)) {
                                countryinput1 = 19;
                                currentgamephasephase++;
                            }
                        //21
                        if (IsKeyDown(KEY_N))
                            if (IsKeyPressed(KEY_A)) {
                                countryinput1 = 20;
                                currentgamephasephase++;
                            }
                        //22
                        if (IsKeyDown(KEY_E))
                            if (IsKeyPressed(KEY_G)) {
                                countryinput1 = 21;
                                currentgamephasephase++;
                            }
                        //23
                        if (IsKeyDown(KEY_E))
                            if (IsKeyPressed(KEY_A)) {
                                countryinput1 = 22;
                                currentgamephasephase++;
                            }
                        //24
                        if (IsKeyDown(KEY_C))
                            if (IsKeyPressed(KEY_G)) {
                                countryinput1 = 23;
                                currentgamephasephase++;
                            }
                        //25
                        if (IsKeyDown(KEY_S))
                            if (IsKeyPressed(KEY_A)) {
                                countryinput1 = 24;
                                currentgamephasephase++;
                            }
                        //26
                        if (IsKeyDown(KEY_M))
                            if (IsKeyPressed(KEY_D)) {
                                countryinput1 = 25;
                                currentgamephasephase++;
                            }
                        //27
                        if (IsKeyDown(KEY_M))
                            if (IsKeyPressed(KEY_E)) {
                                countryinput1 = 26;
                                currentgamephasephase++;
                            }
                        //28
                        if (IsKeyDown(KEY_A))
                            if (IsKeyPressed(KEY_F)) {
                                countryinput1 = 27;
                                currentgamephasephase++;
                            }
                        //29
                        if (IsKeyDown(KEY_U))
                            if (IsKeyPressed(KEY_L)) {
                                countryinput1 = 28;
                                currentgamephasephase++;
                            }
                        //30
                        if (IsKeyDown(KEY_I))
                            if (IsKeyPressed(KEY_N)) {
                                countryinput1 = 29;
                                currentgamephasephase++;
                            }
                        //31
                        if (IsKeyDown(KEY_C))
                            if (IsKeyPressed(KEY_H)) {
                                countryinput1 = 30;
                                currentgamephasephase++;
                            }
                        //32
                        if (IsKeyDown(KEY_S))
                            if (IsKeyPressed(KEY_M)) {
                                countryinput1 = 31;
                                currentgamephasephase++;
                            }
                        //33
                        if (IsKeyDown(KEY_S))
                            if (IsKeyPressed(KEY_B)) {
                                countryinput1 = 32;
                                currentgamephasephase++;
                            }
                        //34
                        if (IsKeyDown(KEY_M))
                            if (IsKeyPressed(KEY_N)) {
                                countryinput1 = 33;
                                currentgamephasephase++;
                            }
                        //35
                        if (IsKeyDown(KEY_I))
                            if (IsKeyPressed(KEY_K)) {
                                countryinput1 = 34;
                                currentgamephasephase++;
                            }
                        //36
                        if (IsKeyDown(KEY_Y))
                            if (IsKeyPressed(KEY_K)) {
                                countryinput1 = 35;
                                currentgamephasephase++;
                            }
                        //37
                        if (IsKeyDown(KEY_K))
                            if (IsKeyPressed(KEY_C)) {
                                countryinput1 = 36;
                                currentgamephasephase++;
                            }
                        //38
                        if (IsKeyDown(KEY_J))
                            if (IsKeyPressed(KEY_P)) {
                                countryinput1 = 37;
                                currentgamephasephase++;
                            }
                        //39
                        if (IsKeyDown(KEY_I))
                            if (IsKeyPressed(KEY_D)) {
                                countryinput1 = 38;
                                currentgamephasephase++;
                            }
                        //40
                        if (IsKeyDown(KEY_N))
                            if (IsKeyPressed(KEY_G)) {
                                countryinput1 = 39;
                                currentgamephasephase++;
                            }
                        //41
                        if (IsKeyDown(KEY_W))
                            if (IsKeyPressed(KEY_A)) {
                                countryinput1 = 40;
                                currentgamephasephase++;
                            }
                        //42
                        if (IsKeyDown(KEY_A))
                            if (IsKeyPressed(KEY_U)) {
                                countryinput1 = 41;
                                currentgamephasephase++;
                            }
                        if (currentgamephase == deploytroops && player[currentplayer].star > 1)
                            if (IsKeyPressed(KEY_T)) {
                                temp = player[currentplayer].star;
                                if (temp > 10)
                                    temp = 10;
                                player[currentplayer].troop += starexchangemap[temp];
                                player[currentplayer].star -= temp;
                            }
                        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
                            if (currentgamephase == deploytroops)
                                currentgamephase = attackdefend;
                            else if (currentgamephase == attackdefend)
                                currentgamephase = movetroops;
                            else if (currentgamephase == movetroops)
                                currentgamephase = end;
                        }
                        if (IsKeyPressed(KEY_F) && IsKeyUp(KEY_A)) {
                            fastcombat = !fastcombat;
                        }
                    }
                    else if ((currentgamephase == attackdefend || currentgamephase == movetroops) && currentgamephasephase == 1)
                    {
                        //1
                        if (IsKeyDown(KEY_A))
                            if (IsKeyPressed(KEY_S)) {
                                countryinput2 = 0;
                                currentgamephasephase++;
                            }
                        //2
                        if (IsKeyDown(KEY_N))
                            if (IsKeyPressed(KEY_W)) {
                                countryinput2 = 1;
                                currentgamephasephase++;
                            }
                        //3
                        if (IsKeyDown(KEY_G))
                            if (IsKeyPressed(KEY_R)) {
                                countryinput2 = 2;
                                currentgamephasephase++;
                            }
                        //4
                        if (IsKeyDown(KEY_A))
                            if (IsKeyPressed(KEY_B)) {
                                countryinput2 = 3;
                                currentgamephasephase++;
                            }
                        //5
                        if (IsKeyDown(KEY_O))
                            if (IsKeyPressed(KEY_N)) {
                                countryinput2 = 4;
                                currentgamephasephase++;
                            }
                        //6
                        if (IsKeyDown(KEY_Q))
                            if (IsKeyPressed(KEY_U)) {
                                countryinput2 = 5;
                                currentgamephasephase++;
                            }
                        //7
                        if (IsKeyDown(KEY_W))
                            if (IsKeyPressed(KEY_U)) {
                                countryinput2 = 6;
                                currentgamephasephase++;
                            }
                        //8
                        if (IsKeyDown(KEY_E))
                            if (IsKeyPressed(KEY_U)) {
                                countryinput2 = 7;
                                currentgamephasephase++;
                            }
                        //9
                        if (IsKeyDown(KEY_C))
                            if (IsKeyPressed(KEY_U)) {
                                countryinput2 = 8;
                                currentgamephasephase++;
                            }
                        //10
                        if (IsKeyDown(KEY_V))
                            if (IsKeyPressed(KEY_N)) {
                                countryinput2 = 9;
                                currentgamephasephase++;
                            }
                        //11
                        if (IsKeyDown(KEY_B))
                            if (IsKeyPressed(KEY_R)) {
                                countryinput2 = 10;
                                currentgamephasephase++;
                            }
                        //12
                        if (IsKeyDown(KEY_P))
                            if (IsKeyPressed(KEY_R)) {
                                countryinput2 = 11;
                                currentgamephasephase++;
                            }
                        //13
                        if (IsKeyDown(KEY_A))
                            if (IsKeyPressed(KEY_G)) {
                                countryinput2 = 12;
                                currentgamephasephase++;
                            }
                        //14
                        if (IsKeyDown(KEY_I))
                            if (IsKeyPressed(KEY_C)) {
                                countryinput2 = 13;
                                currentgamephasephase++;
                            }
                        //15
                        if (IsKeyDown(KEY_G))
                            if (IsKeyPressed(KEY_B)) {
                                countryinput2 = 14;
                                currentgamephasephase++;
                            }
                        //16
                        if (IsKeyDown(KEY_S))
                            if (IsKeyPressed(KEY_C)) {
                                countryinput2 = 15;
                                currentgamephasephase++;
                            }
                        //17
                        if (IsKeyDown(KEY_N))
                            if (IsKeyPressed(KEY_E)) {
                                countryinput2 = 16;
                                currentgamephasephase++;
                            }
                        //18
                        if (IsKeyDown(KEY_W))
                            if (IsKeyPressed(KEY_E)) {
                                countryinput2 = 17;
                                currentgamephasephase++;
                            }
                        //19
                        if (IsKeyDown(KEY_S))
                            if (IsKeyPressed(KEY_E)) {
                                countryinput2 = 18;
                                currentgamephasephase++;
                            }
                        //20
                        if (IsKeyDown(KEY_U))
                            if (IsKeyPressed(KEY_R)) {
                                countryinput2 = 19;
                                currentgamephasephase++;
                            }
                        //21
                        if (IsKeyDown(KEY_N))
                            if (IsKeyPressed(KEY_A)) {
                                countryinput2 = 20;
                                currentgamephasephase++;
                            }
                        //22
                        if (IsKeyDown(KEY_E))
                            if (IsKeyPressed(KEY_G)) {
                                countryinput2 = 21;
                                currentgamephasephase++;
                            }
                        //23
                        if (IsKeyDown(KEY_E))
                            if (IsKeyPressed(KEY_A)) {
                                countryinput2 = 22;
                                currentgamephasephase++;
                            }
                        //24
                        if (IsKeyDown(KEY_C))
                            if (IsKeyPressed(KEY_G)) {
                                countryinput2 = 23;
                                currentgamephasephase++;
                            }
                        //25
                        if (IsKeyDown(KEY_S))
                            if (IsKeyPressed(KEY_A)) {
                                countryinput2 = 24;
                                currentgamephasephase++;
                            }
                        //26
                        if (IsKeyDown(KEY_M))
                            if (IsKeyPressed(KEY_D)) {
                                countryinput2 = 25;
                                currentgamephasephase++;
                            }
                        //27
                        if (IsKeyDown(KEY_M))
                            if (IsKeyPressed(KEY_E)) {
                                countryinput2 = 26;
                                currentgamephasephase++;
                            }
                        //28
                        if (IsKeyDown(KEY_A))
                            if (IsKeyPressed(KEY_F)) {
                                countryinput2 = 27;
                                currentgamephasephase++;
                            }
                        //29
                        if (IsKeyDown(KEY_U))
                            if (IsKeyPressed(KEY_L)) {
                                countryinput2 = 28;
                                currentgamephasephase++;
                            }
                        //30
                        if (IsKeyDown(KEY_I))
                            if (IsKeyPressed(KEY_N)) {
                                countryinput2 = 29;
                                currentgamephasephase++;
                            }
                        //31
                        if (IsKeyDown(KEY_C))
                            if (IsKeyPressed(KEY_H)) {
                                countryinput2 = 30;
                                currentgamephasephase++;
                            }
                        //32
                        if (IsKeyDown(KEY_S))
                            if (IsKeyPressed(KEY_M)) {
                                countryinput2 = 31;
                                currentgamephasephase++;
                            }
                        //33
                        if (IsKeyDown(KEY_S))
                            if (IsKeyPressed(KEY_B)) {
                                countryinput2 = 32;
                                currentgamephasephase++;
                            }
                        //34
                        if (IsKeyDown(KEY_M))
                            if (IsKeyPressed(KEY_N)) {
                                countryinput2 = 33;
                                currentgamephasephase++;
                            }
                        //35
                        if (IsKeyDown(KEY_I))
                            if (IsKeyPressed(KEY_K)) {
                                countryinput2 = 34;
                                currentgamephasephase++;
                            }
                        //36
                        if (IsKeyDown(KEY_Y))
                            if (IsKeyPressed(KEY_K)) {
                                countryinput2 = 35;
                                currentgamephasephase++;
                            }
                        //37
                        if (IsKeyDown(KEY_K))
                            if (IsKeyPressed(KEY_C)) {
                                countryinput2 = 36;
                                currentgamephasephase++;
                            }
                        //38
                        if (IsKeyDown(KEY_J))
                            if (IsKeyPressed(KEY_P)) {
                                countryinput2 = 37;
                                currentgamephasephase++;
                            }
                        //39
                        if (IsKeyDown(KEY_I))
                            if (IsKeyPressed(KEY_D)) {
                                countryinput2 = 38;
                                currentgamephasephase++;
                            }
                        //40
                        if (IsKeyDown(KEY_N))
                            if (IsKeyPressed(KEY_G)) {
                                countryinput2 = 39;
                                currentgamephasephase++;
                            }
                        //41
                        if (IsKeyDown(KEY_W))
                            if (IsKeyPressed(KEY_A)) {
                                countryinput2 = 40;
                                currentgamephasephase++;
                            }
                        //42
                        if (IsKeyDown(KEY_A))
                            if (IsKeyPressed(KEY_U)) {
                                countryinput2 = 41;
                                currentgamephasephase++;
                            }

                    }
                    else if ((currentgamephase == deploytroops && currentgamephasephase == 1) || (currentgamephase == movetroops && currentgamephasephase == 2)) {
                        //0
                        if (IsKeyPressed(KEY_ZERO) || IsKeyPressed(KEY_KP_0)) {
                            numberinput = 0;
                            currentgamephasephase++;
                        }
                        //1
                        if (IsKeyPressed(KEY_ONE) || IsKeyPressed(KEY_KP_1)) {
                            numberinput = 1;
                            currentgamephasephase++;
                        }
                        //2
                        if (IsKeyPressed(KEY_TWO) || IsKeyPressed(KEY_KP_2)) {
                            numberinput = 2;
                            currentgamephasephase++;
                        }
                        //3
                        if (IsKeyPressed(KEY_THREE) || IsKeyPressed(KEY_KP_3)) {
                            numberinput = 3;
                            currentgamephasephase++;
                        }
                        //4
                        if (IsKeyPressed(KEY_FOUR) || IsKeyPressed(KEY_KP_4)) {
                            numberinput = 4;
                            currentgamephasephase++;
                        }
                        //5
                        if (IsKeyPressed(KEY_FIVE) || IsKeyPressed(KEY_KP_5)) {
                            numberinput = 5;
                            currentgamephasephase++;
                        }
                        //6
                        if (IsKeyPressed(KEY_SIX) || IsKeyPressed(KEY_KP_6)) {
                            numberinput = 6;
                            currentgamephasephase++;
                        }
                        //7
                        if (IsKeyPressed(KEY_SEVEN) || IsKeyPressed(KEY_KP_7)) {
                            numberinput = 7;
                            currentgamephasephase++;
                        }
                        //8
                        if (IsKeyPressed(KEY_EIGHT) || IsKeyPressed(KEY_KP_8)) {
                            numberinput = 8;
                            currentgamephasephase++;
                        }
                        //9
                        if (IsKeyPressed(KEY_NINE) || IsKeyPressed(KEY_KP_9)) {
                            numberinput = 9;
                            currentgamephasephase++;
                        }
                        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER))
                            if (moveaftercapture == 1) {
                                moveaftercapture = 0;
                                currentgamephase = attackdefend;
                                currentgamephasephase = 0;
                            }
                    }
                    else if (currentgamephase == deploytroops && currentgamephasephase == 2) {
                        if (country[countryinput1].controller == currentplayer && player[currentplayer].troop >= numberinput) {
                            country[countryinput1].troops += numberinput;
                            player[currentplayer].troop -= numberinput;
                        }
                        currentgamephasephase = 0;
                    }
                    else if (currentgamephase == attackdefend && currentgamephasephase == 2) {
                        if (relationmap[countryinput1][countryinput2] && country[countryinput1].troops > 1 &&
                            country[countryinput1].controller == currentplayer && country[countryinput2].controller != currentplayer) {
                            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                            shuffle(defenderdice.begin(), defenderdice.end(), std::default_random_engine(seed));
                            shuffle(attackerdice.begin(), attackerdice.end(), std::default_random_engine(seed));
                            temp = (country[countryinput1].troops) - 1;
                            if (temp > 3)
                                temp = 3;
                            for (int i = 0; i < temp-1; i++) {
                                temp3 = attackerdice[i];
                                temp2 = i;
                                for (int j = i+1; j < temp; j++)
                                    if (attackerdice[j] > temp3) {
                                        temp3 = attackerdice[j];
                                        temp2 = j;
                                    }
                                if (temp2 != i){
                                    attackerdice[temp2] = attackerdice[i];
                                    attackerdice[i] = temp3;
                                }
                            }
                            if (country[countryinput2].troops > temp)
                                temp2 = temp;
                            else
                                temp2 = country[countryinput2].troops;
                            if (temp2 > 2)
                                temp2 = 2;
                            if (temp2 > 1 && defenderdice[0] < defenderdice[1]) {
                                temp3 = defenderdice[0];
                                defenderdice[0] = defenderdice[1];
                                defenderdice[1] = temp3;
                            }
                            if (country[countryinput2].troops > 0) {
                                if (fastcombat == 1) {
                                    for (int i = 0; i < temp2; i++) {
                                        if (attackerdice[i] > defenderdice[i])
                                            country[countryinput2].troops -= attackerdice[i] - defenderdice[i];
                                        else if (attackerdice[i] < defenderdice[i])
                                            country[countryinput1].troops -= defenderdice[i] - attackerdice[i];
                                        else
                                            country[countryinput1].troops--;
                                        if (country[countryinput1].troops < 1) {
                                            country[countryinput1].troops = 1;
                                            break;
                                        }
                                        if (country[countryinput2].troops < 0) {
                                            country[countryinput2].troops = 0;
                                            break;
                                        }
                                    }
                                }
                                else {
                                    for (int i = 0; i < temp2; i++)
                                        if (attackerdice[i] > defenderdice[i])
                                            country[countryinput2].troops--;
                                        else
                                            country[countryinput1].troops--;
                                }
                            }
                            if (country[countryinput2].troops == 0) {
                                country[countryinput2].controller = currentplayer;
                                country[countryinput1].troops -= temp;
                                country[countryinput2].troops += temp;
                                countrycapturnedthisturn = 1;
                                moveaftercapture = 1;
                            }
                        }
                        if (moveaftercapture == 1) {
                            currentgamephase = movetroops;
                            currentgamephasephase = 2;
                        }
                        else
                            currentgamephasephase = 0;
                    }
                    else if (currentgamephase == movetroops && currentgamephasephase == 3) {
                        if (country[countryinput1].controller == currentplayer && country[countryinput2].controller == currentplayer
                            && country[countryinput1].troops - numberinput >= 1) {
                            alreadytreversed[countryinput1] = 1;
                            if (country[countryinput1].troops >= numberinput && relationfinder(countryinput1, countryinput2)) {
                                country[countryinput1].troops -= numberinput;
                                country[countryinput2].troops += numberinput;
                            }
                            for (int i = 0; i < 42; i++)
                                alreadytreversed[i] = 0;
                        }
                        if (moveaftercapture == 1)
                            currentgamephasephase = 2;
                        else
                            currentgamephasephase = 0;
                    }
                    else if (currentgamephase == end) {
                        temp = 0;
                        for (int i = 0; i < 42; i++)
                            if (country[i].controller == currentplayer)
                                temp++;
                        if (temp > 24) {
                            calcwinner = 1;
                            gameover = 1;
                        }
                        else {
                            if (countrycapturnedthisturn == 1) {
                                countrycapturnedthisturn = 0;
                                if (shufflehelp[carddrawtracker] == 42) {
                                    calcwinner = 1;
                                    gameover = 1;
                                }
                                else {
                                    player[currentplayer].star += country[shufflehelp[carddrawtracker]].starnumber;
                                    carddrawtracker++;
                                }
                            }
                            for (int i = 0; i < 3; i++)
                                if (player[i].alive == 1) {
                                    temp = 0;
                                    for (int j = 0; j < 42; j++)
                                        if (country[j].controller == i)
                                            temp++;
                                    if (temp == 0)
                                        player[i].alive = 0;
                                }
                            do {
                                currentplayer++;
                                if (currentplayer > 2)
                                    currentplayer = 0;
                            } while (player[currentplayer].alive == 0);
                            currentgamephase = start;
                        }
                    }
                    else if (currentgamephase == start) {
                        player[currentplayer].troop += 3;
                        temp = 0;
                        for (int i = 0; i < 42; i++)
                            if (country[i].controller == currentplayer)
                                temp++;
                        if (temp > 23)
                            player[currentplayer].troop += 5;
                        else if (temp > 20)
                            player[currentplayer].troop += 4;
                        else if (temp > 17)
                            player[currentplayer].troop += 3;
                        else if (temp > 14)
                            player[currentplayer].troop += 2;
                        else if (temp > 11)
                            player[currentplayer].troop += 1;
                        temp = 0;
                        for (int i = 0; i < 9; i++) {
                            if (country[i].controller == currentplayer)
                                temp++;
                        }
                        if (temp == 9)
                            player[currentplayer].troop += 5;
                        temp = 0;
                        for (int i = 9; i < 13; i++) {
                            if (country[i].controller == currentplayer)
                                temp++;
                        }
                        if (temp == 4)
                            player[currentplayer].troop += 2;
                        temp = 0;
                        for (int i = 13; i < 20; i++) {
                            if (country[i].controller == currentplayer)
                                temp++;
                        }
                        if (temp == 7)
                            player[currentplayer].troop += 5;
                        temp = 0;
                        for (int i = 20; i < 26; i++) {
                            if (country[i].controller == currentplayer)
                                temp++;
                        }
                        if (temp == 6)
                            player[currentplayer].troop += 3;
                        temp = 0;
                        for (int i = 26; i < 38; i++) {
                            if (country[i].controller == currentplayer)
                                temp++;
                        }
                        if (temp == 12)
                            player[currentplayer].troop += 7;
                        temp = 0;
                        for (int i = 38; i < 42; i++) {
                            if (country[i].controller == currentplayer)
                                temp++;
                        }
                        if (temp == 4)
                            player[currentplayer].troop += 2;
                        currentgamephase = deploytroops;
                    }
                }
                }
            else {
                while (calcwinner == 1) {
                    temp = 0;
                    temp2 = 0;
                    temp3 = 0;
                    for (int i = 0; i < 42; i++) {
                        if (country[i].controller == 0)
                            temp++;
                        else if (country[i].controller == 1)
                            temp2++;
                        else if (country[i].controller == 2)
                            temp3++;
                    }
                    if (temp > temp2 && temp > temp3)
                        winner = player[0].color;
                    else if (temp2 > temp && temp2 > temp3)
                        winner = player[1].color;
                    else if (temp3 > temp && temp3 > temp2)
                        winner = player[2].color;
                    else {
                        temp = 0;
                        temp2 = 0;
                        temp3 = 0;
                        for (int i = 0; i < 42; i++) {
                            if (country[i].controller == 0)
                                temp += country[i].troops;
                            else if (country[i].controller == 1)
                                temp2 += country[i].troops;
                            else if (country[i].controller == 2)
                                temp3 += country[i].troops;
                        }
                        if (temp > temp2 && temp > temp3)
                            winner = player[0].color;
                        else if (temp2 > temp && temp2 > temp3)
                            winner = player[1].color;
                        else if (temp3 > temp2 && temp3 > temp)
                            winner = player[2].color;
                        else winner = 'D';
                    }
                    scoretxt.open("scoreboard.txt", std::fstream::out);
                    if (winner == 'D')
                        scoretxt << "Draw\n";
                    else if (winner == 'B')
                        scoretxt << "Blue\n";
                    else if (winner == 'R')
                        scoretxt << "Red\n";
                    else if (winner == 'G')
                        scoretxt << "Green\n";
                    scoretxt.close();
                    scoretxt.open("scoreboard.txt", std::fstream::app);
                    for (int i = 0; i < 9; i++)
                        scoretxt << scorelinetxt[i] << '\n';
                    scoretxt.close();
                    calcwinner = 0;
                }
                if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
                    scoretxt.open("scoreboard.txt", std::fstream::in);
                    for (int i = 0; i < 10; i++) {
                        std::getline(scoretxt, scorelinetxt[i]);
                    }
                    scoretxt.close();
                    gamestartup = 1;
                    currentscreen = menu;
                }
            }
        }break;
        }
        // screen updator
        BeginDrawing();

        ClearBackground(BLACK);
        switch (currentscreen)
        {
        case game: {
            if (!gameover) {
                //north america
                //1 AS
                currentcountrycolor = player[country[0].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[0].troops % temp) - (country[0].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(20, 74, 160, 126, ORANGE);
                DrawText("AS", 70, 110, 25, BLACK);
                DrawText(&currentcountrycolor, 120, 110, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 70, 140, 25, BLACK);
                //2 NW
                currentcountrycolor = player[country[1].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[1].troops % temp) - (country[1].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(182, 51, 303, 59, GOLD);
                DrawText("NW", 250, 60, 25, BLACK);
                DrawText(&currentcountrycolor, 300, 60, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 380, 60, 25, BLACK);
                //4 AB
                currentcountrycolor = player[country[3].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[3].troops % temp) - (country[3].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(182, 114, 159, 126, YELLOW);
                DrawText("AB", 200, 120, 25, BLACK);
                DrawText(&currentcountrycolor, 280, 120, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 240, 200, 25, BLACK);
                //7 WU
                currentcountrycolor = player[country[6].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[6].troops % temp) - (country[6].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(191, 241, 184, 92, ORANGE);
                DrawText("WU", 200, 250, 25, BLACK);
                DrawText(&currentcountrycolor, 260, 250, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 330, 250, 25, BLACK);
                //5 ON
                currentcountrycolor = player[country[4].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[4].troops % temp) - (country[4].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(344, 151, 103, 87, GOLD);
                DrawText("ON", 350, 160, 25, BLACK);
                DrawText(&currentcountrycolor, 415, 160, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 388, 200, 25, BLACK);
                //3 GR
                currentcountrycolor = player[country[2].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[2].troops % temp) - (country[2].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(451, 112, 293, 54, YELLOW);
                DrawText("GR", 460, 120, 25, BLACK);
                DrawText(&currentcountrycolor, 520, 120, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 630, 120, 25, BLACK);
                //6 QU
                currentcountrycolor = player[country[5].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[5].troops % temp) - (country[5].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(449, 169, 131, 67, ORANGE);
                DrawText("QU", 455, 170, 25, BLACK);
                DrawText(&currentcountrycolor, 515, 170, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 480, 210, 25, BLACK);
                //8 EU
                currentcountrycolor = player[country[7].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[7].troops % temp) - (country[7].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(382, 241, 124, 111, YELLOW);
                DrawText("EU", 390, 250, 25, BLACK);
                DrawText(&currentcountrycolor, 460, 250, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 420, 330, 25, BLACK);
                //9 CU
                currentcountrycolor = player[country[8].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[8].troops % temp) - (country[8].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(284, 337, 92, 118, GOLD);
                DrawText("CU", 290, 340, 25, BLACK);
                DrawText(&currentcountrycolor, 340, 340, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 320, 430, 25, BLACK);
                //south america
                //10 VN
                currentcountrycolor = player[country[9].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[9].troops % temp) - (country[9].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(340, 460, 212, 46, LIME);
                DrawText("VN", 350, 470, 25, BLACK);
                DrawText(&currentcountrycolor, 400, 470, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 500, 470, 25, BLACK);
                //12 PR
                currentcountrycolor = player[country[11].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[11].troops % temp) - (country[11].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(339, 510, 110, 110, DARKGREEN);
                DrawText("PR", 345, 520, 25, BLACK);
                DrawText(&currentcountrycolor, 400, 520, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 380, 590, 25, BLACK);
                //11 BR
                currentcountrycolor = player[country[10].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[10].troops % temp) - (country[10].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(454, 510, 248, 145, GREEN);
                DrawText("BR", 465, 520, 25, BLACK);
                DrawText(&currentcountrycolor, 525, 520, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 490, 610, 25, BLACK);
                //13 AG
                currentcountrycolor = player[country[12].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[12].troops % temp) - (country[12].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(352, 624, 98, 201, LIME);
                DrawText("AG", 360, 635, 25, BLACK);
                DrawText(&currentcountrycolor, 410, 635, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 380, 730, 25, BLACK);
                //europe
                //14 IC
                currentcountrycolor = player[country[13].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[13].troops % temp) - (country[13].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(747, 145, 97, 63, BLUE);
                DrawText("IC", 760, 155, 25, BLACK);
                DrawText(&currentcountrycolor, 810, 155, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 800, 180, 25, BLACK);
                //15 GB
                currentcountrycolor = player[country[14].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[14].troops % temp) - (country[14].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(770, 210, 75, 97, DARKBLUE);
                DrawText("GB", 778, 220, 25, BLACK);
                DrawText(&currentcountrycolor, 828, 220, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 790, 260, 25, BLACK);
                //18 WE
                currentcountrycolor = player[country[17].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[17].troops % temp) - (country[17].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(730, 309, 115, 133, SKYBLUE);
                DrawText("WE", 740, 320, 25, BLACK);
                DrawText(&currentcountrycolor, 790, 320, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 770, 400, 25, BLACK);
                //16 SC
                currentcountrycolor = player[country[15].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[15].troops % temp) - (country[15].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(848, 128, 168, 110, SKYBLUE);
                DrawText("SC", 860, 140, 25, BLACK);
                DrawText(&currentcountrycolor, 910, 140, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 880, 190, 25, BLACK);
                //17 NE
                currentcountrycolor = player[country[16].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[16].troops % temp) - (country[16].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(847, 242, 169, 91, BLUE);
                DrawText("NE", 860, 250, 25, BLACK);
                DrawText(&currentcountrycolor, 910, 250, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 890, 300, 25, BLACK);
                //19 SE
                currentcountrycolor = player[country[18].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[18].troops % temp) - (country[18].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(847, 335, 169, 107, DARKBLUE);
                DrawText("SE", 860, 350, 25, BLACK);
                DrawText(&currentcountrycolor, 910, 350, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 890, 410, 25, BLACK);
                //20 UR
                currentcountrycolor = player[country[19].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[19].troops % temp) - (country[19].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1019, 123, 187, 243, { 30 , 130 ,210 , 255 });
                DrawText("UR", 1030, 140, 25, BLACK);
                DrawText(&currentcountrycolor, 1080, 140, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1060, 240, 25, BLACK);
                //africa
                //21 NA
                currentcountrycolor = player[country[20].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[20].troops % temp) - (country[20].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(706, 445, 248, 175, BROWN);
                DrawText("NA", 720, 460, 25, BLACK);
                DrawText(&currentcountrycolor, 770, 460, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 750, 500, 25, BLACK);
                //24 CG
                currentcountrycolor = player[country[23].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[23].troops % temp) - (country[23].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(838, 625, 118, 121, DARKBROWN);
                DrawText("CG", 850, 640, 25, RAYWHITE);
                DrawText(&currentcountrycolor, 900, 640, 25, RAYWHITE);
                DrawText(&currentcountrytroopcount[0], 880, 700, 25, RAYWHITE);
                //25 SA
                currentcountrycolor = player[country[24].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[24].troops % temp) - (country[24].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(960, 679, 158, 156, BROWN);
                DrawText("SA", 970, 690, 25, BLACK);
                DrawText(&currentcountrycolor, 1020, 690, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1000, 770, 25, BLACK);
                //26 MD
                currentcountrycolor = player[country[25].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[25].troops % temp) - (country[25].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1120, 617, 85, 153, DARKBROWN);
                DrawText("MD", 1130, 630, 25, RAYWHITE);
                DrawText(&currentcountrycolor, 1180, 630, 25, RAYWHITE);
                DrawText(&currentcountrytroopcount[0], 1150, 730, 25, RAYWHITE);
                //22 EG
                currentcountrycolor = player[country[21].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[21].troops % temp) - (country[21].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(956, 445, 158, 82, DARKBROWN);
                DrawText("EG", 970, 460, 25, RAYWHITE);
                DrawText(&currentcountrycolor, 1020, 460, 25, RAYWHITE);
                DrawText(&currentcountrytroopcount[0], 1000, 500, 25, RAYWHITE);
                //23 EA
                currentcountrycolor = player[country[22].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[22].troops % temp) - (country[22].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(960, 532, 157, 144, BEIGE);
                DrawText("EA", 970, 540, 25, BLACK);
                DrawText(&currentcountrycolor, 1020, 540, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1000, 640, 25, BLACK);
                //also 23
                DrawRectangle(1117, 445, 80, 166, BEIGE);
                //asia
                //27 ME
                currentcountrycolor = player[country[26].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[26].troops % temp) - (country[26].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1019, 369, 268, 72, GREEN);
                DrawText("ME", 1030, 380, 25, BLACK);
                DrawText(&currentcountrycolor, 1080, 380, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1180, 380, 25, BLACK);
                //28 AF
                currentcountrycolor = player[country[27].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[27].troops % temp) - (country[27].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1208, 252, 129, 114, DARKGREEN);
                DrawText("AF", 1220, 260, 25, BLACK);
                DrawText(&currentcountrycolor, 1270, 260, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1250, 340, 25, BLACK);
                //29 UL
                currentcountrycolor = player[country[28].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[28].troops % temp) - (country[28].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1208, 51, 146, 199, LIME);
                DrawText("UL", 1220, 60, 25, BLACK);
                DrawText(&currentcountrycolor, 1270, 60, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1250, 160, 25, BLACK);
                //30 IN
                currentcountrycolor = player[country[29].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[29].troops % temp) - (country[29].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1290, 369, 154, 199, LIME);
                DrawText("IN", 1300, 380, 25, BLACK);
                DrawText(&currentcountrycolor, 1350, 380, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1330, 480, 25, BLACK);
                //31 CH
                currentcountrycolor = player[country[30].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[30].troops % temp) - (country[30].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1341, 253, 271, 113, GREEN);
                DrawText("CH", 1360, 260, 25, BLACK);
                DrawText(&currentcountrycolor, 1420, 260, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1380, 340, 25, BLACK);
                //33 SB
                currentcountrycolor = player[country[32].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[32].troops % temp) - (country[32].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1358, 51, 143, 199, DARKGREEN);
                DrawText("SB", 1370, 60, 25, BLACK);
                DrawText(&currentcountrycolor, 1420, 60, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1400, 160, 25, BLACK);
                //36 YK
                currentcountrycolor = player[country[35].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[35].troops % temp) - (country[35].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1504, 51, 149, 86, LIME);
                DrawText("YK", 1520, 60, 25, BLACK);
                DrawText(&currentcountrycolor, 1570, 60, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1550, 100, 25, BLACK);
                //35 IK
                currentcountrycolor = player[country[34].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[34].troops % temp) - (country[34].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1504, 141, 149, 47, GREEN);
                DrawText("IK", 1510, 160, 25, BLACK);
                DrawText(&currentcountrycolor, 1550, 160, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1600, 160, 25, BLACK);
                //34 MN
                currentcountrycolor = player[country[33].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[33].troops % temp) - (country[33].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1504, 191, 149, 58, LIME);
                DrawText("MN", 1510, 200, 25, BLACK);
                DrawText(&currentcountrycolor, 1565, 200, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1610, 200, 25, BLACK);
                //37 KC
                currentcountrycolor = player[country[36].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[36].troops % temp) - (country[36].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1656, 51, 161, 150, DARKGREEN);
                DrawText("KC", 1670, 60, 25, BLACK);
                DrawText(&currentcountrycolor, 1720, 60, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1700, 110, 25, BLACK);
                //38 JP
                currentcountrycolor = player[country[37].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[37].troops % temp) - (country[37].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1656, 204, 161, 70, GREEN);
                DrawText("JP", 1670, 210, 25, BLACK);
                DrawText(&currentcountrycolor, 1720, 210, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1700, 250, 25, BLACK);
                //32 SM
                currentcountrycolor = player[country[31].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[31].troops % temp) - (country[31].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1447, 370, 171, 125, DARKGREEN);
                DrawText("SM", 1460, 380, 25, BLACK);
                DrawText(&currentcountrycolor, 1510, 380, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1490, 450, 25, BLACK);
                //australia
                //39 ID
                currentcountrycolor = player[country[38].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[38].troops % temp) - (country[38].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1490, 498, 115, 131, VIOLET);
                DrawText("ID", 1500, 510, 25, BLACK);
                DrawText(&currentcountrycolor, 1550, 510, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1530, 600, 25, BLACK);
                //40 NG
                currentcountrycolor = player[country[39].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[39].troops % temp) - (country[39].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1608, 575, 186, 54, PURPLE);
                DrawText("NG", 1620, 580, 25, BLACK);
                DrawText(&currentcountrycolor, 1670, 580, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1745, 580, 25, BLACK);
                //41 WA
                currentcountrycolor = player[country[40].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[40].troops % temp) - (country[40].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1528, 632, 160, 133, DARKPURPLE);
                DrawText("WA", 1540, 640, 25, BLACK);
                DrawText(&currentcountrycolor, 1590, 640, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1570, 700, 25, BLACK);
                //42 AU
                currentcountrycolor = player[country[41].controller].color;
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (country[41].troops % temp) - (country[41].troops % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawRectangle(1691, 632, 115, 130, VIOLET);
                DrawText("AU", 1700, 640, 25, BLACK);
                DrawText(&currentcountrycolor, 1750, 640, 25, BLACK);
                DrawText(&currentcountrytroopcount[0], 1730, 700, 25, BLACK);
                //line
                DrawRectangle(1817, 235, 99, 2, RED);
                DrawRectangle(0, 130, 20, 2, RED);
                //HUD
                DrawRectangle(0, 850, 1920, 100, GRAY);
                switch (player[currentplayer].color) {
                case 'R': {
                    DrawText("Red", 20, 875, 60, RED);
                }break;
                case 'G': {
                    DrawText("Green", 20, 875, 60, GREEN);
                }break;
                case 'B': {
                    DrawText("Blue", 20, 875, 60, DARKBLUE);
                }break;
                }
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (player[currentplayer].troop % temp) - (player[currentplayer].troop % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawText("troops : ", 300, 860, 35, BLACK);
                DrawText(&currentcountrytroopcount[0], 460, 860, 35, BLACK);
                for (int i = 2; i >= 0; i--) {
                    temp = (pow(10, i + 1));
                    temp2 = (pow(10, i));
                    currentcountrytroopcount[2 - i] = (player[currentplayer].star % temp) - (player[currentplayer].star % temp2);
                    currentcountrytroopcount[2 - i] /= pow(10, i);
                    currentcountrytroopcount[2 - i] += '0';
                }
                DrawText("stars : ", 300, 900, 35, BLACK);
                DrawText(&currentcountrytroopcount[0], 440, 900, 35, BLACK);
                switch (currentgamephase) {
                case deploytroops: {
                    DrawText("deploy phase", 650, 870, 50, BLACK);
                }break;
                case attackdefend: {
                    DrawText("attack phase", 650, 870, 50, BLACK);
                }break;
                case movetroops: {
                    DrawText("move phase", 650, 870, 50, BLACK);
                }break;
                }
                if (currentgamephasephase == 0)
                    DrawText("select a country", 1020, 880, 35, BLACK);
                else if ((currentgamephase == attackdefend || currentgamephase == movetroops) && currentgamephasephase == 1)
                    DrawText("select another country", 1020, 880, 35, BLACK);
                else
                    DrawText("select a number", 1020, 880, 35, BLACK);
                DrawText(&timerchar[0], 1770, 880, 40, BLACK);
                if(currentgamephasephase == 0)
                    DrawText("[ENTER] next phase", 1375, 860, 35, BLACK);
                if (currentgamephase == attackdefend && currentgamephasephase == 0)
                    if (fastcombat == 1)
                        DrawText("[F]FC:On", 1500, 910, 35, BLACK);
                    else
                        DrawText("[F]FC:Off", 1500, 910, 35, BLACK);
                if (currentgamephase == deploytroops && currentgamephasephase == 0)
                        DrawText("[T]Turn in stars", 1375, 910, 35, BLACK);
            }
            else {
                DrawRectangle(0, 190, 1920, 570, LIGHTGRAY);
                if (winner != 'D') {
                    DrawText("Game Over !", 640, 300, 115, BLACK);
                    if (winner == 'R')
                        DrawText("Winner is : Red !", 530, 530, 115, BLACK);
                    else if (winner == 'B')
                        DrawText("Winner is : Blue !", 520, 530, 115, BLACK);
                    else if (winner == 'G')
                        DrawText("Winner is : Green !", 460, 530, 115, BLACK);
                }
                else {
                    DrawText("Game is a Draw !", 450, 410, 130, BLACK);
                }
            }
        }break;

        case menu:
        {
            DrawText("Global war !", 190, 75, 150, LIGHTGRAY);
            DrawText("[P] Play", 190, 250, 50, LIGHTGRAY);
            DrawText("[T] Tutorial", 190, 350, 50, LIGHTGRAY);
            if (fastcombat == 1)
                DrawText("[F] Fast Combat : On", 190, 450, 50, LIGHTGRAY);
            else
                DrawText("[F] Fast Combat : Off", 190, 450, 50, LIGHTGRAY);
            if (musicison == 1)
                DrawText("[M] Music : On", 190, 550, 50, LIGHTGRAY);
            else
                DrawText("[M] Music : Off", 190, 550, 50, LIGHTGRAY);
            DrawText("[S] Scoreboard", 190, 650, 50, LIGHTGRAY);
            DrawText("[ESC] Exit", 190, 750, 50, LIGHTGRAY);
            DrawText("( if you havent played before\n please visit the tutorial first )", 1300, 250, 30, LIGHTGRAY);
        }break;
        case tutorial: {
            DrawText("each country shows 3 things : its name , current controller and number of troops on it", 20, 20, 30, LIGHTGRAY);
            DrawText("in order to select a country press and hold down the key for its first letter , then press the key for its second letter", 20, 60, 30, LIGHTGRAY);
            DrawText("while no countries are sellected press ENTER to go to the next phase", 20, 100, 30, LIGHTGRAY);
            DrawText("when prompted to select a number press a number between 0 and 9", 20, 140, 30, LIGHTGRAY);
            DrawText("please note the in order to perform an action with more than 9 untis you have to repeat it multiple times", 20, 180, 30, LIGHTGRAY);
            DrawText("when deploying troops select the target country and the number of troops", 20, 220, 30, LIGHTGRAY);
            DrawText("when attacking select the country you'd like to attack from and the country you'd like to attack", 20, 260, 30, LIGHTGRAY);
            DrawText("please note that the maximum number of attackers and defender are automaticlly assigned", 20, 300, 30, LIGHTGRAY);
            DrawText("after capturing a country all the remaning offenders are moved there automaticlly", 20, 340, 30, LIGHTGRAY);
            DrawText("there's also a move phase with the origin and destination being the country you attacked from and the one you claimed", 20, 380, 30, LIGHTGRAY);
            DrawText("press enter during this move phase to go back to the attack phase", 20, 420, 30, LIGHTGRAY);
            DrawText("when moving troops select where you'd like to move them from , where to and how many you'd like to move", 20, 460, 30, LIGHTGRAY);
            DrawText("during the deploy toops phase when no countries are selected press [T] to turn in your stars for troops", 20, 500, 30, LIGHTGRAY);
            DrawText("during the attck phase while no countires are selected you can toggle fast combat by pressing [F]", 20, 540, 30, LIGHTGRAY);
            DrawText("[B]/[ENTER] Main Menu", 1500, 860, 30, LIGHTGRAY);
        }break;
        case scoreboard: {
            for (int i = 0; i < 10; i++) {
                if (scorelinetxt[i] == "Draw")
                    DrawText("Draw", 190, 100 + 75 * i, 60, LIGHTGRAY);
                if (scorelinetxt[i] == "Green")
                    DrawText("Green", 190, 100 + 75 * i, 60, LIGHTGRAY);
                if (scorelinetxt[i] == "Blue")
                    DrawText("Blue", 190, 100 + 75 * i, 60, LIGHTGRAY);
                if (scorelinetxt[i] == "Red")
                    DrawText("Red", 190, 100 + 75 * i, 60, LIGHTGRAY);
            }
            DrawText("[B]/[ENTER] Main Menu", 1500, 860, 30, LIGHTGRAY);
        }break;
        }
        EndDrawing();
    }
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}

bool relationfinder(int number1, int number2) {
    bool result = 0;
    if (relationmap[number1][number2] == 1)
        result = 1;
    else {
        for (int i = 0; i < 42; i++) {
            if (relationmap[number1][i] == 1 && country[i].controller == currentplayer && alreadytreversed[i] == 0) {
                alreadytreversed[i] = 1;
                result = relationfinder(i, number2);
            }
            if (result) break;
        }
    }
    return result;
}
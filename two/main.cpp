#include <emscripten.h>
#include <emscripten/bind.h>
#include <stdio.h>
#include <math.h>
#include <chrono>
#include <thread>
#include <stdlib.h>
#include <sstream>
#include "two.cpp"
using namespace emscripten;

int x;
int score = 0;
int shipX = 95;
bool start = false;
bool dead = false;
int missleX = 0;
int missleY = 1000;
bool missleActive = false;
int explosionDuration = 0;
int explosionX = 0;
int explosionY = 0;

    Asteroid Asteroids[7] { {rand() % 195 + 5, rand() % 100 - 200, rand() % 20 + 2},
    { rand() % 195 + 5, rand() % 100 - 200, rand() % 20 + 5},
    {rand() % 195 + 5, rand() % 100 - 200, rand() % 20 + 5},
    {rand() % 195 + 5, rand() % 100 - 200, rand() % 20 + 5},
    {rand() % 195 + 5, rand() % 100 - 200, rand() % 20 + 5},
    {rand() % 195 + 5, rand() % 100 - 200, rand() % 20 + 5},
    {rand() % 195 + 5, rand() % 100 - 200, rand() % 20 + 5} };


void callJS(std::string f){

      //const std::string tmp = i.str();
      const char* s = f.c_str();  
      emscripten_run_script(s);

}

void resetAsteroids(){
    for(int i = 0; i < 8; i++){
        Asteroids[i].X = rand() % 195 + 5;
        Asteroids[i].Y = rand() % 100 - 200;
        Asteroids[i].Radius = rand() % 20 + 5;
    }
}

void doGame()
{
    if (!dead && start)
    {
      x = x + 1;  
      std::stringstream f;

      f << "setScore(" << score << ")";
      callJS(f.str());
     
     f.str("");
     f << "drawShip(" << shipX << ", 65)";    
     callJS(f.str()); 

    if (missleActive)
	{
        f.str("");
        f << "drawMissle(" << missleX << ", " << missleY << ")";    
        callJS(f.str());
        missleY = missleY - 3; 				
	}

    if(missleY < -10)
    {
        missleActive = false;
    }

	if(explosionDuration > 0)
	{
        f.str("");
        f << "drawExplosion(" << explosionX << ", " << explosionY << ")";    
        callJS(f.str());        
		explosionDuration -= 1;
	} 
     
     for(int i = 0; i < 8; i++){
        f.str("");
         f << "drawAsteroid(" << Asteroids[i].X << ", " << Asteroids[i].Y << ", " << Asteroids[i].Radius << ")";         
         callJS(f.str()); 
         if( Asteroids[i].Y < 120 && !dead){
         Asteroids[i].Y += 2;
         }else{
                Asteroids[i].X = rand() % 195 + 5;
                Asteroids[i].Y = rand() % 100 - 200;
                Asteroids[i].Radius = rand() % 20 + 5;
                if (!dead)
                {
                    score++;
                }
         }

        //Check Collision
        if ((Asteroids[i].Y + Asteroids[i].Radius) > 67 && (Asteroids[i].Y - Asteroids[i].Radius) < 83 && (Asteroids[i].X - Asteroids[i].Radius) < (shipX + 10) && (Asteroids[i].X + Asteroids[i].Radius) > (shipX - 10))
        {
            missleActive = false;
            dead = true;
            resetAsteroids();          
        }
        //Check Missle Collision
        if (missleActive && (Asteroids[i].X - Asteroids[i].Radius) < (missleX + 6) && (Asteroids[i].X + Asteroids[i].Radius) > (missleX) && (Asteroids[i].Y - Asteroids[i].Radius) < missleY && (Asteroids[i].Y + Asteroids[i].Radius) > missleY)
        {
            missleActive = false;
            explosionX = Asteroids[i].X;
            explosionY = Asteroids[i].Y;
            explosionDuration = 15;
            Asteroids[i].X = rand() % 100;
            Asteroids[i].Y = rand() % 100 - 200;
            Asteroids[i].Radius = rand() % 20 + 2;                
        }
     }

    }
        else
    {
        if (start)
        {
            callJS("drawDead()"); 
            shipX = 95;
        }
        else
        {
            callJS("drawStart()"); 
        }
    }

}

void handleKey(std::string k){
    if(k == "ArrowLeft"){
        if (shipX > 10) shipX -= 3;
    }
    if(k == "ArrowRight"){
        if (shipX < 190) shipX += 3;
    }
    if(k == "Enter"){
        start = true;
        dead = false;
        score = 0;
    }
    if(k == "ArrowUp"){
        missleActive = true;
        missleX = shipX - 3;
        missleY = 55;
        score -= 10;
    }
}

int main() {
    emscripten_run_script("doSomething()");
    x = 0;  
    emscripten_set_main_loop(doGame,15,1);
}

EMSCRIPTEN_BINDINGS(my_module)
{
  function("handleKey", &handleKey);
}
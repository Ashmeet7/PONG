#include <iostream>
#include "raylib.h"

Color Green = Color{38, 185, 154, 255};
Color Dark_Green = Color{20, 160, 133, 255};
Color Light_Green = Color{129, 204, 184, 255};
Color Yellow = Color{243, 213, 91, 255};

const int WIDTH = 800;
const int HEIGHT = 600;

int player_score = 0;
int cpu_score = 0;

class Ball{
    public: 
        float posx;
        float posy;
        int speedx = 7;
        int speedy = 7;
        int radius = 10;

        Ball() {
            posx = WIDTH/2;
            posy = HEIGHT/2;
        }

        void draw() {
            DrawCircle(posx, posy, radius, Yellow);
        }

        void resetball() {
            posx = WIDTH/2;
            posy = HEIGHT/2;
            int speedchoice[2] = {1,-1};

            speedx *= speedchoice[GetRandomValue(0,1)];
            speedy *= speedchoice[GetRandomValue(0,1)];
        }

        void update() {
            posx += speedx;
            posy += speedy;

            if(posy + radius >= GetScreenHeight() || posy+radius <= 0) {
                speedy *= -1;
            }

            if(posx + radius >= GetScreenWidth()) {
                player_score++; 
                resetball();
            }
            if(posx - radius <= 0) {
                cpu_score++;
                resetball();
            }
        }
};

class Paddle{
    public:
        float width = 25, height = 120;
        float posx = 5;
        float posy = HEIGHT/2 - height+10;
        int speed = 7;

        void draw() {
            DrawRectangleRounded(Rectangle{posx,posy,width,height},0.8,0,WHITE);
        }

        void update() {
            if(IsKeyDown(KEY_UP)) {
                if(posy - speed >= 0)
                    posy -= speed;
            }
            if(IsKeyDown(KEY_DOWN)) {
                if(posy + height + speed <= GetScreenHeight()) {
                    posy += speed;
                }
            }
        }
};

class ComputerPaddle : public Paddle {
    public:
        ComputerPaddle() {
            posx = WIDTH-width-5;
            posy = HEIGHT/2-height+10;
        }

        void update(int ball_y) {
            if(posy+height/2 < ball_y) {
                if(posy+speed <= HEIGHT-height)
                    posy += speed;
            }
            if(ball_y < posy+height/2) {
                if(posy-speed >= 0)
                    posy -= speed; 
            }
        }
};

Ball ball = Ball();
Paddle playerPaddle = Paddle();
ComputerPaddle cpu = ComputerPaddle();

int main() {

    InitWindow(WIDTH, HEIGHT, "PONG Game");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        BeginDrawing();

        ClearBackground(Dark_Green); 

        DrawRectangle(0,0,WIDTH/2,HEIGHT, Green);
        DrawCircle(WIDTH/2, HEIGHT/2, 150, Light_Green);
        DrawLine(WIDTH/2,0,WIDTH/2,HEIGHT,WHITE);

        ball.draw();
        playerPaddle.draw();
        cpu.draw();

        ball.update();
        playerPaddle.update();
        cpu.update(ball.posy);

        if(CheckCollisionCircleRec(Vector2{ball.posx, ball.posy}, ball.radius, Rectangle{playerPaddle.posx, playerPaddle.posy, playerPaddle.width, playerPaddle.height})) {
            ball.speedx *= -1;            
        }
        if(CheckCollisionCircleRec(Vector2{ball.posx, ball.posy}, ball.radius, Rectangle{cpu.posx, cpu.posy, cpu.width, cpu.height})) {
            ball.speedx *= -1;            
        }

        DrawText(TextFormat("%i", player_score), WIDTH/4-20, 20, 80, WHITE);
        DrawText(TextFormat("%i", cpu_score), WIDTH-WIDTH/4+20, 20, 80, WHITE);

        EndDrawing(); 
    }
    
    CloseWindow();
    return 0;
}
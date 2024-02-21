#include "Game.h"

#include <iostream>

Game::Game() : window(nullptr), renderer(nullptr), running(false), currentFrame(0), paddle(0, 0, 0, 0), ball(0, 0, 0, 0, 0, 0, 0), scoreboard(0), gameStarted(false) {}

Game::~Game() {}

bool Game::Init(const char* title, int xpos, int ypos, int width, int height, int flags) {
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "SDL init success\n";

        window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);

        if (window != nullptr) { // window init success
            std::cout << "window creation success\n";
            renderer = SDL_CreateRenderer(window, -1, 0);

            if (renderer != nullptr) { // renderer init success
                std::cout << "renderer creation success\n";
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

                windowWidth = width;
                windowHeight = height;


                //START BUTTON
               
                int BUTTON_WIDTH = 250;
                int BUTTON_HEIGHT = 125;

                startButtonRect.x = (getWindowWidth() - BUTTON_WIDTH) / 2;
                startButtonRect.y = (getWindowHeight() - BUTTON_HEIGHT) / 2;
                startButtonRect.w = BUTTON_WIDTH;
                startButtonRect.h = BUTTON_HEIGHT;

                // paddle size
                int paddleWidth = 150;
                int paddleHeight = 30;
            
                // WINDOW_HEIGHT - paddleheight to get to center horizontally
                int paddleX = (width - paddleWidth) / 2; 

                // WINDOW_HEIGHT - paddleheight to get the bottom of the screen
                int paddleY = height - paddleHeight; 

                int ballRadius = 5;
                int ballX = paddleX + paddleWidth / 2; // Center of the paddle horizontally
                int ballY = paddleY - ballRadius; // On top of the paddle

                paddle = Paddle(paddleX, paddleY, paddleWidth, paddleHeight);
                ball = Ball(ballX, ballY, ballRadius, 1, 1, windowWidth, windowHeight); // Example velocities 5 and 5

            }
            else {
                std::cout << "renderer init failed\n";
                return false;
            }
        }
        else {
            std::cout << "window init failed\n";
            return false;
        }
    }
    else {
        std::cout << "SDL init fail\n";
        return false; // SDL init fail
    }
    std::cout << "init success\n";
    running = true;
    return true;
}

void Game::HandleEvents() {
    SDL_Event event;
    if (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            running = false;
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {

                cout << "Resized Window" << endl;
                cout << "Old WW " << windowWidth << endl;
                cout << "Old WW " << windowHeight << endl;

                cout << " asdsad " << endl;
                // Update window dimensions
                windowWidth = event.window.data1;
                cout << "New WW" << windowWidth << endl;
                windowHeight = event.window.data2;
                cout << "New WH" << windowHeight << endl;

                // Recalculate paddle position to remain at the bottom
                int paddleWidth = paddle.getWidth();
                int paddleHeight = paddle.getHeight();
                int paddleX = (windowWidth - paddleWidth) / 2; // Center horizontally
                int paddleY = windowHeight - paddleHeight; // Bottom of the screen
                paddle.setX(paddleX);
                paddle.setY(paddleY);

                // Update ball position to remain on the paddle
                int ballRadius = ball.getRadius();
                int ballX = paddleX + paddleWidth / 2; // Center of the paddle horizontally
                int ballY = paddleY - ballRadius; // On top of the paddle
                ball.setX(ballX);
                ball.setY(ballY);
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            // Check if the left mouse button is clicked
            if (event.button.button == SDL_BUTTON_LEFT) {
                // Start the ball moving by giving it some initial velocity
                cout << "Move Ball" << endl;
                cout << ball.getVelocityX() << "  " << ball.getVelocityY() << endl;
                cout << ball.getX() << "  " << ball.getY() << endl;
                ball.setVelocityX(2); // Example initial velocity
               ball.setVelocityY(2); // Example initial velocity
            }
            break;
        }
    }
}

void Game::RenderStartScreen() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render title and instructions (you can use SDL functions for rendering text)

    // Render start button
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &startButtonRect);

    // Present renderer
    SDL_RenderPresent(renderer);

}

bool Game::IsMouseOverStartButton(int mouseX, int mouseY) {
    return mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w &&
        mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h;
}

void Game::Update() {
    currentFrame = int(((SDL_GetTicks() / 150) % 5)); // % 5 for 5 frames
    ball.Update();
}

void Game::Render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    paddle.Render(renderer);
    ball.Render(renderer);
 /*   for (auto& brick : bricks) {
        brick.Render(renderer);
    }*/
    //scoreboard.Render(renderer);

    SDL_RenderPresent(renderer);
}

bool Game::IsRunning() {
    return running;
}

void Game::Clean() {
    std::cout << "cleaning game\n";

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}


int Game::getWindowHeight() const {

    return this->windowHeight;
}

int Game::getWindowWidth() const {

    return this->windowWidth;
}

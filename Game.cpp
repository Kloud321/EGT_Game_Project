#include "Game.h"
#include "TextureManager.h"

#include <iostream>

Game::Game() : window(nullptr), renderer(nullptr), running(false), currentFrame(0), paddle(0, 0, 0, 0),

ball(0, 0, 0, 0, 0, 0, 0), scoreboard(0), gameStarted(false), fontSize(85), playerLives(2), fontManager(){}

Game::~Game() {

}

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

                InitBricks();
                // Image init
                IMG_Init(IMG_INIT_PNG);
                
                // Load texture using TextureManager
                if (!TextureManager::Instance()->loadTexture("images/b.bmp", "background", renderer)) {
                    std::cerr << "Failed to load texture!" << std::endl;
                    return false;
                }

                // Check if TTF is init
                if (TTF_Init() == -1) {
                    std::cerr << "TTF_Init() failed: " << TTF_GetError() << std::endl;
                    return false;
                }
                else {
                    cout << "TTF_Init succeded!" << endl;
                }

                // Load font
                if (!fontManager.loadFont("D:\\Private folder\\University and styduing\\EGT\\FreeORG C++ Course\\VS Projects EGT\\EGT_Game_Project\\EGT_Game_Project\\Fonts\\ARCADE_I.ttf", fontSize)) {
                    return false;
                }
                else {
                    cout << "FONT Loaded succecfully!" << endl;
                }

                // Windows dimensions
                windowWidth = width;
                windowHeight = height;

                // ----------------------------------------------//

                //START BUTTON

                int BUTTON_WIDTH = 250;
                int BUTTON_HEIGHT = 125;

                startButtonRect.x = (getWindowWidth() - BUTTON_WIDTH) / 2;
                startButtonRect.y = (getWindowHeight() - BUTTON_HEIGHT) / 2;
                startButtonRect.w = BUTTON_WIDTH;
                startButtonRect.h = BUTTON_HEIGHT;

                // ------------------------------------------------//

                // PADDLE

                int paddleWidth = 150;
                int paddleHeight = 30;
            
                // WINDOW_HEIGHT - paddleheight to get to center horizontally
                int paddleX = (width - paddleWidth) / 2; 

                // WINDOW_HEIGHT - paddleheight to get the bottom of the screen
                int paddleY = height - paddleHeight; 

                int ballRadius = 5;
                int ballX = paddleX + paddleWidth / 2; // Center of the paddle horizontally
                int ballY = paddleY - ballRadius; // On top of the paddle
                //ball.setInitialPosition(ballX, ballY);
       
                paddle = Paddle(paddleX, paddleY, paddleWidth, paddleHeight);
                ball = Ball(ballX, ballY, ballRadius, 6, 6, windowWidth, windowHeight);

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


        case SDL_MOUSEMOTION:
            // if gameStarted we get the mouse X position and update paddle X 
            if (gameStarted && ball.getBallState() ==true) {
                int mouseX = event.motion.x;
                paddle.setX(mouseX - paddle.getWidth() / 2);
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (!gameStarted && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (IsMouseOverStartButton(mouseX, mouseY)) {
                    // Start the game
                    gameStarted = true;
              
                }
            }
            else if (gameStarted && event.button.button == SDL_BUTTON_LEFT) {
                // Only toggle ball movement if it's not already moving
                cout << "BALL STATE" << ball.getBallState() << endl;
                if (!ball.getBallState()) { 
                    // Toggle ball movement
                    ball.setBallMoving(true);
                    cout << "Ball is moving" << endl;
                   
                }
                }
         
            break;

        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED) {

                // Update window dimensions
                windowWidth = event.window.data1;
                windowHeight = event.window.data2;
        
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
           
        }
    }
}

void Game::RenderStartScreen() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw an image
    TextureManager::Instance()->drawTexture("background", 0, 0, windowWidth, windowHeight, renderer);
    
 
    // DRAW FONT
    SDL_Color textColor = { 255, 255, 255 }; // White color
    int textWidth, textHeight;
    fontManager.getTextSize("START", textColor, &textWidth, &textHeight);
    int buttonWidth = textWidth + 10; // Add padding
    int buttonHeight = textHeight + 10; // Add padding
    startButtonRect.x = (getWindowWidth() - buttonWidth) / 2;
    startButtonRect.y = (getWindowHeight() - buttonHeight) / 2;
    startButtonRect.w = buttonWidth;
    startButtonRect.h = buttonHeight;
    fontManager.renderText("START", textColor, renderer, startButtonRect.x + 5, startButtonRect.y + 5);

    // Draw border around the start button
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
    SDL_RenderDrawRect(renderer, &startButtonRect);

    // Present renderer
    SDL_RenderPresent(renderer);

}

bool Game::IsMouseOverStartButton(int mouseX, int mouseY) {
    return mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w &&
        mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h;
}

void Game::Update() {

    if (!ball.Update(paddle)) {

        cout << "OUTSUIDE" << ball.getY() << endl;
        setLives(getLives() - 1);
        cout << "Lives Left" << getLives() << endl;
        if (getLives() > 0) {
            if (ball.getY() >= getWindowHeight()) {
                cout << "WE are inside" << endl;
                int paddleX = (getWindowWidth() - paddle.getWidth()) / 2;
                paddle.setX(paddleX);
            }
            ball.setX(ball.getInitialX());
            ball.setY(ball.getInitialY());
            ball.setVelocityX(6);
            ball.setVelocityY(6);

            cout << ball.getY() << endl;
            cout << getWindowHeight() << endl;
        }
        else {
            cout << "Game over!" << endl;
        }
    }
}

void Game::InitBricks() {
    const int brickWidth = 60;
    const int brickHeight = 30;
    const int spacing = 1;

    int WINDOW_HEIGHT = 600;
    cout << "Height " << windowWidth << endl;
    int WINDOW_WIDTH = 1300;
    cout << "WIDTH " << windowWidth << endl;

    int leftCubeStartX = spacing;
    int leftCubeStartY = (WINDOW_HEIGHT - (8 * (brickHeight + spacing))) / 2;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 3; ++j) {
            int x = leftCubeStartX + j * (brickWidth + spacing);
            int y = leftCubeStartY + i * (brickHeight + spacing);
            bricks.push_back(Brick(x, y, brickWidth, brickHeight, true));
        }
    }

    int rightCubeStartX = WINDOW_WIDTH - (3 * (brickWidth + spacing)) - spacing;
    int rightCubeStartY = (WINDOW_HEIGHT - (8 * (brickHeight + spacing))) / 2;
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 3; ++j) {
            int x = rightCubeStartX + j * (brickWidth + spacing);
            int y = rightCubeStartY + i * (brickHeight + spacing);
            bricks.push_back(Brick(x, y, brickWidth, brickHeight, true));
        }
    }

}


void Game::Render() {
    if (!gameStarted) {
        RenderStartScreen();
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 128, 0, 0, 255);
        SDL_RenderClear(renderer);

        paddle.Render(renderer);
        ball.Render(renderer);
        for (auto& brick : bricks) {
               brick.Render(renderer);
           }
           //scoreboard.Render(renderer);

        SDL_RenderPresent(renderer);
    }
    
}

bool Game::IsRunning() {
    return running;
}

void Game::Clean() {
    cout << "cleaning game\n";

    TextureManager::Instance()->clean();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Game::RunGameLoop() {
    Uint32 frameStart;
    int frameTime;
    const int desiredFrameRate = 50;

    while (IsRunning()) {
        frameStart = SDL_GetTicks();

        HandleEvents();
        Update();
        Render();

        frameTime = SDL_GetTicks() - frameStart;

        if (frameTime < 1000 / desiredFrameRate) {
            SDL_Delay(1000 / desiredFrameRate - frameTime);
        }
    }
}

int Game::getWindowHeight() const {

    return this->windowHeight;
}

int Game::getWindowWidth() const {

    return this->windowWidth;
}

int Game::getLives() const {

    return this->playerLives;
}

void Game::setLives(int lives) {

    this->playerLives = lives;
}

bool Game::checkGameStarted() const {

    return this->gameStarted;
}
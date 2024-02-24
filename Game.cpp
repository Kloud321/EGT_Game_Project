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

                //START BUTTON init
                InitStartButton();

                // Init paddle and ball
                InitPaddleAndBall();

                // Init bricks
                InitBricks();
                
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

        setLives(getLives() - 1);
        cout << "Lives Left" << getLives() << endl;
        if (getLives() > 0) {
            if (ball.getY() >= getWindowHeight()) {
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

void Game::InitStartButton() {
    int BUTTON_WIDTH = 250;
    int BUTTON_HEIGHT = 125;

    startButtonRect.x = (getWindowWidth() - BUTTON_WIDTH) / 2;
    startButtonRect.y = (getWindowHeight() - BUTTON_HEIGHT) / 2;
    startButtonRect.w = BUTTON_WIDTH;
    startButtonRect.h = BUTTON_HEIGHT;
}

void Game::InitPaddleAndBall() {

    // PADDLE width and height;
    int paddleWidth = 150;
    int paddleHeight = 30;

    // X and Y pos for paddle
    int paddleX = (getWindowWidth() - paddleWidth) / 2;  // (WINDOW_HEIGHT - paddleheight) devided by 2 to get to center horizontally
    int paddleY = getWindowHeight() - paddleHeight; // WINDOW_HEIGHT - paddleheight to get the bottom of the screen


    int ballRadius = 5; // Set ball radius
    int ballX = paddleX + paddleWidth / 2;  // Set ball to the center of the paddle horizontally
    int ballY = paddleY - ballRadius;  // On top of the paddle

    paddle = Paddle(paddleX, paddleY, paddleWidth, paddleHeight);
    ball = Ball(ballX, ballY, ballRadius, 6, 6, windowWidth, windowHeight);
}

void Game::InitBricks() {
    const int brickWidth = 60;
    const int brickHeight = 30;
    const int spacing = 2;

    int WINDOW_HEIGHT = getWindowHeight();
    int WINDOW_WIDTH = getWindowWidth();

    //X & Y for left cube of bricks
    int leftCubeStartX = spacing;
    int leftCubeStartY = (WINDOW_HEIGHT - (12 * (brickHeight + spacing))) / 2;

    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 3; ++col) {
            int x = leftCubeStartX + col * (brickWidth + spacing);
            int y = leftCubeStartY + row * (brickHeight + spacing);
            bricks.push_back(Brick(x, y, brickWidth, brickHeight, false));
        }
    }

    //X & Y for right cube of bricks
    int rightCubeStartX = WINDOW_WIDTH - (3 * (brickWidth + spacing)) - spacing;
    int rightCubeStartY = (WINDOW_HEIGHT - (12 * (brickHeight + spacing))) / 2;
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 3; ++col) {
            int x = rightCubeStartX + col * (brickWidth + spacing);
            int y = rightCubeStartY + row * (brickHeight + spacing);
            bricks.push_back(Brick(x, y, brickWidth, brickHeight, false));
        }
    }

    //X & Y for the cube in the middle

    int middleCubeWidth = 3 * brickWidth + spacing;
    int middleCubeHeight = 5 * brickHeight + 4 * spacing;

    int middleCubeStartX = (WINDOW_WIDTH - middleCubeWidth) / 2 + 2 * spacing;
    int middleCubeStartY = (WINDOW_HEIGHT - (12 * (brickHeight + spacing))) / 2;

    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 3; ++col) {
            int x = middleCubeStartX + col * (brickWidth + spacing);
            int y = middleCubeStartY + row * (brickHeight + spacing);
            bricks.push_back(Brick(x, y, brickWidth, brickHeight, false));
        }
    }

    // Initialize the bricks below the middle cube
    int startX = (WINDOW_WIDTH - (7 * brickWidth + spacing)) / 2;
    int startY = middleCubeStartY + middleCubeHeight + brickHeight + 2*spacing; // 1 brick space below the middle cube
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 7; ++col) {
            int x = startX + col * (brickWidth + spacing);
            int y = startY + row * (brickHeight + spacing);
            bricks.push_back(Brick(x, y, brickWidth, brickHeight, true)); // Using gray bricks
        }
    }

    // Initialize the left col |__| shape bricks
    int startLeftColX = (WINDOW_WIDTH - (7 * brickWidth + spacing)) / 2;
    int startLeftColY = (WINDOW_HEIGHT - (12 * (brickHeight + spacing))) / 2;
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 1; ++col) {
            int x = startLeftColX + col * (brickWidth + spacing);
            int y = startLeftColY + row * (brickHeight + spacing);
            bricks.push_back(Brick(x, y, brickWidth, brickHeight, true)); // Using gray bricks
        }
    }

    // Initialize the right col |__| shape bricks
    int startRightColX = WINDOW_WIDTH - 8 * brickWidth - 5 *spacing + 1;
    int startRightColY = (WINDOW_HEIGHT - (12 * (brickHeight + spacing))) / 2;
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 1; ++col) {
            int x = startRightColX + col * (brickWidth + spacing);
            int y = startRightColY + row * (brickHeight + spacing);
            bricks.push_back(Brick(x, y, brickWidth, brickHeight, true)); // Using gray bricks
        }
    }
}

void Game::Render() {
    if (!gameStarted) {
        RenderStartScreen();
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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
#include "Game.h"
#include "TextureManager.h"

#include <iostream>

Game::Game() : window(nullptr), renderer(nullptr), running(false), paddle(0, 0, 0, 0),ball(0, 0, 0, 0, 0, 0, 0),gameStarted(false),
gameOver(false), gameWon(false), paused(false), fontSize(85), playerLives(2), fontManager(), fileHandler(), score(0) {}

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
                if (IMG_Init(IMG_INIT_PNG) == -1) {
                    std::cerr << "Failed to initialize SDL_image: " << IMG_GetError() << std::endl;
                }

                // Audio init
                if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
                    std::cerr << "SDL_mixer could not initialize! SDL_mixer Error: " << Mix_GetError() << std::endl;
                    return false;
                }
                // Load Sound effect
                startSound = Mix_LoadWAV("Sounds/start.wav");
                if (!startSound) {
                    std::cerr << "Failed to load start sound effect! SDL_mixer Error: " << Mix_GetError() << std::endl;
                    return false;
                }
              
                // Load Background music
                backgroundMusic = Mix_LoadMUS("Sounds/background1.mp3");
                if (!backgroundMusic) {
                    std::cerr << "Failed to load background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
                    return false;
                }

                if (Mix_PlayMusic(backgroundMusic, -1) == -1) {
                    std::cerr << "Failed to play background music! SDL_mixer Error: " << Mix_GetError() << std::endl;
                    return false;
                }
                Mix_VolumeMusic(10);

                // Load texture using TextureManager
                if (!TextureManager::Instance()->loadTexture("images/b.bmp", "background", renderer)) {
                    std::cerr << "Failed to load texture!" << std::endl;
                    return false;
                }

                if (!TextureManager::Instance()->loadTexture("images/black-brick.bmp", "game", renderer)) {
                    std::cerr << "Failed to load background texture!" << std::endl;
                    return false;
                }

                if (!TextureManager::Instance()->loadTexture("images/heart.png", "heart", renderer)) {
                    std::cerr << "Failed to load heart texture!" << std::endl;
                    return false;
                }

                if (!TextureManager::Instance()->loadTexture("images/pause.png", "pause", renderer)) {
                    std::cerr << "Failed to load pause texture!" << std::endl;
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

                // Init paddle
                InitPaddle();

                // Init ball 
                InitBall();

                // Init bricks
                InitBricks();

                // Load scores
                LoadScoresFromFile();
                
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
            if (!paused && gameStarted && ball.getBallState() == true) {
                int mouseX = event.motion.x;
                int paddleX = mouseX - paddle.getWidth() / 2;
                if (paddleX < 0) {
                    // Cannot move more than the left side of the screen
                    paddleX = 0;
                }
                else if (paddleX + paddle.getWidth() > getWindowWidth()) {
                    // Cannot move more than the right side of the screen
                    paddleX = getWindowWidth() - paddle.getWidth();
                }
                paddle.setX(paddleX);
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            if (!gameStarted && event.button.button == SDL_BUTTON_LEFT) {
                int mouseX, mouseY;
                SDL_GetMouseState(&mouseX, &mouseY);
                if (IsMouseOverStartButton(mouseX, mouseY)) {
                    // Start the game
                    gameStarted = true;
                    Mix_PlayChannel(-1, startSound, 0);
                    Mix_VolumeMusic(10);

                }
            }
            else if (gameStarted && event.button.button == SDL_BUTTON_LEFT) {
                // Only toggle ball movement if it's not already moving
                if (!paused) {
                    cout << "BALL STATE: " << ball.getBallState() << endl;
                    if (!ball.getBallState()) {
                        // Toggle ball movement
                        ball.setBallMoving(true);
                    }
                }
            }
            break;

        case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_SPACE) {
                // Toggle on and off pause state
                TogglePause();
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

void Game::RenderGameOverScreen() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw an image
    TextureManager::Instance()->drawTexture("background", 0, 0, windowWidth, windowHeight, renderer);

    // DRAW FONT
    SDL_Color textColor = { 255, 255, 255 }; // White color
    int textWidth, textHeight;
    fontManager.getTextSize("GAME OVER", textColor, &textWidth, &textHeight);
    int buttonWidth = textWidth + 10; // Add padding
    int buttonHeight = textHeight + 10; // Add padding
    gameOverRect.x = (getWindowWidth() - buttonWidth) / 2;
    gameOverRect.y = (getWindowHeight() - buttonHeight) / 2;
    gameOverRect.w = buttonWidth;
    gameOverRect.h = buttonHeight;
    fontManager.renderText("GAME OVER", textColor, renderer, gameOverRect.x + 5, gameOverRect.y + 5);

    // Draw border around the start button
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
    SDL_RenderDrawRect(renderer, &gameOverRect);

    // Present renderer
    SDL_RenderPresent(renderer);

}

void Game::RenderGameWonScreen() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw an image
    TextureManager::Instance()->drawTexture("background", 0, 0, windowWidth, windowHeight, renderer);

    // DRAW FONT
    SDL_Color textColor = { 255, 255, 255 }; // White color
    int textWidth, textHeight;
    fontManager.getTextSize("YOU WIN", textColor, &textWidth, &textHeight);
    int buttonWidth = textWidth + 10; // Add padding
    int buttonHeight = textHeight + 10; // Add padding
    gameWonRect.x = (getWindowWidth() - buttonWidth) / 2;
    gameWonRect.y = (getWindowHeight() - buttonHeight) / 2;
    gameWonRect.w = buttonWidth;
    gameWonRect.h = buttonHeight;
    fontManager.renderText("YOU WIN", textColor, renderer, gameWonRect.x + 5, gameWonRect.y + 5);

    // Draw border around the start button
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White color
    SDL_RenderDrawRect(renderer, &gameWonRect);

    // Present renderer
    SDL_RenderPresent(renderer);

}

void Game::RenderTopScreenElements() {
    
    // Drawing heart with transparency
    TextureManager::Instance()->drawTexture("heart", 1050, 10, 69, 69, renderer);

    // Drawing text for remaining lives
    std::string livesText = "x" + std::to_string(getLives());
    SDL_Color textColor = { 255, 255, 255 }; // White color
    SDL_Rect xNummberOfLivesRect = { getWindowWidth() - 160, 10, 0, 0 };
    fontManager.renderText(livesText.c_str(), textColor, renderer, xNummberOfLivesRect.x, xNummberOfLivesRect.y);

    // Score
    SDL_Rect scoreRect = { (getWindowWidth() - 100) / 2, 10, 30, 15 };
    std::string scoreText = std::to_string(getScore());
    fontManager.renderText(scoreText.c_str(), textColor, renderer, scoreRect.x, scoreRect.y);

    // Draw pause button with transparency
    // Set color
    SDL_Color pauseButtonColor = { 128, 128, 128, 255 };
    SDL_Color outlineColor = { 255, 255, 255, 255 };
    // Outline rect
    SDL_Rect pauseButtonOutlineRect = { 20 - 2, 10 - 2, 60 + 4, 60 + 4 };
    SDL_SetRenderDrawColor(renderer, outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
    SDL_RenderFillRect(renderer, &pauseButtonOutlineRect);

    // Button rect
    SDL_Rect pauseButtonRect = { 20, 10, 60, 60 };
    SDL_SetRenderDrawColor(renderer, pauseButtonColor.r, pauseButtonColor.g, pauseButtonColor.b, pauseButtonColor.a);
    SDL_RenderFillRect(renderer, &pauseButtonRect);
    TextureManager::Instance()->drawTexture("pause", pauseButtonRect.x, pauseButtonRect.y, pauseButtonRect.w, pauseButtonRect.h, renderer);
}

bool Game::IsMouseOverStartButton(int mouseX, int mouseY) {
    return mouseX >= startButtonRect.x && mouseX <= startButtonRect.x + startButtonRect.w &&
        mouseY >= startButtonRect.y && mouseY <= startButtonRect.y + startButtonRect.h;
}

void Game::Update() {

    if (!ball.Update(paddle, bricks, score)) {
        //
        setLives(getLives() - 1);
        if (getLives() > 0) {

            ResetPaddleAndBall();
        }
        else {
            cout << "Game over!" << endl;
            gameOver = true;
            ball.setBallMoving(false);
            fileHandler.SaveScore(score);
            Mix_PlayChannel(-1, startSound, 0);
        }
    }
    else if (isGameWon()) {
        gameWon = true;
        ball.setBallMoving(false);
        fileHandler.SaveScore(score);
        Mix_PlayChannel(-1, startSound, 0);
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

void Game::ResetPaddleAndBall() {
    InitBall();
    InitPaddle();
};

void Game::InitBall() {

    int ballRadius = 5; // Set ball radius
    int ballX = getWindowWidth() / 2;  // Set ball to the center of the paddle horizontally
    int ballY = getWindowHeight() - paddle.getHeight() - ballRadius;  // On top of the paddle

    ball = Ball(ballX, ballY, ballRadius, 6, 6, getWindowWidth(), getWindowHeight());
}

void Game::InitPaddle() {
    // PADDLE width and height;
    int const paddleWidth = 150;
    int const paddleHeight = 30;

    // X and Y pos for paddle
    int paddleX = (getWindowWidth() - paddleWidth) / 2;  // (WINDOW_HEIGHT - paddleheight) devided by 2 to get to center horizontally
    int paddleY = getWindowHeight() - paddleHeight; // WINDOW_HEIGHT - paddleheight to get the bottom of the screen

    paddle = Paddle(paddleX, paddleY, paddleWidth, paddleHeight);
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
            //Horizontaly next to each other
            int x = leftCubeStartX + col * (brickWidth + spacing);
            //Verticaly next to each other
            int y = leftCubeStartY + row * (brickHeight + spacing);
            bricks.push_back(Brick(x, y, brickWidth, brickHeight, false));
        }
    }

    //X & Y for right cube of bricks
    int rightCubeStartX = WINDOW_WIDTH - (3 * (brickWidth + spacing)) - spacing;
    int rightCubeStartY = (WINDOW_HEIGHT - (12 * (brickHeight + spacing))) / 2;
    for (int row = 0; row < 5; ++row) {
        for (int col = 0; col < 3; ++col) {
            //Horizontaly next to each other
            int x = rightCubeStartX + col * (brickWidth + spacing);
            // Verticaly next to each other
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
            //Horizontaly next to each other
            int x = middleCubeStartX + col * (brickWidth + spacing);
            // Verticaly next to each other
            int y = middleCubeStartY + row * (brickHeight + spacing);
            bricks.push_back(Brick(x, y, brickWidth, brickHeight, false));
        }
    }

    // Initialize the bricks below the middle cube
    int startX = (WINDOW_WIDTH - (7 * brickWidth + spacing)) / 2;
    int startY = middleCubeStartY + middleCubeHeight + brickHeight + 2*spacing; // 1 brick space below the middle cube
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 7; ++col) {
            //Horizontaly next to each other
            int x = startX + col * (brickWidth + spacing);
            // Verticaly next to each other
            int y = startY + row * (brickHeight + spacing);
            bricks.push_back(Brick(x, y, brickWidth, brickHeight, true)); // Using gray bricks
        }
    }

    // Initialize the left col |__| shape bricks
    int startLeftColX = (WINDOW_WIDTH - (7 * brickWidth + spacing)) / 2;
    int startLeftColY = (WINDOW_HEIGHT - (12 * (brickHeight + spacing))) / 2;
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 1; ++col) {
            //Horizontaly next to each other
            int x = startLeftColX + col * (brickWidth + spacing);
            // Verticaly next to each other
            int y = startLeftColY + row * (brickHeight + spacing);
            bricks.push_back(Brick(x, y, brickWidth, brickHeight, true)); // Using gray bricks
        }
    }

    // Initialize the right col |__| shape bricks
    int startRightColX = WINDOW_WIDTH - 8 * brickWidth - 5 *spacing + 1;
    int startRightColY = (WINDOW_HEIGHT - (12 * (brickHeight + spacing))) / 2;
    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 1; ++col) {
            //Horizontaly next to each other
            int x = startRightColX + col * (brickWidth + spacing);
            // Verticaly next to each other
            int y = startRightColY + row * (brickHeight + spacing);
            bricks.push_back(Brick(x, y, brickWidth, brickHeight, true)); // Using gray bricks
        }
    }
}

void Game::Render() {
    if (!gameStarted) {
        RenderStartScreen();
    }
    else if (gameOver) {
        RenderGameOverScreen();
    }
    else if (gameWon) {
        RenderGameWonScreen();
    }
    else
    {
        SDL_SetRenderDrawColor(renderer, 121, 44, 236, 255);
        SDL_RenderClear(renderer);

        TextureManager::Instance()->drawTexture("game", 0, 0, windowWidth, windowHeight, renderer);

        paddle.Render(renderer);
        ball.Render(renderer);
        RenderTopScreenElements();

        for (auto& brick : bricks) {
               brick.Render(renderer);
           }
        SDL_RenderPresent(renderer);
    }
}

bool Game::IsRunning() {
    return running;
}

void Game::Clean() {
    cout << "cleaning game\n";

    Mix_FreeChunk(startSound);
    Mix_FreeMusic(backgroundMusic);
    Mix_CloseAudio();

    TextureManager::Instance()->clean();

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void Game::RunGameLoop() {
    // store the start time of the current frame
    Uint32 frameStart;
    // store the time taken to complete the frame
    int frameTime;
    const int desiredFrameRate = 50;
    //continues as long as the game is running
    while (IsRunning()) {
        frameStart = SDL_GetTicks();  // get the current time in milliseconds at the start of the frame

        HandleEvents();
        if (!paused) {
            Update();
        }
        Render(); //Render the current frame

        frameTime = SDL_GetTicks() - frameStart; // calculate the time taken to complete the frame

        // delay to achieve the desired frame rate
        if (frameTime < 1000 / desiredFrameRate) { // if the frame was rendered faster than desired frame rate
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

bool Game::isGameWon(){
    for (auto& brick : bricks) {
        if (!brick.IsBroken()) {
            return false;
        }
    }
    return true;
}

int Game::getScore() const{
    
    return this->score;
}

void Game::setScore(int newScore) {

    this->score = newScore;
}

void Game::TogglePause() {
    paused = !paused;
    if (paused) {
        Mix_PauseMusic();
    }
    else {
        Mix_ResumeMusic();
    }
}

void Game::LoadScoresFromFile() {
    std::vector<int> scores = fileHandler.LoadScores();

}

std::vector<Brick>Game::getBricks() const {
    return this->bricks;
}

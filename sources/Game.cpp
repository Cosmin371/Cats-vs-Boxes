//
// Created by Cosmin on 04.04.2024.
//

#include "../headers/Game.h"

Game::Game() :
    window{sf::RenderWindow(sf::VideoMode(1500, 850), "Cats vs. Boxes", sf::Style::Titlebar | sf::Style::Close)},
    shooterCatButton{Button<ShooterCat>(Button<ShooterCat>(sf::Vector2f(200.f, 100.f), 0.f, 0.f, sf::Color(202, 209, 12), true))},
    generatorCatButton{Button<GeneratorCat>(sf::Vector2f(200.f, 100.f), 200.f, 0.f, sf::Color(12, 209, 28), true)},
    wallCatButton{Button<WallCat>(sf::Vector2f(200.f, 100.f), 400.f, 0.f, sf::Color(12, 182, 209), true)},

    grid{std::vector<std::vector<bool>>(5, std::vector<bool>(10, false))}
    {

        /// Initializare tiles
    sf::RectangleShape tile;
    tile.setSize(sf::Vector2f (150.f, 150.f));
    tile.setFillColor(sf::Color(144, 238, 144));
    tile.setOutlineThickness(3.f);
    tile.setOutlineColor(sf::Color(165, 42, 42));

    window.setFramerateLimit(60);

    /// construire background
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < 10; ++j) {
            tile.setPosition(float(j) * 150.f, 100.f + 150.f * float(i));
            background_tiles.push_back(tile);
        }
    }

    /// construire box-uri pt scor si altele
    resourcesBox.setter(sf::Vector2f(300.f, 100.f), sf::Vector2f(600.f, 0.f), sf::Color(186,85,211));
    enemyCountBox.setter(sf::Vector2f(300.f, 100.f), sf::Vector2f(900.f, 0.f), sf::Color(255,196,12));
    scoreBox.setter(sf::Vector2f(300.f, 100.f), sf::Vector2f(1200.f, 0.f), sf::Color(0,128,128));

    /// construire text
    font.loadFromFile("./fonts/yoster.ttf");
    scoreText.setFont(font);
    scoreText.setPosition(sf::Vector2f(1210.f, 10.f));
    scoreText.setString("Score: 0\nHigh Score: " + std::to_string(highScore));
}
void Game::update() {

    /// Spawn Enemies
    sf::Time deltaTime = spawnTimer.getElapsedTime();
    if (deltaTime >= sf::seconds(2)) {
        spawnEnemy();
        spawnTimer.restart();
    }

    /// Move Enemies
    for (unsigned int i = 0; i < enemies.size(); ++i) {
        enemies[i].move();
        if (enemies[i].getShape().getPosition().x <= 0.f)
            enemies.erase(enemies.begin() + i);
    }


    /// Spawn Projectiles
    for (auto &cat : cats)
        cat->run(projectiles);

    /// Move Projectiles
    for (unsigned int i = 0; i < projectiles.size(); ++i) {
        projectiles[i].move();
        if (projectiles[i].getShape().getPosition().x >= 1480.f)
            projectiles.erase(projectiles.begin() + i);
    }

    /// Update Score
    scoreText.setString("Score: " + std::to_string(score) + "\nHigh Score: " + std::to_string(highScore));

    handleButtonDrag();

}

void Game::render() {
    /// Render background tiles
    for (auto &tile : background_tiles)
        window.draw(tile);

    /// Render enemies
    for (auto &enemy : enemies)
        enemy.draw(window, sf::RenderStates::Default);

    /// Render buttons
    shooterCatButton.draw(window, sf::RenderStates::Default);
    generatorCatButton.draw(window, sf::RenderStates::Default);
    wallCatButton.draw(window, sf::RenderStates::Default);

    resourcesBox.draw(window, sf::RenderStates::Default);
    enemyCountBox.draw(window, sf::RenderStates::Default);
    scoreBox.draw(window, sf::RenderStates::Default);

    /// Render cats
    for (auto &cat : cats) {
        cat->draw(window, sf::RenderStates::Default);
        //std::cout << "pisica\n";
    }

    /// Render projectiles
    for (auto &projectile: projectiles)
        projectile.draw(window, sf::RenderStates::Default);

    /// Render text
    window.draw(scoreText);

    /// Display
    window.display();
}

bool Game::isRunning() const {
    return window.isOpen();
}

void Game::closeIfNeeded() {
    /// Fereastra se inchide cand apasam X-ul ferestrei sau Esc
    sf::Event ev{};
    while (window.pollEvent(ev)) {
        switch (ev.type) {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::KeyPressed:
                if (ev.key.code == sf::Keyboard::Escape)
                    window.close();
            default:
                break;
        }
    }
}

void Game::run() {
    while (isRunning()) {
        closeIfNeeded();

        update();
        render();
    }
}

void Game::setHighScore(const int &s) {
    Game::highScore = s;
}

void Game::spawnEnemy() {
    static std::random_device rd;
    static std::mt19937 gen(rd());

    // Randomly generate a line between 0 and 4
    std::uniform_int_distribution<> dis(0, 4);
    int randomLine = dis(gen);

    // Create and initialize the enemy with the random line
    Enemy enemy(randomLine); // NOLINT(*-use-auto)

    // Add the enemy to your collection or perform further actions
    // For example, you might have a vector of enemies
    enemies.push_back(enemy);
}

void Game::updateMousePosition() {
    mousePosWindow = sf::Mouse::getPosition(window);
    mousePosView = window.mapPixelToCoords(mousePosWindow);
//    std::cout << mousePosView.x << " " << mousePosView.y << "\n";
}

void Game::handleButtonDrag() {
    updateMousePosition();

    shooterCatButton.dragAndDrop(grid, cats, mousePosView);
    generatorCatButton.dragAndDrop(grid, cats, mousePosView);
    wallCatButton.dragAndDrop(grid, cats, mousePosView);
}
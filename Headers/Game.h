//
// Created by Cosmin on 04.04.2024.
//

#ifndef OOP_GAME_H
#define OOP_GAME_H
#include <SFML/Graphics.hpp>

class Game {
    sf::RenderWindow window;

public:
    /// Constructori / Destructori
    Game();
    ~Game() = default;

    /// Functii accesorii
    [[nodiscard]] bool isRunning() const;

    /// Functii
    void closeIfNeeded();
    void update();
    void render();
};


#endif //OOP_GAME_H

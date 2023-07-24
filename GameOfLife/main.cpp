#include <SFML/Graphics.hpp>
#include "game.h"
#include <iostream>

const int CellSize = 20;

void draw_grid(sf::RenderWindow& window) {
    auto size = window.getSize();
    auto rows = size.y / CellSize;
    auto cols = size.x / CellSize;
    sf::RectangleShape hline(sf::Vector2f(size.x, 1));
    sf::RectangleShape vline(sf::Vector2f(1, size.y));
    hline.setFillColor(sf::Color::Color(255, 255, 255, 64));
    vline.setFillColor(sf::Color::Color(255, 255, 255, 64));

    for (auto row = 0; row < rows; row++) {
        hline.setPosition(sf::Vector2f(0, row * CellSize));
        window.draw(hline);
    }
    for (auto col = 0; col < cols; col++) {
        vline.setPosition(sf::Vector2f(col * CellSize, 0));
        window.draw(vline);
    }
}

void draw_game_state(sf::RenderWindow& window, gol::Game& game) {
    sf::RectangleShape cell(sf::Vector2f(CellSize, CellSize));
    
    for (auto row = 0; row < gol::Rows; row++) {
        for (auto col = 0; col < gol::Rows; col++) {
            if (game.at(row, col) == gol::ALIVE) {
                cell.setPosition(sf::Vector2f(col * CellSize, row * CellSize));
                window.draw(cell);
            }
        }
    }
}

int main() {
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 800), "Game of Life", sf::Style::Titlebar | sf::Style::Close);

    // create the game object
    auto game = gol::Game();

    // make initial state (empty map with a glider)
    std::array<std::array<int, gol::Cols>, gol::Rows> state{};
    state[3][1] = gol::ALIVE;
    state[1][2] = gol::ALIVE;
    state[2][3] = gol::ALIVE;
    state[3][2] = gol::ALIVE;
    state[3][3] = gol::ALIVE;
    game.setState(state);

    sf::Clock clock;    // starts clock
    
    // run the program as long as the window is open
    while (window.isOpen()) {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event)) {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                switch (event.key.code) {
                case sf::Keyboard::Key::Right:
                    if (game.isPaused()) game.step();
                    break;
                case sf::Keyboard::Key::R:
                    game.setState(state);
                    break;
                case sf::Keyboard::Key::Space:
                    game.setPaused(!game.isPaused());
                    break;
                case sf::Keyboard::Key::Up:
                    game.setGameSpeed(game.getGameSpeed() + 0.5);
                    std::cout << "Game speed: " << game.getGameSpeed() << std::endl;
                    break;
                case sf::Keyboard::Key::Down:
                    game.setGameSpeed(game.getGameSpeed() - 0.5 < 0 ? 0 : game.getGameSpeed() - 0.5);
                    std::cout << "Game speed: " << game.getGameSpeed() << std::endl;
                    break;
                case sf::Keyboard::Key::Escape:
                case sf::Keyboard::Key::Q:
                    window.close();
                    break;
                }
            }
        }
        
        if (!game.isPaused()) {
            float stepTime = 1.0 / (float)game.getGameSpeed();    // get time between steps
            sf::Time elapsed = clock.getElapsedTime();
            if (elapsed.asMilliseconds() >= stepTime * 1000) {
                game.step();
                clock.restart();
            }
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        // draw grid
        draw_grid(window);

        // draw game
        draw_game_state(window, game);

        // end the current frame
        window.display();
    }

    return 0;
}
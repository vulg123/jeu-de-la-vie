#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <random>

const int GRID_SIZE = 50;
const int CELL_SIZE = 10;

class GameOfLife {
public:
    GameOfLife() : grid(GRID_SIZE, std::vector<bool>(GRID_SIZE, false)) {}

    void initRandom();
    void update();
    void draw(sf::RenderWindow& window);

private:
    std::vector<std::vector<bool>> grid;

    int countNeighbors(int x, int y);
};

void GameOfLife::initRandom() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 1);

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            grid[i][j] = dis(gen);
        }
    }
}

void GameOfLife::update() {
    std::vector<std::vector<bool>> newGrid = grid;

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int neighbors = countNeighbors(i, j);

            if (grid[i][j]) {
                if (neighbors < 2 || neighbors > 3) {
                    newGrid[i][j] = false;
                }
            } else {
                if (neighbors == 3) {
                    newGrid[i][j] = true;
                }
            }
        }
    }

    grid = newGrid;
}

void GameOfLife::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::White);

    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (grid[i][j]) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(i * CELL_SIZE, j * CELL_SIZE);
                cell.setFillColor(sf::Color::Black);
                window.draw(cell);
            }
        }
    }

    window.display();
}

int GameOfLife::countNeighbors(int x, int y) {
    int count = 0;

    for (int i = -1; i <= 1; ++i) {
        for (int j = -1; j <= 1; ++j) {
            int nx = (x + i + GRID_SIZE) % GRID_SIZE;
            int ny = (y + j + GRID_SIZE) % GRID_SIZE;

            if (grid[nx][ny]) {
                count++;
            }
        }
    }

    if (grid[x][y]) {
        count--;
    }

    return count;
}

int main() {
    sf::RenderWindow window(sf::VideoMode(GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE), "Game of Life");
    window.setFramerateLimit(10);

    GameOfLife game;
    game.initRandom();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        game.update();
        game.draw(window);
    }

    return 0;
}

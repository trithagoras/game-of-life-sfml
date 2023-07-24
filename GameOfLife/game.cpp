#include "game.h"
#include <vector>

namespace gol {
	std::array<std::array<int, 3>, 3> makeKernel() {
		return std::array<std::array<int, 3>, 3> {{
			{ 1, 1, 1 },
			{ 1, 0, 1 },
			{ 1, 1, 1 }
		}};
	}

	int convolve(std::array<std::array<int, 3>, 3> kernel, std::array<std::array<int, Cols>, Rows> map, int pivotRow, int pivotCol) {
		// convolves a kernel over a pivot point in the map, accounting for out-of-bound position
		std::array<std::array<int, 3>, 3> slice{ {
			{ pivotRow == 0 || pivotCol == 0 ? 0 : map[pivotRow - 1][pivotCol - 1], pivotRow == 0 ? 0 : map[pivotRow - 1][pivotCol], pivotRow == 0 || pivotCol == Cols - 1 ? 0 : map[pivotRow - 1][pivotCol + 1] },
			{ pivotCol == 0 ? 0 : map[pivotRow][pivotCol - 1], map[pivotRow][pivotCol], pivotCol == Cols - 1 ? 0 : map[pivotRow][pivotCol + 1] },
			{ pivotRow == Rows - 1 || pivotCol == 0 ? 0 : map[pivotRow + 1][pivotCol - 1], pivotRow == Rows - 1 ? 0 : map[pivotRow + 1][pivotCol], pivotRow == Rows - 1 || pivotCol == Cols - 1 ? 0 : map[pivotRow + 1][pivotCol + 1] }
		} };

		int neighbours = 0;
		for (auto y = 0; y < 3; y++) {
			for (auto x = 0; x < 3; x++) {
				neighbours += slice[y][x] * kernel[y][x];
			}
		}

		return neighbours;
	}

	Game::Game(float gameSpeed) {
		this->gameSpeed = gameSpeed;
		map = {};
		paused = true;
	}

	float Game::getGameSpeed() {
		return gameSpeed;
	}

	void Game::setGameSpeed(float gameSpeed) {
		this->gameSpeed = gameSpeed;
	}

	int Game::at(int row, int col) {
		return map[row][col];
	}

	void Game::setState(std::array<std::array<int, Cols>, Rows> state) {
		map = state;
	}

	void Game::step() {
		auto nextState = map;
		auto kernel = makeKernel();
		for (auto y = 0; y < Rows; y++) {
			for (auto x = 0; x < Cols; x++) {
				auto neighbours = convolve(kernel, map, y, x);
				if (nextState[y][x] == ALIVE && neighbours != 2 && neighbours != 3) {
					nextState[y][x] = DEAD;
				} else if (map[y][x] == DEAD && neighbours == 3) {
					nextState[y][x] = ALIVE;
				}
			}
		}
		map = nextState;
	}

	bool Game::isPaused() {
		return paused;
	}
	void Game::setPaused(bool value) {
		paused = value;
	}
}
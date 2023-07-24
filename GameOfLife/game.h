#pragma once
#include <array>
#include <string>

namespace gol {
	constexpr int ALIVE = 1;
	constexpr int DEAD = 0;
	const int GameSize = 30;
	const int Cols = GameSize;
	const int Rows = GameSize;

	class Game {
	public:
		Game(float gameSpeed = 1.0);
		float getGameSpeed();
		void setGameSpeed(float gameSpeed);
		int at(int row, int col);
		void step();
		void setState(std::array<std::array<int, Cols>, Rows> state);
		bool isPaused();
		void setPaused(bool value);
	private:
		bool paused;
		float gameSpeed;    // frequency: game-steps per second
		std::array<std::array<int, Cols>, Rows> map;
	};
}
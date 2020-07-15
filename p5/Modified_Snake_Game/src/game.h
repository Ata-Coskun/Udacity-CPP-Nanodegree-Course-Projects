#ifndef GAME_H
#define GAME_H

#include <random>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  int GetSizeMonster() const;

 private:
  Snake snake;

  MonsterSnake monster;
  int order{0};


  SDL_Point food1, food2, food3;

  std::vector<SDL_Point> foods;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score{0};
  int monster_score{0};

  void PlaceFood(SDL_Point & food);
  void Update();
};

#endif

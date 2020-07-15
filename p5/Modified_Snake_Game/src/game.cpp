#include "game.h"
#include <iostream>
#include "SDL.h"
#include <memory>
#include <thread>
#include <future>

Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      monster(grid_height,grid_width),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width)),
      random_h(0, static_cast<int>(grid_height)) {
  PlaceFood(food1);
  PlaceFood(food2);
  PlaceFood(food3);
  foods.push_back(food1);
  foods.push_back(food2);
  foods.push_back(food3);
}

void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    //std::thread(async_move_monster,controller,running);
    controller.HandleInput(running,monster,foods[order]);
    controller.HandleInput(running, snake);

    Update();

    renderer.Render(snake, foods, monster);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count,monster_score);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::PlaceFood(SDL_Point &food) {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.alive) return;

  snake.Update();
  monster.Update();

  int new_x = static_cast<int>(snake.head_x);
  int new_y = static_cast<int>(snake.head_y);

  int x = static_cast<int>(monster.head_x);
  int y = static_cast<int>(monster.head_y);

  // Check if there's food over here
  for(auto food : foods){
  if (food.x == new_x && food.y == new_y) {
    score++;

    if(food.x == food1.x && food.y == food1.y){PlaceFood(food1); foods[0] = food1; order++;}
    if(food.x == food2.x && food.y == food2.y){PlaceFood(food2); foods[1] =  food2; order++;}
    if(food.x == food3.x && food.y == food3.y){PlaceFood(food3); foods[2] = food3; order++;}

    if(order > 2){order = 0;}
    // Grow snake and increase speed.
    snake.GrowBody();
    snake.speed += 0.015;
  }

  if (food.x == x && food.y == y) {
    monster_score++;

    if(food.x == food1.x && food.y == food1.y){PlaceFood(food1); foods[0] = food1; order++;}
    if(food.x == food2.x && food.y == food2.y){PlaceFood(food2); foods[1] =  food2; order++;}
    if(food.x == food3.x && food.y == food3.y){PlaceFood(food3); foods[2] = food3; order++;}

    if(order > 2){order = 0;}
    // Grow snake and increase speed.
    monster.GrowBody();
    monster.speed += 0.01;
  }
 }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.size; }
int Game::GetSizeMonster() const { return monster.size; }

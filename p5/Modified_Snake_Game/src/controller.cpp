#include "controller.h"
#include <iostream>
#include "SDL.h"
#include "snake.h"

void Controller::ChangeDirection(Snake &snake, Snake::Direction input,
                                 Snake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::ChangeDirectionMonster(MonsterSnake &snake, MonsterSnake::Direction input,
                                 MonsterSnake::Direction opposite) const {
  if (snake.direction != opposite || snake.size == 1) snake.direction = input;
  return;
}

void Controller::HandleInput(bool &running, Snake &snake) const {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT) {
      running = false;
    } else if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          ChangeDirection(snake, Snake::Direction::kUp,
                          Snake::Direction::kDown);
          //ChangeDirectionMonster(monster, MonsterSnake::Direction::kUp,
            //                              MonsterSnake::Direction::kDown);

          break;

        case SDLK_DOWN:
          ChangeDirection(snake, Snake::Direction::kDown,
                          Snake::Direction::kUp);
          //ChangeDirectionMonster(monster, MonsterSnake::Direction::kDown,
                                          //MonsterSnake::Direction::kUp);
          break;

        case SDLK_LEFT:
          ChangeDirection(snake, Snake::Direction::kLeft,
                          Snake::Direction::kRight);
          //ChangeDirectionMonster(monster, MonsterSnake::Direction::kLeft,
            //                              MonsterSnake::Direction::kRight);
          break;

        case SDLK_RIGHT:
          ChangeDirection(snake, Snake::Direction::kRight,
                          Snake::Direction::kLeft);
          //ChangeDirectionMonster(monster, MonsterSnake::Direction::kRight,
            //                              MonsterSnake::Direction::kLeft);
          break;
      }
    }
  }
}

 void Controller::HandleInput(bool &running, MonsterSnake & monster, SDL_Point const & food)const{

     int x = static_cast<int>(monster.head_x);
     int y = static_cast<int>(monster.head_y);

     if(food.y < y){
     ChangeDirectionMonster(monster,  MonsterSnake::Direction::kUp,
                      MonsterSnake::Direction::kDown);
     }

     if(food.y > y){
     ChangeDirectionMonster(monster, MonsterSnake::Direction::kDown,
                                       MonsterSnake::Direction::kUp);
     }
     if(food.x < x){
     ChangeDirectionMonster(monster,  MonsterSnake::Direction::kLeft,
                                   MonsterSnake::Direction::kRight);
    }
     if(food.x > x){
     ChangeDirectionMonster(monster,  MonsterSnake::Direction::kRight,
                                                    MonsterSnake::Direction::kLeft);
      }
 }

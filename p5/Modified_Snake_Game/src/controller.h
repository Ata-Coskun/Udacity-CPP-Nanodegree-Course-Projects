#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include "monstersnake.h"
class Controller {
 public:
  void HandleInput(bool &running, Snake &snake) const;
  void HandleInput(bool &running, MonsterSnake & monster, SDL_Point const & food)const;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
  void ChangeDirectionMonster(MonsterSnake &snake, MonsterSnake::Direction input,
                       MonsterSnake::Direction opposite) const;

};

#endif

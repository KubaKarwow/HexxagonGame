#include "Move.h"

Move::Move(sf::Vector2i start, sf::Vector2i end, bool isFar) {
    this->start=start;
    this->end=end;
    this->isFar=isFar;
    if(isFar) this->amountTaken=0;
    else this->amountTaken=1;
}
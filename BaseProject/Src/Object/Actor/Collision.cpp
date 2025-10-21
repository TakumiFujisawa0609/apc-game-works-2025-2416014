#include "Collision.h"
#include "../Stage/Stage.h"

Collision* Collision::instance_ = nullptr;

Collision::Collision() 
	:stage_(nullptr)
{

}

Collision::~Collision()
{
}

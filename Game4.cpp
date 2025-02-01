#include "GameEngine.h"

int main(int argc, char* argv[])
{
	std::shared_ptr<GameEngine> game = std::make_shared<GameEngine>();
	game->run(game);

	return 0;
}
// complete
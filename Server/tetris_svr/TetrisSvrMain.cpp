#include "TetrisEngine.h"

int main() 
{
	server svr(TetrisEngine::getMe().io_s,6112);
	TetrisEngine::getMe().Run();

	return 0;
}
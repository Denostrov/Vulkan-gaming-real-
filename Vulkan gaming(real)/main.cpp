
#include "Game.h"
#include "print.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

int main()
{
	if (!debugLog || !errorLog)
	{
		std::cout << "Couldn't create log files. Possibly permissions denied? Try running as administrator\n";
		while (true)
		{

		}
		return 740;
	}

	Game game{};

	return 0;
}
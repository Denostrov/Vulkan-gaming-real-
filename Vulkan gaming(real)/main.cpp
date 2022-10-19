
#include "Game.h"
#include "print.h"
#include "logging.h"

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

	try
	{
		Game game{};
	}
	catch (std::exception const& e)
	{
		errorLog << e.what();
	}
	catch (...)
	{
		errorLog << "Caught unknown exception\n";
	}

	return 0;
}
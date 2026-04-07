#include "CmdArgumentHandler.h"

namespace Vectora {
	void CmdArgumentHandler::Init(int argc, char* argv[])
	{
		arguments.clear();
		for (int i = 0; i < argc; i++)
			arguments.push_back(argv[i]);
	}
	const std::vector<std::string>& CmdArgumentHandler::GetAllArguments()
	{
		return arguments;
	}
}

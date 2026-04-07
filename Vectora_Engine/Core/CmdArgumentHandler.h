#pragma once

#include <vector>
#include <string>

namespace Vectora
{
	class CmdArgumentHandler
	{
	public:
		static void Init(int argc, char* argv[]);
		static const std::vector<std::string>& GetAllArguments();
	private:
		inline static std::vector<std::string> arguments;
	};
}


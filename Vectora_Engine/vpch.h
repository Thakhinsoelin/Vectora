#pragma once

#include <iostream>
#include <memory>
#include <strstream>
#include <sstream>
#include <utility>
#include <algorithm>
#include <functional>
#include <vector>
#include <array>
#include <string>
#include <stdint.h>
#include <unordered_map>
#include <unordered_set>
#include <tuple>

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/fmt/fmt.h> // Ensure the base fmt header is visible

#include <type_traits>

#include "Core/Log.h"

#ifdef V_PLATFORM_WINDOWS
#include <Windows.h>

#endif
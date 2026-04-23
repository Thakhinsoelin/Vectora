#pragma once
#include "Core.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

#include <spdlog/fmt/bundled/format.h> // Ensure we have the fmt header
#include "Core/UUID.h"
// This tells fmt how to handle any GLM vector or matrix
template<typename T, glm::length_t L, glm::qualifier Q>
struct fmt::formatter<glm::vec<L, T, Q>> : fmt::ostream_formatter {};

template<typename T, glm::length_t C, glm::length_t R, glm::qualifier Q>
struct fmt::formatter<glm::mat<C, R, T, Q>> : fmt::ostream_formatter {};

template<typename T, glm::qualifier Q>
struct fmt::formatter<glm::qua<T, Q>> : fmt::ostream_formatter {};

template<>
struct fmt::formatter<Vectora::UUID> : fmt::ostream_formatter {};

namespace Vectora {
	class VECTORA_API Log
	{
	public:
		Log() {

		}
		~Log() {

		}
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; };
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; };
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};
}

template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

inline std::ostream& operator<<(std::ostream& os, const Vectora::UUID& uuid)
{
	return os << (uint64_t)uuid;
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

#define VE_CORE_ERROR(...)	::Vectora::Log::GetCoreLogger()->error(__VA_ARGS__)
#define VE_CORE_WARN(...)	::Vectora::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define VE_CORE_INFO(...)	::Vectora::Log::GetCoreLogger()->info(__VA_ARGS__)
#define VE_CORE_TRACE(...)	::Vectora::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define VE_CORE_CRITICAL(...)	::Vectora::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define VE_ERROR(...)	::Vectora::Log::GetClientLogger()->error(__VA_ARGS__)
#define VE_WARN(...)		::Vectora::Log::GetClientLogger()->warn(__VA_ARGS__)
#define VE_INFO(...)		::Vectora::Log::GetClientLogger()->info(__VA_ARGS__)
#define VE_TRACE(...)	::Vectora::Log::GetClientLogger()->trace(__VA_ARGS__)
#define VE_CRITICAL(...)	::Vectora::Log::GetClientLogger()->critical(__VA_ARGS__)
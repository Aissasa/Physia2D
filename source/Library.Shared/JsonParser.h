#pragma once

#include <memory>
#include <document.h>

namespace Physia2D
{
	class P2World;
	class P2Body;

	/** Singleton Json parser that parses bodies configurations and uses them to create bodies.
	*/
	class JsonParser final
	{
	public:

		JsonParser(const JsonParser&) = delete;
		JsonParser(JsonParser&&) = delete;
		JsonParser& operator=(const JsonParser&) = delete;
		JsonParser& operator=(JsonParser&&) = delete;

		static JsonParser& GetInstance();

		std::shared_ptr<P2Body> ParseBody(const std::string& filePath) const;
		std::shared_ptr<P2Body> ParseBody(const rapidjson::Value& doc) const;

	private:

		JsonParser() = default;
		~JsonParser() = default;
	};
}
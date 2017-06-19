#pragma once

#include <memory>
#include <document.h>

namespace Physia2D
{
	class P2World;
	class P2Body;

	class JsonParser final
	{
	public:

		static JsonParser& GetInstance();
		std::shared_ptr<P2World> ParseWorld(const std::string& filePath);
		std::shared_ptr<P2Body> ParseBody(const std::string& filePath) const;
		std::shared_ptr<P2Body> ParseBody(const rapidjson::Value& doc) const;

		void DeleteParser() const;

	private:

		JsonParser() = default;
		static JsonParser* sInstance;
	};
}
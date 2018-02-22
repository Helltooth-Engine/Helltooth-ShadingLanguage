#pragma once

#include "tokenizer/Tokenizer.hpp"

#include "parser/ShaderTypes.hpp"

namespace htsl {

	class UniformParser {
	private:
		static UniformParser* s_Instance;

	public:
		std::string name;
		bool hasName;
		std::vector<std::string> names;
		std::vector<std::string> textureNames; // Textures have to be handled differently because they can be 2 entities at once.
		std::vector<std::string> textureArrays;

#ifdef HT_DIRECTX
		std::vector<std::string> samplerNames; // This is done because the sampler is attribuited and bound to the texture (only DX)
		std::vector<unsigned int> textureNumbers;
#endif

		std::string Parse(const Token& token, Tokenizer& tokenizer, const ShaderType& type);

		inline static void Init() { if (!s_Instance) s_Instance = new UniformParser(); }
		inline static void End() { if (s_Instance) { delete s_Instance; s_Instance = nullptr; } }
		inline static UniformParser* Get() { return s_Instance; }
	};

}
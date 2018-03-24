#pragma once

#include "tokenizer/Tokenizer.hpp"

#include "parser/TypeParser.hpp"

namespace htsl {

	class MethodParser {
	private:
		static MethodParser* s_Instance;
	public:
		std::vector<std::string> methodsWithTextures;

		std::string Parse(const Token& token, Tokenizer& tokenizer);

		inline static void Init() { if (!s_Instance) s_Instance = new MethodParser(); }
		inline static void End() { if (s_Instance) { delete s_Instance; s_Instance = nullptr; } }
		inline static MethodParser* Get() { return s_Instance; }
	};

}
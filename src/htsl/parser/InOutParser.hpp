#pragma once

#include "tokenizer/Tokenizer.hpp"

#include "parser/ShaderTypes.hpp"
#include "parser/TypeParser.hpp"

namespace htsl {
	
	class InOutParser {
	private:
		static InOutParser* s_Instance;

	public:
		std::vector<std::string> attribNames;
		std::string name;

		bool hasName = false;

		bool Parse(const Token& token, Tokenizer& tokenizer, std::string& result, const ShaderType& type);

		inline static void Init() { if (!s_Instance) s_Instance = new InOutParser(); }
		inline static void End() { if (s_Instance) { delete s_Instance; s_Instance = nullptr; } }
		inline static InOutParser* Get() { return s_Instance; }

	};


}
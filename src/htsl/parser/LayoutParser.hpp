#pragma once

#include "tokenizer/Tokenizer.hpp"

#include "parser/ShaderTypes.hpp"
#include "parser/TypeParser.hpp"

namespace htsl {

	class LayoutParser {
	private:
		std::vector<std::string> s_LayoutAttribNames;

		static LayoutParser* s_Instance;

	public:
		std::string Parse(Tokenizer& tokenizer, const std::string& currentLine, const ShaderType& type);

		void AddAttribNames(const Token& token, Tokenizer& tokenizer) {
			if (token.GetType() != TokenType::IDENTIFIER)
				tokenizer.Log("%s", "[HTSL] Unexpected token type, expected identifier as layout attribute");
			s_LayoutAttribNames.push_back(token.GetData());
		}

		std::string layoutName;
		std::string startName;
		std::vector<std::string> attributes;
		bool hasName = false;

		inline std::vector<std::string> GetInputLayout() { return s_LayoutAttribNames; }

		inline static void Init() { if (!s_Instance) s_Instance = new LayoutParser(); }
		inline static void End() { if (s_Instance) { delete s_Instance; s_Instance = nullptr; } }
		inline static LayoutParser* Get() { return s_Instance; }
	};

}

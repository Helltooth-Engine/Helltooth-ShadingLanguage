#pragma once

#include "tokenizer/Tokenizer.hpp"

#include "parser/ShaderTypes.hpp"

#include "parser/TypeParser.hpp"

namespace htsl {

	class LayoutParser {
	private:
		static std::vector<std::string> s_LayoutAttribNames;

	public:
		static std::string Parse(Tokenizer& tokenizer, const std::string& currentLine, const ShaderType& type);

		static void AddAttribNames(Tokenizer& tokenizer) {
			Token token = tokenizer.GetNextToken();
			if (token.GetType() != TokenType::IDENTIFIER)
				tokenizer.Log("[HTSL] Unexpected token type, expected identifier as layout attribute");
			s_LayoutAttribNames.push_back(token.GetData());
		}

		static std::string layoutName;
		static std::string startName;
		static std::vector<std::string> attributes;

		inline static std::vector<std::string> GetInputLayout() { s_LayoutAttribNames; }
	};

}

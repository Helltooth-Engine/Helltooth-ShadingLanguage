#pragma once

#include "tokenizer/Tokenizer.hpp"
#include "parser/TypeParser.hpp"


namespace htsl {

	class MainParser {
	public:
		static std::string Parse(const Token& token, Tokenizer& tokenizer);


	};

}
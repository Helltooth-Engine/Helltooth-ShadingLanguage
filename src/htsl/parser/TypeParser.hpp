#pragma once

#include "tokenizer/Token.hpp"

namespace htsl {

	class TypeParser {
	public:
		static bool Parse(const Token& token, std::string& parseResult);
	};


}
#ifdef HT_OPENGL
#include "parser/TypeParser.hpp"

namespace htsl {

	bool TypeParser::Parse(const Token& token, std::string& parseResult) {
		if (token.GetType() == TokenType::IDENTIFIER) {
			// HTSL is using the GLSL types
			parseResult = token.GetData();
			return true;
		}
		return false;
	}

}


#endif
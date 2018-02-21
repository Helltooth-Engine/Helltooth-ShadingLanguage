#ifdef HT_OPENGL
#include "parser/TypeParser.hpp"
#include "parser/LayoutParser.hpp"

namespace htsl {

	bool TypeParser::Parse(const Token& token, std::string& parseResult) {
		if (token.GetType() == TokenType::IDENTIFIER) {
			// HTSL is using the GLSL types
			if (!LayoutParser::Get()->hasName)
				for (auto layoutName : LayoutParser::Get()->attributes)
					if (token.GetData() == layoutName) {
						parseResult = LayoutParser::Get()->layoutName + "." + token.GetData();
						return true;
					}
			parseResult = token.GetData();
			return true;
		}
		return false;
	}

}


#endif
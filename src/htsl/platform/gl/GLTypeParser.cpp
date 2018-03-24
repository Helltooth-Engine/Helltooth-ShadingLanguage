#ifdef HT_OPENGL
#include "parser/TypeParser.hpp"
#include "parser/LayoutParser.hpp"
#include "parser/InOutParser.hpp"

namespace htsl {

	bool TypeParser::Parse(const Token& token, std::string& parseResult, bool mainBlock) {
		if (token.GetType() == TokenType::IDENTIFIER) {
			// HTSL is using the GLSL types
			if (mainBlock) {
				if (!LayoutParser::Get()->hasName)
					for (auto layoutName : LayoutParser::Get()->attributes)
						if (token.GetData() == layoutName) {
							parseResult = LayoutParser::Get()->layoutName + token.GetData();
							return true;
						}

				if (!InOutParser::Get()->hasName)
					for (auto inOutName : InOutParser::Get()->attribNames)
						if (token.GetData() == inOutName) {
							parseResult = InOutParser::Get()->name + "." + token.GetData();
							return true;
						}
			}
			parseResult = token.GetData();
			return true;
		}
		return false;
	}

}

#endif
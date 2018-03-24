#ifdef HT_OPENGL
#include "parser/MainParser.hpp"

namespace htsl {

	std::string MainParser::Parse(const Token& token, Tokenizer& tokenizer, const ShaderType& type) {
		std::string result;
		result += "void main() {\n";
		Token openBrace = tokenizer.GetNextToken();
		if(!tokenizer.LogIf(openBrace, "{"))
			return "";

		Token closeBrace = tokenizer.GetNextToken();

		while (closeBrace.GetData() != "}") {
			result += "\t";
			Token semiColon = closeBrace;
			bool lastTokenIdentifier = false;
			bool lastTokenWasDot = false;
			while (semiColon.GetData() != ";") {
				std::string typeParse;
				if (semiColon.GetData() == LayoutParser::Get()->layoutName) {
					// Next token is expected to be a .
					Token dot = tokenizer.PeekNextToken();
					if (dot.GetData() == ".")
						tokenizer.GetNextToken();
					lastTokenWasDot = true;
				}
				if (semiColon.GetType() == TokenType::TOKEN) {
					if (semiColon.GetData() == "=")
						result += " " + semiColon.GetData() + " ";
					else if (semiColon.GetData() == ",")
						result += semiColon.GetData() + " ";
					else result += semiColon.GetData();
					lastTokenIdentifier = false;
				}
				else if (!TypeParser::Parse(semiColon, typeParse, true)) {
					
					result += semiColon.GetData();
					lastTokenIdentifier = false;
				}
				else {
					if (semiColon.GetData() == "sample2D" || semiColon.GetData() == "sample3D")
						typeParse = "texture";
					else if (semiColon.GetData() == "OUT_POSITION")
						typeParse = "gl_Position";
					if (lastTokenIdentifier && !lastTokenWasDot)
						result += " " + typeParse;
					else
						result += typeParse;
					if(lastTokenWasDot)
						lastTokenIdentifier = false;
					lastTokenWasDot = false;
				}

				semiColon = tokenizer.GetNextToken();
			}
			result += ";\n";
			closeBrace = tokenizer.GetNextToken();
		}
		result += "}\n";

		Token endingSemiColon = tokenizer.GetNextToken();
		if (!tokenizer.LogIf(endingSemiColon, ";"))
			return "";

		return result;
	}
	
}

#endif // HT_OPENGL
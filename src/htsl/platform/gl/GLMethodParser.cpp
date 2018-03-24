#ifdef HT_OPENGL
#include "parser/MethodParser.hpp"

namespace htsl {
	MethodParser* MethodParser::s_Instance = nullptr;

	std::string MethodParser::Parse(const Token& token, Tokenizer& tokenizer) {
		std::string result;

		// First token is the 
		std::string parsedType;
#ifdef HT_DEBUG
		if (!TypeParser::Parse(token, parsedType)) {
			tokenizer.Log("[HTSL] Could not parse type");
			return "";
		}
#else
		TypeParser::Parse(token, parsedType);
#endif // HT_DEBUG
		result += parsedType + " ";

		Token methodNameToken = tokenizer.GetNextToken();

		std::string methodName = methodNameToken.GetData();
		result += methodName + "(";

		Token openParanthesis = tokenizer.GetNextToken();
		if (!tokenizer.LogIf(openParanthesis, "("))
			return "";

		Token closeParanthesis = tokenizer.GetNextToken();
		while (closeParanthesis.GetData() != ")") {
			// First is type
			std::string currentType;
			if (closeParanthesis.GetData() == "Texture2D")
				currentType = "sampler2D";
			else if (closeParanthesis.GetData() == "Texture3D")
				currentType = "samplerCube";
			else
#ifdef HT_DEBUG
				if (!TypeParser::Parse(closeParanthesis, currentType)) {
					tokenizer.Log("[HTSL] Could not parse type");
					return "";
				}
#else
				TypeParser::Parse(closeParanthesis, currentType);
#endif // HT_DEBUG

			result += currentType;

			// Name
			Token nameToken = tokenizer.GetNextToken();
			result += " " + nameToken.GetData();

			closeParanthesis = tokenizer.GetNextToken();
			if (closeParanthesis.GetData() == ",") {
				result += ", ";
				closeParanthesis = tokenizer.GetNextToken();
			}
		}
		result += ") {\n";
		Token openBrace = tokenizer.GetNextToken();
		if (!tokenizer.LogIf(openBrace, "{"))
			return "";

		Token closeBrace = tokenizer.GetNextToken();
		while (closeBrace.GetData() != "}") {
			Token semiColon = closeBrace;
			result += "\t";
			bool lastTokenIdentifier = false;
			while (semiColon.GetData() != ";") {
				std::string typeParse;
				if (semiColon.GetType() == TokenType::TOKEN) {
					if (semiColon.GetData() == "=")
						result += " " + semiColon.GetData() + " ";
					else if (semiColon.GetData() == ",")
						result += semiColon.GetData() + " ";
					else
						result += semiColon.GetData();
					lastTokenIdentifier = false;
				}
				else if (!TypeParser::Parse(semiColon, typeParse)) {
					result += semiColon.GetData();
					lastTokenIdentifier = false;
				}
				else {
					if (typeParse == "sample2D") {
						typeParse = "texture";
					}
					if (lastTokenIdentifier)
						result += " " + typeParse;
					else
						result += typeParse;
					lastTokenIdentifier = true;
				}
				semiColon = tokenizer.GetNextToken();
			}
			result += ";\n";
			closeBrace = tokenizer.GetNextToken();
		}

		result += "}";

		return result;
	}

}

#endif // HT_OPENGL
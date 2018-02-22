#ifdef HT_OPENGL
#include "parser/InOutParser.hpp"

namespace htsl {
	
	InOutParser* InOutParser::s_Instance = nullptr;

	bool InOutParser::Parse(const Token& token, Tokenizer& tokenizer, std::string& result, const ShaderType& type) {
		// token is either in or out, tokenizer doesn't contain this token
		
		bool isIn = false;

		if (token.GetData() == "in") {
			name = "In ";
			isIn = true;
		}
		else if (token.GetData() == "out") {
			name = "Out ";
		}
		else return false;

		Token openBrace = tokenizer.GetNextToken();
#ifdef HT_DEBUG
		if (openBrace.GetData() != "{") {
			tokenizer.Log("[HTSL] Unexpected token '%s', expected '{'", openBrace.GetData().c_str());
			return false;
		}
#endif // HT_DEBUG
		result += "{\n";

		Token closeBrace = tokenizer.GetNextToken();

		while (closeBrace.GetData() != "}") {
			// close brace will be a type;

			std::string currentType;
#ifdef HT_DEBUG
			if (!TypeParser::Parse(closeBrace, currentType)) {
				tokenizer.Log("[HTSL] Could not parse type");
				return false;
			}
#else
			TypeParser::Parse(closeBrace, currentType);
#endif // HT_DEBUG
			result += "\t" + currentType + " ";

			// INDENTIFIER NOOW
			Token attrib = tokenizer.GetNextToken();
			result += attrib.GetData();

			attribNames.push_back(attrib.GetData());

			Token semiColon = tokenizer.GetNextToken();
			if(!tokenizer.LogIf(semiColon, ";"))
				return "";

			result += ";\n";

			closeBrace = tokenizer.GetNextToken();
		}

		result += "}";
		Token semiColon = tokenizer.PeekNextToken();

		if (semiColon.GetData() != ";") {
			// the keyword is as
			Token as = tokenizer.GetNextToken();
			if(!tokenizer.LogIf(as, "as"))
				return "";

			Token nameToken = tokenizer.GetNextToken();
			name = nameToken.GetData();
			hasName = true;
		}
		else {
			if (token.GetData() == "out")
				name = "Out";
			else if (token.GetData() == "in")
				name = "In";
		}

		// Try again and this time it should TOTALLY be a semicolon
		semiColon = tokenizer.GetNextToken();
		if(!tokenizer.LogIf(semiColon, ";"))
			return "";
		
		result = token.GetData() + " DATA " + result + " " + name + ";" + "\n\n";

		return true;
	}


}

#endif // HT_OPENGL
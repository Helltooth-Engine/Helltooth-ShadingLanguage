#ifdef HT_DIRECTX

#include "parser/LayoutParser.hpp"

namespace htsl {

	LayoutParser* LayoutParser::s_Instance = nullptr;

	std::string LayoutParser::Parse(Tokenizer& tokenizer, const std::string& currentLine, const ShaderType& type) {
		std::string result;
		// First token is already "layout"

		Token openbrace = tokenizer.PeekNextToken();
		if (openbrace.GetData() != "{") {
			tokenizer.Log("[HTSL] Unexpected token '%s', expected {\n", openbrace.GetData());
		}
		result += "struct ";

		std::string layoutData = " {\n";

		Token closebrace = tokenizer.PeekNextToken();
		while (closebrace.GetData() != "}") {
			layoutData += "\t";

			// First thing we parse is the actual type which has to be parsed by the TypeParser
			
			Token typeToken = tokenizer.GetNextToken();

			std::string parseResult;
#ifdef HT_DEBUG
			if (!TypeParser::Parse(typeToken, parseResult)) {
				tokenizer.Log("[HTSL] Unexpected token '%s'", typeToken.GetData());
				return "";
			}
#else 
			TypeParser::Parse(typeToken, parseResult);
#endif // HT_DEBUG
			layoutData += parseResult + " ";

			// The Next token expected may be either as or ; because it's a choice to use this

			Token nextToken = tokenizer.PeekNextToken();
			if (nextToken.GetData() != ";") {
				AddAttribNames(tokenizer);

				Token as = tokenizer.GetNextToken();
#ifdef HT_DEBUG
				if (as.GetData() != "as") {
					tokenizer.Log("[HTSL] Unexpected token '%s', expected 'as'", as.GetData());
					return "";
				}
#endif // HT_DEBUG
			}
			// This is the actual name of the attribute that's supposed to be used
			Token attributeName = tokenizer.GetNextToken();
			attributes.push_back(attributeName.GetData());

			layoutData += attributeName.GetData();

			// Expected semicolon
			Token semiColon = tokenizer.GetNextToken();
			layoutData += semiColon.GetData() + "\n";
#ifdef HT_DEBUG
			if (semiColon.GetData() != ";") {
				tokenizer.Log("[HTSL] Unexpected token '%s', expected 'as'");
				return "";
			}
#endif // HT_DEBUG
			closebrace = tokenizer.PeekNextToken();
		}

		tokenizer.GetNextToken(); // Close brace
		layoutData += "}";

		Token endColon = tokenizer.PeekNextToken();
		if (endColon.GetData() == "as") {
			tokenizer.GetNextToken(); // skip it

			Token nameToken = tokenizer.GetNextToken();
			startName = nameToken.GetData();
			layoutName = startName; // so no modifying needs to happen
			
		}
		else {
			switch (type) {
			case ShaderType::VERTEX:
				layoutName = "In";
				break;
			case ShaderType::FRAGMENT:
				layoutName = "Out";
				break;
			}
			startName = "";
		}

		Token colon = tokenizer.GetNextToken();
		layoutData += ";";
#ifdef HT_DEBUG
		if (!(colon.GetData() == ";")) {
			tokenizer.Log("[HTSL] Unexpected token '%s', expected ';'", colon.GetData());
			return "";
		}
#endif // HT_DEBUG

		result += layoutName + layoutData;
		return result;
	}


}

#endif
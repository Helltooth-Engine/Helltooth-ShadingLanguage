#ifdef HT_DIRECTX

#include "parser/LayoutParser.hpp"

namespace htsl {

	LayoutParser* LayoutParser::s_Instance = nullptr;

	std::string LayoutParser::Parse(Tokenizer& tokenizer, const std::string& currentLine, const ShaderType& type) {
		std::string result;
		// First token is already "layout"

		Token openbrace = tokenizer.PeekNextToken();
		tokenizer.LogIf(openbrace, "{");
		
		result += "struct ";

		std::string layoutData = " {\n";
		int currentAttrib = 0;

		Token closebrace = tokenizer.PeekNextToken();
		while (closebrace.GetData() != "}") {
			layoutData += "\t";

			// First thing we parse is the actual type which has to be parsed by the TypeParser
			
			Token typeToken = tokenizer.GetNextToken();

			std::string parseResult;
#ifdef HT_DEBUG
			if (!TypeParser::Parse(typeToken, parseResult)) {
				tokenizer.Log("[HTSL] Unexpected token '%s'", typeToken.GetData().c_str());
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
				tokenizer.LogIf(as, "as");
			}
			// This is the actual name of the attribute that's supposed to be used
			Token attributeName = tokenizer.GetNextToken();
			attributes.push_back(attributeName.GetData());

			layoutData += attributeName.GetData();

			layoutData += " : " + s_LayoutAttribNames[currentAttrib++];

			// Expected semicolon
			Token semiColon = tokenizer.GetNextToken();
			tokenizer.LogIf(semiColon, ";");
			
			layoutData += semiColon.GetData() + "\n";
			
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
			hasName = true;
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
		tokenizer.LogIf(colon, ";");

		result += layoutName + layoutData;
		return result;
	}


}

#endif
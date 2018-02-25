#ifdef HT_OPENGL

#include "parser/LayoutParser.hpp"

namespace htsl {

	LayoutParser* LayoutParser::s_Instance = nullptr;

	std::string LayoutParser::Parse(Tokenizer& tokenizer, const std::string& currentLine, const ShaderType& type) {
		std::string result;
		// First token is already "layout"

		Token openbrace = tokenizer.PeekNextToken();
		if(!tokenizer.LogIf(openbrace, "{"))
			return "";

		// You need to add the layout location for each one of the ints

		std::string layout = "layout (location = ";
		std::string layout2 = ") ";
		switch (type) {
		case ShaderType::VERTEX:
			layout2 += "in ";
			break;
		case ShaderType::FRAGMENT:
			layout2 += "out ";
			break;
		}

		std::vector<std::pair<std::string, std::string>> layoutData;

		int currentTokenNumber = 0;
		Token closebrace = tokenizer.PeekNextToken();
		while (closebrace.GetData() != "}") {

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
			
			// Next token expected is the layout attribute if it's a vertex shader otherwise there's nothing like that
			Token nameorAttribName = tokenizer.GetNextToken();

			Token nextToken = tokenizer.PeekNextToken();
			if (nextToken.GetData() != ";") {
				AddAttribNames(nameorAttribName, tokenizer);

				Token as = tokenizer.GetNextToken();
				if (!tokenizer.LogIf(as, "as"))
					return "";
				Token attribName = tokenizer.GetNextToken();
				attributes.push_back(attribName.GetData());
			}
			else
				attributes.push_back(nameorAttribName.GetData());

			// Expected semicolon
			Token semiColon = tokenizer.GetNextToken();
			if(!tokenizer.LogIf(semiColon, ";"))
				return "";
			
			layoutData.push_back({ layout + std::to_string(currentTokenNumber) + layout2 + parseResult + " " , attributes[currentTokenNumber] + semiColon.GetData() + "\n" });

			closebrace = tokenizer.PeekNextToken();
			currentTokenNumber++;
		}

		tokenizer.GetNextToken(); // Close brace

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
		if(!tokenizer.LogIf(colon, ";"))
			return "";


		for (std::pair<std::string, std::string> layout : layoutData) {
			result += layout.first + layoutName + layout.second;
		}

		return result;
	}


}

#endif
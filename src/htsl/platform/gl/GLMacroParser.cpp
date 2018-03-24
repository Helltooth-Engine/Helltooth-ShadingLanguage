#ifdef HT_OPENGL
#include "parser/MacroParser.hpp"

namespace htsl {
	std::vector<std::pair<std::string, std::string>> MacroParser::s_MacroValues;
	
	std::string MacroParser::Parse(Tokenizer& tokenizer, const std::string& currentLine, const ShaderType& type) {
		std::string result;

		// currentLine contains the #
		// Tokenizer doesn't contain the #

		Token macroName = tokenizer.GetNextToken();

		if (macroName.GetData() == "htversion") {
			// Next type after the htversion is expected to be an int literal *MUST*
			result.append("#version ");
			Token versionNumber = tokenizer.GetNextToken();
			if ((versionNumber.GetData() == "#")) {
				result += MacroParser::Parse(tokenizer, tokenizer.GetNextLines(1), type);
			}
			else {
#ifdef HT_DEBUG
				if (versionNumber.GetType() != TokenType::INT_LITERAL) {
					tokenizer.Log("[HTSL] Version can't be anything else than an int literal");
					return "";
				}
#endif // HT_DEBUG

				result.append(versionNumber.GetData() + " ");
			
				// Test if the next thing after it is a string literal
				std::string line = tokenizer.GetNextLines(1);

				// Check if next token can be part of the version
				Token nextToken = tokenizer.PeekNextToken();
				if (nextToken.GetType() == TokenType::IDENTIFIER) {
					result.append(nextToken.GetData());
					// Skip it
					tokenizer.GetNextToken();
				}
			}
			result += "\n\n";
		}
		else {
			for (auto macroValue : s_MacroValues) {
				if (macroName.GetData() == macroValue.first) {
					result.append(macroValue.second);
				}
			}
		}

		return result;
	}

}

#endif
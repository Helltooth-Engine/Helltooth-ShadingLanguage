#ifdef HT_DIRECTX
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
#ifdef HT_DEBUG
			Token versionNumber = tokenizer.GetNextToken();
			if (!(versionNumber.GetData() == "#")) {
				if (versionNumber.GetType() != TokenType::INT_LITERAL)
					printf("[HTSL] Version can't be anything else than an int literal");
				// just skip it since this is directx
				// Test if the next thing after it is a string literal
				std::string line = tokenizer.GetNextLines(1);
				
				if (line == " core") {
					// Skip it
					tokenizer.GetNextToken();
				}
			}
#endif
		}
		else {
			// Test if it's part of the macro group
		}
		
		return result;
	}
}

#endif
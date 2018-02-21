#pragma once

#include "tokenizer/Tokenizer.hpp"

#include "parser/TypeParser.hpp"

namespace htsl {

	class StructParser {
	public:
		static std::string Parse(const Token& token, Tokenizer& tokenizer) {
			std::string result = "struct";

			Token name = tokenizer.GetNextToken();
			result += " " + name.GetData() + " {\n";

			Token openbrace = tokenizer.GetNextToken();
			if (!tokenizer.LogIf(openbrace, "{"))
				return "";

			Token closebrace = tokenizer.GetNextToken();
			while (closebrace.GetData() != "}") {
				
				// first token is the type
				std::string type;
#ifdef HT_DEBUG
				if (!TypeParser::Parse(closebrace, type)) {
					printf("[HTSL] Could not parse type\n");
					return "";
				}
#else
				TypeParser::Parse(closebrace, type);
#endif // HT_DEBUG

				result += "\t" + type + " ";

				// now comes za name

				Token name = tokenizer.GetNextToken();
				result += name.GetData();

				Token semiColon = tokenizer.GetNextToken();
				if(!tokenizer.LogIf(semiColon, ";"))
					return "";

				result += ";\n";

				closebrace = tokenizer.GetNextToken();
			}

			result += "}\n";

			return result;
		}


	};



}

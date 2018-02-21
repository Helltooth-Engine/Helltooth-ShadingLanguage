#ifdef HT_DIRECTX
#include "parser/InOutParser.hpp"


namespace htsl {
	
	InOutParser* InOutParser::s_Instance = nullptr;


	bool InOutParser::Parse(const Token& token, Tokenizer& tokenizer, std::string& result, const ShaderType& type) {
		// token is either in or out, tokenizer doesn't contain this token

		result += "struct ";

		bool out = false;

		std::string name;

		std::string data = " {\n";

		if (token.GetData() == "out") {
			// This is where I should add the position
			data += "\tfloat4 position : SV_POSITION;\n";
			m_OutAttribNames.push_back("SV_POSITION");
			out = true;
		}

		Token openbrace = tokenizer.GetNextToken();
		tokenizer.LogIf(openbrace, "{");
		
		Token closebrace = tokenizer.GetNextToken();

		while (closebrace.GetData() != "}") {
			std::string type;
#ifdef HT_DEBUG
			if (!TypeParser::Parse(closebrace, type)) {
				printf("[HTSL] Could not parse type\n");
				return false;
			}
#else
			TypeParser::Parse(closebrace, type);
#endif // HT_DEBUG

			data += "\t" + type + " ";

			Token name = tokenizer.GetNextToken();
			data += name.GetData();
			std::string uppername = name.GetData();

			for (auto & c : uppername) c = toupper(c);

			data += " : " + uppername;

			if (out) m_OutAttribNames.push_back(uppername);
			else m_InAttribNames.push_back(uppername);

			Token closeColon = tokenizer.GetNextToken();
			tokenizer.LogIf(closeColon, ";");
			data += ";\n";

			closebrace = tokenizer.GetNextToken();
		}

		data += "}";

		Token as = tokenizer.PeekNextToken();
		if (as.GetData() == "as") {
			tokenizer.GetNextToken();

			Token structName = tokenizer.GetNextToken();
			name = structName.GetData();
		}
		Token semiColon = tokenizer.GetNextToken();
		tokenizer.LogIf(semiColon, ";");

		result += name + data + ";\n";

		return true;
	}


}


#endif // HT_DIRECTX
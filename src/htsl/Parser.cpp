#include "Parser.hpp"

namespace htsl {
	Parser* Parser::s_Instance = nullptr;

	std::vector<std::string> Parser::Parse(const std::string& data) {
		// Give all macro data to the Macro Parser
		MacroParser::SetMacros(m_MacroValues);

		Tokenizer tokenizer(data);
		std::vector<std::string> result;

		while (tokenizer.HasNextToken()) {
			std::string currentResult = ParseShader(tokenizer);
			if (currentResult == "") {
				//something went wrong
#ifdef HT_DEBUG
				printf("%s", "[HTSL] Could not parse shader.\n");
#endif
				break;
			}
			result.push_back(currentResult);
		}

		// Reset all the macro data from the Macro Parser
		MacroParser::ClearMacros();

		return result;
	}

	std::string Parser::ParseShader(Tokenizer& tokenizer) {
		LayoutParser::Init();
		InOutParser::Init();
		UniformParser::Init();

		std::string firstLine = tokenizer.GetNextLines(1);
		ShaderType type;

		std::string result = "";
		std::smatch match;

		std::regex_search(firstLine, match, std::regex("^#htshader"));

		if (match.size()) {
			tokenizer.GetNextToken(); // '#'
			tokenizer.GetNextToken(); // 'htshader'
			Token shaderToken = tokenizer.GetNextToken();;
			if (shaderToken.GetData() == "vertex") {
				type = ShaderType::VERTEX; 
				goto parse;
			}
			else if (shaderToken.GetData() == "fragment") { 
				type = ShaderType::FRAGMENT; 
				goto parse;
			}
		}

#ifdef HT_DEBUG
		printf("%s", "[HTSL] Expected shading language type.\n");
#endif
		return "";
	parse:
		while (tokenizer.HasNextToken()) {
			std::string currentLine = tokenizer.GetNextLines(1);
			std::regex_search(currentLine, match, std::regex("^#htshader"));
			if (match.size())
				break;

			Token currentToken = tokenizer.GetNextToken();
			if (currentToken.GetType() == TokenType::EMPTY) 
				break;
			else if (currentToken.GetData() == "#") { //macro
				result += MacroParser::Parse(tokenizer, currentLine, type);
			}
			else if (currentToken.GetData() == "layout") { // Layout
				result += LayoutParser::Get()->Parse(tokenizer, currentLine, type) + "\n";
			}
			else if (currentToken.GetData() == "in" || currentToken.GetData() == "out") { // In out structs
				std::string parseResult;
#ifdef HT_DEBUG
				if (!InOutParser::Get()->Parse(currentToken, tokenizer, parseResult, type)) {
					tokenizer.Log("%s", "[HTSL] 'in' and 'out' are reserved keywords and should not be used for something else");
				}
#else
				InOutParser::Get()->Parse(currentToken, tokenizer, parseResult, type);
#endif // HT_DEBUG
				result += parseResult;
			}
			else if (currentToken.GetData() == "struct") {
				result += StructParser::Parse(currentToken, tokenizer);
			}
			else if (currentToken.GetData() == "uniform") {
				result += UniformParser::Get()->Parse(currentToken, tokenizer, type);
			}
			else if (currentToken.GetData() == "main_block") {
				result += MainParser::Parse(currentToken, tokenizer, type);
			}
			else {
				result += MethodParser::Get()->Parse(currentToken, tokenizer);
			}
		}

		switch (type) {
		case ShaderType::VERTEX:
			m_VertexShaderAttributes = LayoutParser::Get()->GetInputLayout();
			break;
		case ShaderType::FRAGMENT:
			m_FragmentShaderAttributes = LayoutParser::Get()->GetInputLayout();
			break;
		}

		UniformParser::End();
		InOutParser::End();
		LayoutParser::End();
		return result;
	}

}

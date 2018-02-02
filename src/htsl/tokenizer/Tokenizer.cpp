#include "Tokenizer.hpp"


namespace htsl {

	Tokenizer::Tokenizer(std::string data)
		: m_Data(data){
		// Tokens
		for (auto token : std::vector<const char*>{ "#", "\\(", "\\)", "=", ",", "\\*", "/", "-", "\\+", "\\{", "\\}", "\\.", ";", "\\[", "\\]" }) {
			m_Formats.push_back(new TokenFormat(TokenType::TOKEN, std::regex(std::string("^(") + token + ")")));
		}

		// Keywords
		for (const char* keyword : std::vector<const char*>{ "if", "as", "else", "while", "do", "for", "switch", "case", "default", "layout" }) {
			m_Formats.push_back(new TokenFormat(TokenType::KEYWORD, std::regex(std::string("^(") + keyword + ")")));
		}

		// Identifier
		m_Formats.push_back(new TokenFormat(TokenType::IDENTIFIER, std::regex("^([a-zA-Z][a-zA-Z0-9\\-\\_]{0,30})")));

		// Int literal
		m_Formats.push_back(new TokenFormat(TokenType::INT_LITERAL, std::regex("^([0-9]*)")));

		// Float literal
		m_Formats.push_back(new TokenFormat(TokenType::FLOAT_LITERAL, std::regex("^([0-9]*.[0-9]*[fF])")));

		for (size_t i = 0; i < m_Data.size(); i++)
			if (m_Data[i] == '\n') m_Data[i] = ' ';

		RemoveStartingSpaces();
		while (m_Data != "") {
			std::smatch match;
			bool found = false;
			for (TokenFormat* format : m_Formats) {
				std::regex_search(m_Data, match, format->GetPattern());
				if (match.size()) {
					std::ssub_match subMatch = match[0];
					std::string base = subMatch.str();
					m_Tokens.push_back(Token(format->GetType(), base));
					m_Data = m_Data.substr(base.size());
					RemoveStartingSpaces();
					found = true;
					break;
				}
			}
			if (!found) {
				std::cerr << "[HTSL] Could not find token!" << std::endl;
				__debugbreak();
			}
		}
	}

	Tokenizer::~Tokenizer() {
		for (auto tokenFormat : m_Formats)
			delete tokenFormat;
	}

}
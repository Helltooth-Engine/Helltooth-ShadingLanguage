#include "Tokenizer.hpp"

namespace htsl {

	Tokenizer::Tokenizer(const std::string& data)
		: lastToken(TokenType::UNKNOWN, "") {
		// Identifier
		m_Formats.push_back(TokenFormat(TokenType::IDENTIFIER, std::regex("^([a-zA-Z][a-zA-Z0-9\\-\\_]{0,30})")));
		
		// Float literal
		m_Formats.push_back(TokenFormat(TokenType::FLOAT_LITERAL, std::regex("^([0-9].\\.[0-9]*[fF]?)")));
		
		// Int literal
		m_Formats.push_back(TokenFormat(TokenType::INT_LITERAL, std::regex("^([0-9]{1,16})")));
		
		// Tokens
		for (auto token : std::vector<const char*>{ "#", "\\(", "\\)", "=", ",", "\\*", "/", "-", "\\+", "\\{", "\\}", "\\.", ";", "\\[", "\\]" }) {
			m_Formats.push_back(TokenFormat(TokenType::TOKEN, std::regex(std::string("^(") + token + ")")));
		}

		// Keywords
		for (const char* keyword : std::vector<const char*>{ "if", "as", "else", "while", "do", "for", "switch", "case", "default", "layout" }) {
			m_Formats.push_back(TokenFormat(TokenType::KEYWORD, std::regex(std::string("^(") + keyword + ")")));
		}

		m_Lines = Split(data, '\n');
	}

	Tokenizer::~Tokenizer() { }

	Token Tokenizer::GetNextToken() {
		if (lastToken.GetType() != TokenType::UNKNOWN) {
			Token copy = lastToken;
			lastToken = Token(TokenType::UNKNOWN, "");
			return copy;
		}

		// Start by removing spaces
		RemoveStartingSpaces(m_Lines[m_CurrentLine]);

		// While the currentline is empty
		while (m_CurrentLine + 1 < m_Lines.size() && m_Lines[m_CurrentLine] == "")
			RemoveStartingSpaces(m_Lines[++m_CurrentLine]);
		
		if (m_CurrentLine == m_Lines.size()) {
			reachedEnd = true;
			return Token(TokenType::EMPTY, "");
		}

		Token result(TokenType::EMPTY, "");
		std::smatch match;
		bool found = false;
		while (m_CurrentLine < m_Lines.size() && result.GetType() == TokenType::EMPTY) {
			if (m_Lines[m_CurrentLine] == "" && m_CurrentLine + 1 < m_Lines.size())
				m_CurrentLine++;

			for (TokenFormat format : m_Formats) {
				std::regex_search(m_Lines[m_CurrentLine], match, format.GetPattern());
				if (match.size()) {
					std::ssub_match subMatch = match[0];
					std::string base = subMatch.str();
					result = Token(format.GetType(), base);
					m_Lines[m_CurrentLine] = m_Lines[m_CurrentLine].substr(base.size());
					found = true;
					break;
				}
			}
			if (!found)
				m_CurrentLine++;
		}
		if (result.GetType() == TokenType::EMPTY)
			reachedEnd = true;

		return result;
	}

	Token Tokenizer::PeekNextToken() {
		// Start by removing spaces
		RemoveStartingSpaces(m_Lines[m_CurrentLine]);

		// While the currentline is empty
		while (m_CurrentLine + 1 < m_Lines.size() && m_Lines[m_CurrentLine] == "")
			RemoveStartingSpaces(m_Lines[++m_CurrentLine]);

		if (m_CurrentLine == m_Lines.size()) {
			reachedEnd = true;
			return Token(TokenType::EMPTY, "");
		}

		Token result(TokenType::EMPTY, "");
		std::smatch match;
		bool found = false;
		while (m_CurrentLine < m_Lines.size() && result.GetType() == TokenType::EMPTY) {
			for (TokenFormat format : m_Formats) {
				std::regex_search(m_Lines[m_CurrentLine], match, format.GetPattern());
				if (match.size()) {
					std::ssub_match subMatch = match[0];
					std::string base = subMatch.str();
					result = Token(format.GetType(), base);
					m_Lines[m_CurrentLine] = m_Lines[m_CurrentLine].substr(base.size());
					found = true;
					break;
				}
			}
			if (!found)
				m_CurrentLine++;
		}
		if (result.GetType() == TokenType::EMPTY)
			reachedEnd = true;

		lastToken = result;

		return result;
	}

}
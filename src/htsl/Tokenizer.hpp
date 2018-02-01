#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <functional>

#include "TokenFormat.hpp"

namespace htsl {

	class Tokenizer {
	private:
		std::string m_Data;

		std::vector<TokenFormat*> m_Formats;

		int m_CurrentToken = 0;

		std::vector<Token> m_Tokens;

	public:
		Tokenizer(std::string data);
		~Tokenizer();

		Token GetNextToken() { return m_Tokens[m_CurrentToken++]; }

		bool HasNextToken() { return m_CurrentToken < m_Tokens.size(); }


	private:
		inline void RemoveStartingSpaces() {
			while (m_Data[0] == ' ' || m_Data[0] == '\n' || m_Data[0] == '\t')
				m_Data = m_Data.substr(1);
		}
	};

}
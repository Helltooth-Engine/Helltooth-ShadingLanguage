#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <functional>
#include <sstream>

#include "TokenFormat.hpp"

namespace htsl {

	class Tokenizer {
	private:
		std::vector<std::string> m_Lines;

		std::vector<TokenFormat> m_Formats;

		int m_CurrentLine = 0;

		bool reachedEnd = false;
		Token lastToken;

	public:
		Tokenizer(const std::string& data);
		~Tokenizer();

		Token GetNextToken();
		Token PeekNextToken();
		bool HasNextToken() { return !reachedEnd; }

		inline std::string GetNextLines(int lines) {
			std::string result = "";
			for (int i = 0; i < lines; i++)
				result.append(m_Lines[m_CurrentLine + i]);
			return result;
		}

		template<typename First, typename ... Args>
		void Log(First arg, Args... message) const {
			printf(arg, std::forward<Args>(message)...);
			printf(" at line %d\n", m_CurrentLine);
		}

		bool LogIf(const Token& token, const std::string& expectedToken) {
#ifdef HT_DEBUG
			if (token.GetData() != expectedToken) {
				Log("[HTSL] Unexpected token '%s', expected '%s'", token.GetData().c_str(), expectedToken.c_str());
				return false;
			}
#endif
			return true;
		}

	private:
		inline void RemoveStartingSpaces(std::string& data) {
			while (data[0] == ' ' || data[0] == '\n' || data[0] == '\t')
				data = data.substr(1);
		}

		inline std::vector<std::string> Split(const std::string& data, char delimiter) {
			std::vector<std::string> strings;
			std::istringstream stream(data);
			
			std::string current;
			while (std::getline(stream, current, delimiter)) {
				strings.push_back(current);
			}

			return strings;
		}
	};

}
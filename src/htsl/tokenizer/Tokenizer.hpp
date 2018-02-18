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

	public:
		Tokenizer(const std::string& data);
		~Tokenizer();

		Token GetNextToken();
		bool HasNextToken() { return !reachedEnd; }

		inline std::string GetNextLines(int lines) {
			std::string result = "";
			for (int i = 0; i < lines; i++)
				result.append(m_Lines[m_CurrentLine + i]);
			return result;
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
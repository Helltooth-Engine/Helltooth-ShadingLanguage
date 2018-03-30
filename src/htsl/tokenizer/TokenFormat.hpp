#pragma once

#include <regex>

#include "Token.hpp"

namespace htsl {

	class TokenFormat {
	private:
		TokenType m_Type;
		std::regex m_Pattern;
		
	public:
		inline TokenFormat(const TokenType& type, const std::regex& pattern) : m_Type(type), m_Pattern(pattern) {}

		inline TokenType GetType() { return m_Type; }
		inline std::regex GetPattern() { return m_Pattern; }

	};

}
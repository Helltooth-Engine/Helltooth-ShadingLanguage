#pragma once

#include <regex>

namespace htsl {
	
	enum class TokenType {
		// When token is not initialized
		UNKNOWN,

		// Signifies the end of a whole file
		EMPTY,

		// ( ) = , * / - + { } # . : [ ]
		TOKEN,

		// if while for switch case
		KEYWORD,

		// starts with letter continues with letter/number/-/_
		IDENTIFIER,

		// numbers
		INT_LITERAL,

		// float expressions: e.g. 0.3f
		FLOAT_LITERAL,
	};

	class Token {
	private:
		TokenType m_Type;
		std::string m_Data;

	public:
		Token(TokenType type, std::string data) : m_Type(type), m_Data(data) {}
		
		inline TokenType GetType() const { return m_Type; }
		inline std::string GetData() const { return m_Data; }

		bool operator==(std::string other) const { return m_Data == other; }
	};

}
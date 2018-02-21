#pragma once

#include "tokenizer/Tokenizer.hpp"

#include "parser/ShaderTypes.hpp"
#include "parser/TypeParser.hpp"

namespace htsl {
	
	class InOutParser {
	private:
		std::vector<std::string> m_InAttribNames;
		std::vector<std::string> m_OutAttribNames;

		static InOutParser* s_Instance;

	public:
		bool Parse(const Token& token, Tokenizer& tokenizer, std::string& result, const ShaderType& type);

		inline static void Init() { if (!s_Instance) s_Instance = new InOutParser(); }
		inline static void End() { if (s_Instance) { delete s_Instance; s_Instance = nullptr; } }
		inline static InOutParser* Get() { return s_Instance; }

	};


}
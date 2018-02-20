#pragma once

#include <vector>
#include <string>
#include <regex>

#include "tokenizer/Tokenizer.hpp"

#include "parser/MacroParser.hpp"
#include "parser/ShaderTypes.hpp"
#include "parser/LayoutParser.hpp"

namespace htsl {

	class Parser {
	private:
		static Parser* s_Instance;

		std::vector<std::pair<std::string, std::string>> m_MacroValues;
	
		Parser() {}

	public:
		std::vector<std::string> Parse(const std::string& data);

		inline void Set(std::string name, std::string data) { m_MacroValues.push_back({ name, data }); }
		
		inline static void Init() { if (!s_Instance) s_Instance = new Parser(); }
		inline static void End() { if (s_Instance) { delete s_Instance; s_Instance = nullptr; } }
		inline static Parser* Get() { return s_Instance; }

	private:
		std::string ParseShader(Tokenizer& tokenizer);
	};
}
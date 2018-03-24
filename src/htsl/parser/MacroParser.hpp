#pragma once

#include "tokenizer/Tokenizer.hpp"

#include "ShaderTypes.hpp"

namespace htsl {

	class MacroParser {
		static std::vector<std::pair<std::string, std::string>> s_MacroValues;

	public:
		static std::string Parse(Tokenizer& tokenizer, const std::string& currentLine, const ShaderType& type);

		inline static void SetMacros(const std::vector<std::pair<std::string, std::string>>& macroValues) { s_MacroValues = macroValues; }
		inline static void ClearMacros() { s_MacroValues.clear(); }
	};
	
}
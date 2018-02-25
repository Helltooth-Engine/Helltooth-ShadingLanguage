#pragma once

#include <vector>
#include <string>
#include <regex>

#include "tokenizer/Tokenizer.hpp"

#include "parser/MacroParser.hpp"
#include "parser/ShaderTypes.hpp"
#include "parser/LayoutParser.hpp"
#include "parser/InOutParser.hpp"
#include "parser/StructParser.hpp"
#include "parser/UniformParser.hpp"
#include "parser/MethodParser.hpp"

namespace htsl {

	class Parser {
	private:
		static Parser* s_Instance;

		std::vector<std::pair<std::string, std::string>> m_MacroValues;
		std::vector<std::string> m_VertexShaderAttributes;
		std::vector<std::string> m_FragmentShaderAttributes;
		

		Parser() {}

	public:
		std::vector<std::string> Parse(const std::string& data);

		inline void Set(std::string name, std::string data) { m_MacroValues.push_back({ name, data }); }
		
		inline static void Init() { if (!s_Instance) s_Instance = new Parser(); }
		inline static void End() { if (s_Instance) { delete s_Instance; s_Instance = nullptr; } }
		inline static Parser* Get() { return s_Instance; }

		inline std::vector<std::string> GetVertexInputLayout() { return m_VertexShaderAttributes; }
		inline std::vector<std::string> GetFragmentInputLayout() { return m_FragmentShaderAttributes; }

	private:
		std::string ParseShader(Tokenizer& tokenizer);
	};
}
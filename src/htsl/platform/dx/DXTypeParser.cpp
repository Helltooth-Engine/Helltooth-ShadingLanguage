#ifdef HT_DIRECTX

#include "parser/TypeParser.hpp"
#include "parser/LayoutParser.hpp"
#include "parser/InOutParser.hpp"
#include "parser/UniformParser.hpp"

namespace htsl {

	bool TypeParser::Parse(const Token& token, std::string& parseResult, bool mainBlock) {
		if (token.GetType() == TokenType::IDENTIFIER) {
			if (token.GetData() == "vec2")
				parseResult = "float2";
			else if (token.GetData() == "vec3")
				parseResult = "float3";
			else if (token.GetData() == "vec4")
				parseResult = "float4";
			else if (token.GetData() == "mat3")
				parseResult = "float3x3";
			else if (token.GetData() == "mat4")
				parseResult = "float4x4";
			else if(mainBlock) {
				if(!LayoutParser::Get()->hasName)
					for (auto layoutName : LayoutParser::Get()->attributes)
						if (token.GetData() == layoutName) {
							parseResult = LayoutParser::Get()->layoutName + "." + token.GetData();
							return true;
						}

				if (!InOutParser::Get()->hasName) 
					for(auto inOutName : InOutParser::Get()->attribNames)
						if(token.GetData() == inOutName) {
							parseResult = InOutParser::Get()->name + "." + token.GetData();
							return true;
						}
				
				if (!UniformParser::Get()->hasName)
					for (auto inOutName : UniformParser::Get()->names)
						if (token.GetData() == inOutName) {
							parseResult = token.GetData();
							return true;
						}
				parseResult = token.GetData();

			}
			else {
				parseResult = token.GetData();
			}
			return true;
		}
		return false;
	}

}

#endif
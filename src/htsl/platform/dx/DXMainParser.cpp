#ifdef HT_DIRECTX
#include "parser/MainParser.hpp"

namespace htsl {

	std::string MainParser::Parse(const Token& token, Tokenizer& tokenizer, const ShaderType& type) {
		std::string result;

		std::string outStructName = "outData";
		std::string outStruct;
		std::string inStructName = "inData";
		std::string inStruct;

		if (type == ShaderType::VERTEX) {
			if (InOutParser::Get()->name == "") {
				result += "struct Out {\n\tfloat4 position : SV_POSITION;\n}\n";
				InOutParser::Get()->name = "Out";
				InOutParser::Get()->attribNames.push_back("position");
			}
			outStruct = InOutParser::Get()->name;
			inStruct = LayoutParser::Get()->layoutName;
			result += outStruct + " main(" + inStruct + " inData) {\n";
			InOutParser::Get()->name = outStructName;
			LayoutParser::Get()->layoutName = inStructName;
		}
		else if (type == ShaderType::FRAGMENT) {
			outStruct = LayoutParser::Get()->layoutName;
			inStruct = InOutParser::Get()->name;
			result += outStruct + " main(" + inStruct + " inData) {\n";
			//result += "float4 main(" + inStruct + " inData) : SV_TARGET {\n";

			LayoutParser::Get()->layoutName = outStructName;
			InOutParser::Get()->name = inStructName;
		}

		// Next expected token is a {

		Token openbrace = tokenizer.GetNextToken();
		if (!tokenizer.LogIf(openbrace, "{"))
			return "";

		result += "\t" + outStruct + " outData;\n";
		

		Token closebrace = tokenizer.GetNextToken();

		while (closebrace.GetData() != "}") {
			result += "\t";
			Token semiColon = closebrace;
			bool lastTokenIdentifier = false;
			bool lastTokenWasDot = false;
			bool fromLayout = false;
			int closingBrackets = 0;

			while (semiColon.GetData() != ";") {
				std::string typeParse;
				bool isMatrix = false;
				if (semiColon.GetType() == TokenType::IDENTIFIER) {
					for(std::string matricesName : UniformParser::Get()->matrices)
						if (semiColon.GetData() == matricesName) {
							isMatrix = true;
							break;
						}
					for (std::string matricesName : LayoutParser::Get()->matrices)
						if (semiColon.GetData() == matricesName) {
							isMatrix = true;
							fromLayout = true;
							break;
						}
				}

				if (semiColon.GetData() == inStruct) {
					Token dot = tokenizer.PeekNextToken();
					if (dot.GetData() == ".")
						result += inStructName;
				}
				else if (semiColon.GetData() == outStruct) {
					Token dot = tokenizer.PeekNextToken();
					if (dot.GetData() == ".")
						result += outStructName;
				}
				else if (semiColon.GetType() == TokenType::TOKEN) {
					if (semiColon.GetData() == ".") {
						lastTokenWasDot = true;
						result += semiColon.GetData();
					}
					else if (semiColon.GetData() == "=" || semiColon.GetData() == "*" || 
						semiColon.GetData() == "+" || semiColon.GetData() == "/" || semiColon.GetData() == "-")
						result += " " + semiColon.GetData() + " ";
					else if (semiColon.GetData() == ",")
						result += semiColon.GetData() + " ";
					else result += semiColon.GetData();
					lastTokenIdentifier = false;
				}
				else if (isMatrix) {
					Token multiply = tokenizer.PeekNextToken();
					if (multiply.GetData() == "*") {
						if (fromLayout) {
							TypeParser::Parse(semiColon, typeParse, !lastTokenWasDot);
							result += "mul(" + typeParse + ", ";
						} else 
							result += "mul(" + semiColon.GetData() + ", ";

						closingBrackets++;
						tokenizer.GetNextToken(); // skip *
					}
				}
				else if (!TypeParser::Parse(semiColon, typeParse, !lastTokenWasDot)) {
					result += semiColon.GetData();
					lastTokenIdentifier = false;
				}
				else {
					if (semiColon.GetData() == "sample2D" || semiColon.GetData() == "sample3D") {
						Token openbrace = tokenizer.GetNextToken();
						if (!tokenizer.LogIf(openbrace, "("))
							return "";

						Token textureName = tokenizer.GetNextToken();

						bool isArray = false;
						for (std::string textureArray : UniformParser::Get()->textureArrays)
							if (textureName == textureArray) {
								isArray = true;
								break;
							}
						
						if (isArray) {
							typeParse = "SampleTexture" + textureName.GetData() + "(";
							Token opensquare = tokenizer.GetNextToken();
							std::string index = tokenizer.GetNextToken().GetData();
							Token closesquare = tokenizer.GetNextToken();

							if (!tokenizer.LogIf(opensquare, "[") && !tokenizer.LogIf(closesquare, "]"))
								return "";

							typeParse += index;
						}
						else {
							typeParse = textureName.GetData() + ".sample(" + textureName.GetData() + "Sampler";
						}

					}
					else if (semiColon.GetData() == "OUT_POSITION")
						typeParse = outStructName + ".position";
					else if (semiColon.GetType() == TokenType::IDENTIFIER) {
						Token nextToken = tokenizer.PeekNextToken();
						if (nextToken.GetType() == TokenType::IDENTIFIER)
							result += semiColon.GetData() + " " + tokenizer.GetNextToken().GetData();
					}
					if (lastTokenIdentifier && !lastTokenWasDot)
						result += " " + typeParse;
					else
						result += typeParse;
					if (lastTokenWasDot)
						lastTokenIdentifier = false;
					lastTokenWasDot = false;
				}
				semiColon = tokenizer.GetNextToken();
			}
			for (int i = 0; i < closingBrackets; i++)
				result += ")";
			result += ";\n";
			closebrace = tokenizer.GetNextToken();
		}

		result += "\treturn " + outStructName + ";\n";
		result += "}\n";

		Token semiColon = tokenizer.GetNextToken();
		if (!tokenizer.LogIf(semiColon, ";"))
			return "";

		return result;
	}

}

#endif // HT_DIRECTX
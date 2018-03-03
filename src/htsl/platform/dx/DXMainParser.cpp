#ifdef HT_DIRECTX
#include "parser/MainParser.hpp"
#include "parser/LayoutParser.hpp"
#include "parser/InOutParser.hpp"
#include "parser/UniformParser.hpp"

namespace htsl {

	std::string MainParser::Parse(const Token& token, Tokenizer& tokenizer, const ShaderType& type) {
		std::string result;

		std::string outStructName = "outData";
		std::string outStruct;
		std::string inStructName = "inData";
		std::string inStruct;

		if (type == ShaderType::VERTEX) {
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

			while (semiColon.GetData() != ";") {
				std::string typeParse;
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
				else if (!TypeParser::Parse(semiColon, typeParse, !lastTokenWasDot)) {
					result += semiColon.GetData();
					lastTokenIdentifier = false;
				}
				else {
					if (semiColon.GetData() == "sample2D" || semiColon.GetData() == "sample3D")
						typeParse = "texture";
					else if (semiColon.GetData() == "OUT_POSITION")
						typeParse = outStructName + ".position";
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
			result += ";\n";
			closebrace = tokenizer.GetNextToken();
		}

		result += "\treturn " + outStructName + ";\n";
		result += "}\n";

		Token semiColon = tokenizer.GetNextToken();
		if (!tokenizer.LogIf(semiColon, ";"))
			return "";

		for (std::string matrix : UniformParser::Get()->matrices)
			std::cout << matrix << " ";
		std::cout << std::endl;

		return result;
	}

}

#endif // HT_DIRECTX
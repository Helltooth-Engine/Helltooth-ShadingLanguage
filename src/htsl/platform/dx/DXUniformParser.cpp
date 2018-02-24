#ifdef HT_DIRECTX

#include "parser/UniformParser.hpp"
#include "parser/TypeParser.hpp"

namespace htsl {

	UniformParser* UniformParser::s_Instance = nullptr;

	std::string GenerateSwitch(std::string textureName, std::string textureCount, bool is3D = false) {
		std::string result = "float4 SampleTexture" + textureName + "(float tID, float" + (is3D ? "3" : "2") + " uvs) {\n";
		result += "\tswitch (tID) {\n";

		for (int i = 0; i < std::stoi(textureCount); i++) {
			std::string currentArrayValue = "[" + std::to_string(i) + "]";
			result += "\tcase " + std::to_string(i) + ":\n";
			result += "\t\treturn " + textureName + currentArrayValue + ".Sample(" + textureName + "Sampler" + currentArrayValue + ", uvs);\n";

		}
		result += "\t}\n}\n";

		return result;
	}

	std::string UniformParser::Parse(const Token& token, Tokenizer& tokenizer, const ShaderType& type) {
		std::string result;
		
		// First token is the uniform keyword
		std::string textureBlock;

		std::string switchCase;

		bool hasSwitch = false;

		result += "cbuffer ";

		std::string bufferData = " : register(b0) {\n"; //only one uniform buffer per shader

		Token openbrace = tokenizer.GetNextToken();
		
		if (!tokenizer.LogIf(openbrace, "{"))
			return "";
		
		Token closebrace = tokenizer.GetNextToken();

		bool isOnlyTexture = true;

		int textureRegisterNumber = 0;
		std::string textureRegister = " : register(t";
		std::string samplerRegister = " : register(s";
		while (closebrace.GetData() != "}") {
			//First is the type;

			bool isTexture;
			bool isTexture3D = false;
			if (closebrace.GetData() != "Texture2D" && closebrace.GetData() != "Texture3D") {
				std::string currentType;
#ifdef HT_DEBUG
				if (!TypeParser::Parse(closebrace, currentType)) {
					tokenizer.Log("[HTSL] Could not parse type.\n");
					return "";
				}
#else
				TypeParser::Parse(closebrace, currentType);
#endif
				bufferData += "\t" + currentType + " ";
				isTexture = false;
				isOnlyTexture = false;
			}
			else {
				if (closebrace.GetData() == "Texture3D") {
					textureBlock += "TextureCube ";
					isTexture3D = true;
				}

				else textureBlock += closebrace.GetData();
				textureBlock += " ";
				isTexture = true;
			}

			// Next thing is the name
			Token nameToken = tokenizer.GetNextToken();
			if (isTexture) {
				textureBlock += nameToken.GetData();
			}
			else {
				names.push_back(nameToken.GetData());
				bufferData += nameToken.GetData();
			}
			std::string arrayString;

			Token isArray = tokenizer.PeekNextToken(); 
			int arrayNumber = 1;
			if (isArray.GetData() == "[") {
				// this shit is an array yo
				tokenizer.GetNextToken();
				Token number = tokenizer.GetNextToken();
				arrayNumber = std::stoi(number.GetData());
				Token closeSquareBrace = tokenizer.GetNextToken();
				if (!tokenizer.LogIf(closeSquareBrace, "]"))
					return "";
				arrayString = "[" + number.GetData() + "]";

				if(isTexture) textureArrays.push_back(nameToken.GetData());
				switchCase += GenerateSwitch(nameToken.GetData(), number.GetData(), isTexture3D);
				hasSwitch = true;
			}
			else if (isTexture) textureNames.push_back(nameToken.GetData());

			if (isTexture) {
				textureBlock += arrayString + textureRegister + std::to_string(textureRegisterNumber) + ");\n";
				std::string samplerName = nameToken.GetData() + "Sampler";
				textureBlock += "SamplerState " + samplerName + arrayString + samplerRegister + std::to_string(textureRegisterNumber) + ");\n";
			}
			else {
				bufferData += arrayString + ";\n";
			}

			textureRegisterNumber += arrayNumber;

			Token semiColon = tokenizer.GetNextToken();
			if (!tokenizer.LogIf(semiColon, ";"))
				return "";
			closebrace = tokenizer.GetNextToken();
		}

		bufferData += "}";

		Token as = tokenizer.PeekNextToken();

		if (as.GetData() == "as") {
			tokenizer.GetNextToken();

			// next token is name

			Token nameToken = tokenizer.GetNextToken();
			name = nameToken.GetData();
			as = tokenizer.GetNextToken();
			hasName = true;
		}
		else {
			hasName = false;
			name = "Uniforms";
		}

		if (!tokenizer.LogIf(as, ";"))
			return "";

		bufferData += ";";

		result = (hasSwitch ? ("\n\n" + switchCase) : "") + textureBlock + (!isOnlyTexture ? (result + name + bufferData) : "");
		return result;
	}

	
}


#endif // HT_DIRECTX
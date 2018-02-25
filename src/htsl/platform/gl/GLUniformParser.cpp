#ifdef HT_OPENGL
#include "parser/UniformParser.hpp"
#include "parser/TypeParser.hpp"


namespace htsl {

	UniformParser* UniformParser::s_Instance = nullptr;

	std::string UniformParser::Parse(const Token& token, Tokenizer& tokenizer, const ShaderType& type) {
		std::string result;

		// First token is the uniform keyword
		std::string textureBlock;

		result += "uniform ";

		std::string bufferData = " {\n"; //only one uniform buffer per shader

		Token openbrace = tokenizer.GetNextToken();

		if (!tokenizer.LogIf(openbrace, "{"))
			return "";

		Token closebrace = tokenizer.GetNextToken();

		bool isOnlyTexture = true;

		int textureRegister = 0;

		while (closebrace.GetData() != "}") {
			//First is the type;

			bool isTexture;

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
				if (closebrace.GetData() == "Texture3D") textureBlock += "samplerCube";
				else textureBlock += "sampler2D";
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
			if (isArray.GetData() == "[") {
				// this shit is an array yo
				tokenizer.GetNextToken();
				Token number = tokenizer.GetNextToken();
				Token closeSquareBrace = tokenizer.GetNextToken();
				if (!tokenizer.LogIf(closeSquareBrace, "]"))
					return "";
				arrayString = "[" + number.GetData() + "]";

				if (isTexture) textureArrays.push_back(nameToken.GetData());
			}
			else if (isTexture) textureNames.push_back(nameToken.GetData());

			if (isTexture) {
				textureBlock += arrayString + ";\n";
			}
			else {
				bufferData += arrayString + ";\n";
			}

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
			name = "Uniforms";
			hasName = false;
		}

		if (!tokenizer.LogIf(as, ";"))
			return "";

		bufferData += ";";

		result = textureBlock + "\n" + (!isOnlyTexture ? (result + name + bufferData) : "") + "\n";
		return result;
	}

}

#endif // HT_OPENGL
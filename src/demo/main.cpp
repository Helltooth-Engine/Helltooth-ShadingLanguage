#include <fstream>
#include <string>
#include <sstream>

#include <iostream>

#include <chrono>

#include <Tokenizer.hpp>

int main(int argc, char* argv[]) {
	


	std::ifstream f("res/demo1.vert");
	
	f.seekg(0, f.end);
	int length = f.tellg();
	f.seekg(0, f.beg);

	char* buffer = new char[length];
	f.read(buffer, length);

	std::string result(buffer);

	f.close();

	std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();

	htsl::Tokenizer tokenizer(result);

	while (tokenizer.HasNextToken()) {
		std::cout << tokenizer.GetNextToken().GetData() << std::endl;
	}

	std::chrono::duration<double> time = std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - t1);
	
	std::cout << "It took " << time.count() << " to tokenize the entire string." << std::endl;
	std::cin.get();

	return 0;
}
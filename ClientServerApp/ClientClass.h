#pragma once
#include <iostream>

class ClientClass {
public:
	void EnterRequest(int& a, int& b) {
		std::cout << "Hello, Enter your operand of example via enter button: ";
		std::cin >> a;
		std::cin >> b;
	}
	void GetResultOnDisplay(int& res) {
		std::cout << "Result of your request: " << res;
	}
};
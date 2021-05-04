#pragma once
#include <string>
#include <vector>

struct Message {
	std::string head, code, tail;
	std::vector<std::string> params;
};
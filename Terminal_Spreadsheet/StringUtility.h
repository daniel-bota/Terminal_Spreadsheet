#pragma once
#include <string>
#include <vector>
#include <format>

class StringUtility
{
public:
	static std::vector<std::string> Split(const std::string& input, char delimiter);
	static std::string Trim(const std::string& input);
	static std::string ValueToDraw(bool header, bool text, const std::string& value, int cellW);
	static bool IsNumber(const std::string& input, double& output);
	static std::string GenerateRowTitle(int index);
	static std::string GenerateColTitle(int index);
};


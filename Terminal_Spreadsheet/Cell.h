#pragma once
#include <string>
#include <vector>
#include "Address.h"
#include "StringUtility.h"
#include <memory>
#include <variant>

class Sheet;

class Cell
{
public:
	virtual ~Cell() = default;
	Cell(Sheet* sheet) : sheet(sheet) {};
	Cell(const Address& address, Sheet* sheet);
	Cell(const Address& address, int width, int height, Sheet* sheet);

	//virtual ValueString
	virtual std::string ValueString() const = 0;
	virtual std::string ValueToDraw() const = 0;
	virtual const std::variant<std::monostate, std::string, double>& Value() const { return value; }
	const std::string& Title() const { return title; }
	const Address& GetAddress() const { return address; }
	int Width() const { return width; }
	int Height() const { return height; }
	virtual int Index() const { return -1; }
	const std::vector<Address>& Referencing() const { return referencing; }
	void SetReferencingCells(const std::vector<Address>& newReferencing);
	void AddReferencingCell(const Address& address);
	void RemoveReferencingCell(const Address& address);
	void NotifyReferencingCells(const Address& notificationSource);

protected:
	void GenerateTitle(const std::string& colTitle, const std::string& rowTitle);
	std::string GenerateRowTitle(int index) const;
	std::string GenerateColTitle(int index) const;

	class Address address { -1, -1 };
	std::string title{ "" };
	int width{ 0 }, height{ 0 };
	std::variant<std::monostate, std::string, double> value;
	Sheet* sheet;
	std::vector<Address> referencing{};
};


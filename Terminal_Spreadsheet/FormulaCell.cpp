#include "FormulaCell.h"

const std::variant<std::monostate, std::string, double>& FormulaCell::GetUpdated()
{
    Update();

    return formula.Value();
}

const std::variant<std::monostate, std::string, double>& FormulaCell::Value() const
{
    return formula.Value();
}

std::string FormulaCell::ValueString() const
{
    if (!Valid())
    {
        return std::format("{}   {}", Expression(), ErrorMessage());
    }

    return std::holds_alternative<std::string>(Value()) ?
        std::format("{} ({})", std::get<std::string>(Value()), Expression()) : 
        std::format("{} ({})", std::to_string(std::get<double>(Value())), Expression());
}

std::string FormulaCell::ValueToDraw() const
{
    if (!Valid())
    {
        return StringUtility::ValueToDraw(false, true, formula.ErrorMessage(), width);
    }

    return std::holds_alternative<std::string>(Value()) ?
        StringUtility::ValueToDraw(false, true, std::get<std::string>(Value()), width) : 
        StringUtility::ValueToDraw(false, false, std::to_string(std::get<double>(Value())), width);
}

const std::string& FormulaCell::Expression() const
{
    return formula.Expression();
}

const std::string& FormulaCell::ErrorMessage() const
{
    return formula.ErrorMessage();
}

bool FormulaCell::Valid() const
{
    return formula.Valid();
}

void FormulaCell::SetFormula(const Formula& newForm)
{
    formula = newForm;
    status = OutOfDate;
}

void FormulaCell::NotifyOutOfDate(const Address& notificationSource)
{
    if (notificationSource == this->address)
    {
        return;
    }

    status = OutOfDate;
    NotifyReferencingCells(notificationSource);
}

void FormulaCell::Update(bool removeDependencies)
{
    if (status == UpToDate && !removeDependencies) { return; }

    formula.Parse(removeDependencies);
    status = UpToDate;
}

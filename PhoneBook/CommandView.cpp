#include "CommandView.h"

// All output of CommandView's methods is reference of 'mInput'
// to avoid creating and copying (std::string) for every method
const std::string& CommandView::GetCommand() const
{
    std::cout << std::endl;
    std::cout << "[0] Insert" << std::endl;
    std::cout << "[1] Search" << std::endl;
    std::cout << "[2] Remove" << std::endl;
    std::cout << "[3] Update" << std::endl;
    std::cout << "[4] Exit" << std::endl;
    std::cout << "Press a command: ";
    std::getline(std::cin, mInput);
    return mInput;
}
const std::string& CommandView::GetName() const
{
    std::cout << "Name: ";
    std::getline(std::cin, mInput);
    return mInput;
}
const std::string& CommandView::GetAddress() const
{
    std::cout << "Addres: ";
    std::getline(std::cin, mInput);
    return mInput;
}
const std::string& CommandView::GetPhoneNumber() const
{
    std::cout << "Phone Number(only " << PHONE_NUMBER_SIZE << " digits allowed, ex - 12345678) : ";
    std::getline(std::cin, mInput);
    return mInput;
}
const std::string& CommandView::GetQueryString() const
{
    std::cout << "Enter a query: ";

    std::getline(std::cin, mInput);
    return mInput;
}


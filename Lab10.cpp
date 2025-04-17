/*
Name: Talan Tabares
Date: 4/16/2025
Subject: Lab 10
Description: Basic calculator that does addition


Inputs:
Input File (user specified)

Outputs:
addDoubleStrings

*/



#include <iostream>     // Library for input/output operations 
#include <fstream>      // Library for file reading 
#include <string>       // Library that gives the string class
#include <cctype>       // Library to help with character checking
#include <algorithm>    // Library to give some algorithms like reverse and max


std::string trim(const std::string& str) { // Remove whitespace from characters
    size_t start = str.find_first_not_of(" \t\r\n");  // Find first non whitespace character
    size_t end = str.find_last_not_of(" \t\r\n");     // Find last non whitespace character
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);  // Return the reduced string or return nothing if not found
}


bool isValidDoubleFormat(const std::string& s) { // Validate that a string is correctly formatted
    std::string str = trim(s);    // Remove whitespace from the string
    if (str.empty()) return false; // Check if string is empty and return false

    size_t i = 0;
    if (str[i] == '+' || str[i] == '-') i++;  // Skip optional sign

    bool hasDigitsBeforeDot = false;
    while (i < str.length() && isdigit(str[i])) { // Read the digits before the decimal
        hasDigitsBeforeDot = true; // Assign to true to denote digit before decimal
        i++;
    }

    bool hasDecimal = false; 
    bool hasDigitsAfterDot = false;

    if (i < str.length() && str[i] == '.') {  // Checks if decimal point is found
        hasDecimal = true; // Assign value to true if decimal point is found
        i++;
        while (i < str.length() && isdigit(str[i])) {  // Read digits after the decimal
            hasDigitsAfterDot = true; 
            i++;
        }
    }

    
    return (hasDigitsBeforeDot || (hasDecimal && hasDigitsAfterDot)) && // If format is good return true
           (!hasDecimal || hasDigitsAfterDot) && i == str.length(); // Else, return false
}


std::string removeLeadingZeros(const std::string& num) { // Remove leading zeros from the string
    size_t i = 0;
    while (i < num.length() && num[i] == '0') i++;  // Skip the leading zeros
    return (i == num.length()) ? "0" : num.substr(i);  // Return 0 if all were zeros
}

void splitDouble(const std::string& s, char& sign, std::string& intPart, std::string& fracPart) { // Split double into sign, integer and fraction
    std::string str = trim(s);  // Remove whitespace
    sign = '+';                 // Default sign
    size_t i = 0;

    if (str[i] == '+' || str[i] == '-') {  // Check for and get sign
        sign = str[i];
        i++;
    }

    size_t dotPos = str.find('.', i);  // Find position of decimal point
    if (dotPos == std::string::npos) {  // No decimal point found
        intPart = str.substr(i);
        fracPart = "";
    } else {
        intPart = str.substr(i, dotPos - i);     // Characters before decimal
        fracPart = str.substr(dotPos + 1);       // Characters after decimal
    }

    intPart = removeLeadingZeros(intPart);  // Normalize integer
}


std::string addStrings(const std::string& a, const std::string& b) { // String addition with non-negative integers
    std::string result;
    int carry = 0, sum = 0;
    int i = int(a.length()) - 1, j = int(b.length()) - 1;  // Start from end

    while (i >= 0 || j >= 0 || carry) {
        int x = (i >= 0) ? a[i--] - '0' : 0;  // Get digit or 0
        int y = (j >= 0) ? b[j--] - '0' : 0;  // Get digit or 0
        sum = x + y + carry;                 // Sum with carry operation
        result += char(sum % 10 + '0');      // Append last digit of sum to result
        carry = sum / 10;                    // Update carry operator
    }

    std::reverse(result.begin(), result.end());  // Reverse to get correct order
    return result;
}

std::string subtractStrings(const std::string& a, const std::string& b) { // Subtract non-negative strings
    std::string result;
    int borrow = 0;
    int i = int(a.length()) - 1, j = int(b.length()) - 1;

    while (i >= 0) {
        int x = a[i--] - '0';
        int y = (j >= 0) ? b[j--] - '0' : 0;

        x -= borrow;
        if (x < y) {
            x += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }

        result += char(x - y + '0');
    }

    while (result.length() > 1 && result.back() == '0')  // Remove trailing zeros
        result.pop_back();

    std::reverse(result.begin(), result.end());  // Reverse result
    return result;
}

void alignFractionParts(std::string& f1, std::string& f2) { // Align lengths of zeros
    size_t len = std::max(f1.size(), f2.size());  // Find max length
    f1.append(len - f1.size(), '0');              // Pad with zeros
    f2.append(len - f2.size(), '0');              // Pad with zeros
}

std::string addDoubleStrings(const std::string& s1, const std::string& s2) { // add string doubles and returns a string
    char sign1, sign2;
    std::string int1, frac1, int2, frac2;
    splitDouble(s1, sign1, int1, frac1);  // Parse through the first number
    splitDouble(s2, sign2, int2, frac2);  // Parse through the second number

    alignFractionParts(frac1, frac2);     // Equalize fractional parts
    std::string fracResult, intResult;
    char resultSign;

    if (sign1 == sign2) {  // If both have same sign do regular operation
        fracResult = addStrings(frac1, frac2);
        bool carry = false;
        if (fracResult.length() > frac1.length()) {  // Check for carry operator
            carry = true;
            fracResult = fracResult.substr(1);
        }

        intResult = addStrings(int1, int2);          // Add integer parts
        if (carry)
            intResult = addStrings(intResult, "1");  // Apply carry operator if needed

        resultSign = sign1;  // Keep original sign
    } else {
        // If the signs are different do subtraction
        std::string whole1 = int1 + frac1;  // Combine values
        std::string whole2 = int2 + frac2;

        // Determine which number is larger to do subtraction correctly
        if (whole1.length() > whole2.length() || (whole1.length() == whole2.length() && whole1 >= whole2)) {
            resultSign = sign1;
            if (frac1 < frac2) {
                int1 = subtractStrings(int1, "1");
                fracResult = subtractStrings("1" + std::string(frac1.length(), '0'), frac2);
            } else {
                fracResult = subtractStrings(frac1, frac2);
            }
            intResult = subtractStrings(int1, int2);
        } else {
            resultSign = sign2;
            if (frac2 < frac1) {
                int2 = subtractStrings(int2, "1");
                fracResult = subtractStrings("1" + std::string(frac2.length(), '0'), frac1);
            } else {
                fracResult = subtractStrings(frac2, frac1);
            }
            intResult = subtractStrings(int2, int1);
        }
    }

    while (!fracResult.empty() && fracResult.back() == '0')  // Clean up leading zeros
        fracResult.pop_back();

    std::string result = (resultSign == '-' ? "-" : "") + intResult;  // Add sign
    if (!fracResult.empty()) result += "." + fracResult;             // Add decimal part if necessary
    return result;
}

int main() {
    std::string filename;
    std::cout << "Enter filename containing numbers: ";  // Get file name from user
    std::cin >> filename;

    std::ifstream file(filename);  // Open the file if possible
    if (!file) {                   // Check if file was opened
        std::cerr << "Failed to open file.\n"; // Error if file wasnt opened
        return 1;
    }

    std::string line;
    std::string base = "-123.456";  // Constant double string to add

    while (std::getline(file, line)) {  // Read each line of the file
        line = trim(line);              // Trim whitespace from each line
        if (isValidDoubleFormat(line)) {  // Validate lines are in double format
            std::string result = addDoubleStrings(line, base);  // Add strings
            std::cout << line << " + " << base << " = " << result << "\n";  // Print result of operations
        } else {
            std::cout << "Invalid number: " << line << "\n";  // Print error if necessary
        }
    }

    file.close();  // Close the file
    return 0;      // Return 0 to show that program ran successfully
}

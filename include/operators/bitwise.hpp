/*
    ===========================================================================
    Bitwise operators
    ===========================================================================
*/

#ifndef BIG_INT_BITWISE_OPERATORS_HPP
#define BIG_INT_BITWISE_OPERATORS_HPP

#include <climits>
#include <vector>

#include "BigInt.hpp"
#include "constructors/constructors.hpp"
#include "functions/conversion.hpp"
#include "functions/math.hpp"
#include "operators/arithmetic_assignment.hpp"
#include "operators/assignment.hpp"
#include "operators/binary_arithmetic.hpp"

/*
    decimal_to_binary
    -----------------
    Convert BigInt to binary as string representation
*/

std::vector<bool> decimal_to_binary(const std::string& num) {
    BigInt quotient = BigInt(num);
    std::string val = num;
    BigInt rem;
    std::vector<bool> bin;

    while (quotient != 0) {
        bin.push_back((quotient.to_string().back() - '0') % 2);
        quotient /= BigInt(2);
    }
    return bin;
}

/*
    binary_to_decimal
    -----------------
    Convert string representation of a binary to BigInt
*/

BigInt binary_to_decimal(const std::vector<bool>& bin) {
    BigInt num = 0;
    for (size_t i = 0; i < bin.size(); ++i) {
        num += (bin.at(i) * pow(BigInt(2), (int)i));
    }
    return num;
}

/*
    twos_complement
    ---------------
    Helper function that returns the string type binary representation of a
    BigInt.
*/

void twos_complement(std::vector<bool>& bin) {
    bin.flip();
    for (size_t i = 0; i < bin.size(); ++i) {
        if (bin.at(i))
            bin.at(i) = 0;
        else {
            bin.at(i) = 1;
            return;
        }
    }
}

/*
    BigInt | BigInt
    ---------------
*/

BigInt BigInt::operator|(const BigInt& num) const {
    // Use built in or operation if number is within long long range
    if (num >= LLONG_MIN && num <= LLONG_MAX &&
        *this >= LLONG_MIN && *this <= LLONG_MAX) {
        return (this->to_long_long() | num.to_long_long());
    }

    std::vector<bool> lhs = decimal_to_binary(this->value);
    std::vector<bool> rhs = decimal_to_binary(num.value);

    // Extend smaller vector to be same size as larger vector
    if (lhs.size() < rhs.size()) {
        lhs.insert(lhs.end(), rhs.size() - lhs.size(), 0);
    }
    else if (lhs.size() > rhs.size()) {
        rhs.insert(rhs.end(), lhs.size() - rhs.size(), 0);
    }

    // Apply twos complement if either value is negative
    if (this->sign == '-') {
        twos_complement(lhs);
    }
    if (num.sign == '-') {
        twos_complement(rhs);
    }

    // Or operation
    std::vector<bool> res;
    for (size_t i = 0; i < rhs.size(); ++i) {
        res.push_back(lhs.at(i) || rhs.at(i));
    }

    BigInt temp = BigInt(1);
    if (this->sign == '-' || num.sign == '-') {
        twos_complement(res);
        temp = -temp;
    }
    temp *= BigInt(binary_to_decimal(res));
    return temp;
}

/*
    BigInt & BigInt
    ---------------
*/

BigInt BigInt::operator&(const BigInt& num) const {
    // Use built in and operation if number is within long long range
    if (num >= LLONG_MIN && num <= LLONG_MAX &&
        *this >= LLONG_MIN && *this <= LLONG_MAX) {
        return (this->to_long_long() & num.to_long_long());
    }

    std::vector<bool> lhs = decimal_to_binary(this->value);
    std::vector<bool> rhs = decimal_to_binary(num.value);

    // Extend smaller vector to be same size as larger vector
    if (lhs.size() < rhs.size()) {
        lhs.insert(lhs.end(), rhs.size() - lhs.size(), 0);
    }
    else if (lhs.size() > rhs.size()) {
        rhs.insert(rhs.end(), lhs.size() - rhs.size(), 0);
    }

    // Apply twos complement if either value is negative
    if (this->sign == '-') {
        twos_complement(lhs);
    }
    if (num.sign == '-') {
        twos_complement(rhs);
    }

    // And operation
    std::vector<bool> res;
    for (size_t i = 0; i < rhs.size(); ++i) {
        res.push_back(lhs.at(i) && rhs.at(i));
    }

    BigInt temp = BigInt(1);
    if (this->sign == '-' && num.sign == '-') {
        twos_complement(res);
        temp = -temp;
    }
    temp *= BigInt(binary_to_decimal(res));
    return temp;
}

/*
    BigInt ^ BigInt
    ---------------
*/

BigInt BigInt::operator^(const BigInt& num) const {
    // Use built in and operation if number is within long long range
    if (num >= LLONG_MIN && num <= LLONG_MAX &&
        *this >= LLONG_MIN && *this <= LLONG_MAX) {
        return (this->to_long_long() ^ num.to_long_long());
    }

    std::vector<bool> lhs = decimal_to_binary(this->value);
    std::vector<bool> rhs = decimal_to_binary(num.value);

    // Extend smaller vector to be same size as larger vector
    if (lhs.size() < rhs.size()) {
        lhs.insert(lhs.end(), rhs.size() - lhs.size(), 0);
    }
    else if (lhs.size() > rhs.size()) {
        rhs.insert(rhs.end(), lhs.size() - rhs.size(), 0);
    }

    // Apply twos complement if either value is negative
    if (this->sign == '-') {
        twos_complement(lhs);
    }
    if (num.sign == '-') {
        twos_complement(rhs);
    }

    // Xor operation
    std::vector<bool> res;
    for (size_t i = 0; i < rhs.size(); ++i) {
        res.push_back(lhs.at(i) ^ rhs.at(i));
    }

    BigInt temp = BigInt(1);
    if ((this->sign == '-') ^ (num.sign == '-')) {
        twos_complement(res);
        temp = -temp;
    }
    temp *= BigInt(binary_to_decimal(res));
    return temp;
}

/*
    ~BigInt
    -------
*/

BigInt BigInt::operator~() const {
    BigInt temp = *this;
    if (temp.sign == '+') {
        temp.sign = '-';
        temp -= 1;
    }
    else {
        temp.sign = '+';
        temp -= 1;
    }
    return temp;
}

/*
    BigInt >> BigInt
    ----------------
*/

BigInt BigInt::operator>>(const BigInt& num) const {
    // Use built in << operation if number is within long long range
    if (num >= LLONG_MIN && num <= LLONG_MAX &&
        *this >= LLONG_MIN && *this <= LLONG_MAX) {
        return (this->to_long_long() >> num.to_long_long());
    }
    return *this / pow(2, num.to_int());
}

/*
    BigInt << BigInt
    ----------------
    Zero fills. Relies on an arbitrary width, limited only by available memory.
    Undefined behavior if either argument is negative.
*/

BigInt BigInt::operator<<(const BigInt& num) const {
    // Use built in << operation if number is within long long range
    if (num >= LLONG_MIN && num <= LLONG_MAX &&
        *this >= LLONG_MIN && *this <= LLONG_MAX) {
        return (this->to_long_long() << num.to_long_long());
    }
    return *this * pow(2, num.to_int());
}

#endif  // BIG_INT_BITWISE_OPERATORS_HPP
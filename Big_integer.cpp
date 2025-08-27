#include <bits/stdc++.h>
using namespace std;
using namespace chrono;

class BigInteger
{
public:
    // constructors
    BigInteger() : sign(1) {}
    BigInteger(const string &number) { parse(number); }
    BigInteger(unsigned long long number) { parse(to_string(number)); }

    // addition
    BigInteger operator+(const BigInteger &other) const
    {
        if (sign == other.sign)
        {
            BigInteger ans = add(*this, other);
            ans.sign = sign;
            return ans;
        }
        if (abs_compare(*this, other) >= 0)
        {
            BigInteger ans = subtract(*this, other);
            ans.sign = sign;
            return ans;
        }
        else
        {
            BigInteger ans = subtract(other, *this);
            ans.sign = other.sign;
            return ans;
        }
    }

    // subtraction
    BigInteger operator-(const BigInteger &other) const
    {
        if (sign != other.sign)
        {
            BigInteger ans = add(*this, other);
            ans.sign = sign;
            return ans;
        }
        if (abs_compare(*this, other) >= 0)
        {
            BigInteger ans = subtract(*this, other);
            ans.sign = sign;
            return ans;
        }
        else
        {
            BigInteger ans = subtract(other, *this);
            ans.sign = -other.sign;
            return ans;
        }
    }

    // multiplication
    BigInteger operator*(const BigInteger &other) const
    {
        BigInteger ans = multiply(*this, other);
        ans.sign = sign * other.sign;
        return ans;
    }

    // division
    BigInteger operator/(const BigInteger &other) const
    {
        if (other == BigInteger("0"))
            throw invalid_argument("Division by zero is not allowed.");
        BigInteger ans = divide(*this, other);
        ans.sign = sign * other.sign;
        return ans;
    }

    // modulo
    BigInteger operator%(const BigInteger &other) const
    {
        if (other == BigInteger("0"))
            throw invalid_argument("Division by zero");
        BigInteger div = *this / other;
        BigInteger rem = *this - (div * other);
        if (rem < BigInteger("0"))
            rem = rem + other;
        return rem;
    }

    // equal
    bool operator==(const BigInteger &other) const
    {
        return sign == other.sign && digits == other.digits;
    }

    // not equal
    bool operator!=(const BigInteger &other) const
    {
        return !(*this == other);
    }
    // less than
    bool operator<(const BigInteger &other) const
    {
        if (sign != other.sign)
            return sign < other.sign;
        if (digits.size() != other.digits.size())
            return sign == 1 ? digits.size() < other.digits.size() : digits.size() > other.digits.size();
        for (int i = digits.size() - 1; i >= 0; i--)
        {
            if (digits[i] != other.digits[i])
                return sign == 1 ? digits[i] < other.digits[i] : digits[i] > other.digits[i];
        }
        return false;
    }

    // greater than
    bool operator>(const BigInteger &other) const
    {
        return other < *this;
    }

    // less than or equal to
    bool operator<=(const BigInteger &other) const
    {
        return !(*this > other);
    }

    // greater than or equal to
    bool operator>=(const BigInteger &other) const
    {
        return !(*this < other);
    }

    // bitwise and
    BigInteger operator&(const BigInteger &other) const
    {
        BigInteger ans;
        ans.digits.resize(max(digits.size(), other.digits.size()), 0);
        for (size_t i = 0; i < ans.digits.size(); i++)
        {
            int a = (i < digits.size()) ? digits[i] : 0;
            int b = (i < other.digits.size()) ? other.digits[i] : 0;
            ans.digits[i] = a & b;
        }
        ans.remove_leading_zeros();
        return ans;
    }

    // bitwise or
    BigInteger operator|(const BigInteger &other) const
    {
        BigInteger ans;
        ans.digits.resize(max(digits.size(), other.digits.size()), 0);
        for (size_t i = 0; i < ans.digits.size(); i++)
        {
            int a = (i < digits.size()) ? digits[i] : 0;
            int b = (i < other.digits.size()) ? other.digits[i] : 0;
            ans.digits[i] = a | b;
        }
        ans.remove_leading_zeros();
        return ans;
    }

    // bitwise xor
    BigInteger operator^(const BigInteger &other) const
    {
        BigInteger ans;
        ans.digits.resize(max(digits.size(), other.digits.size()), 0);
        for (size_t i = 0; i < ans.digits.size(); i++)
        {
            int a = (i < digits.size()) ? digits[i] : 0;
            int b = (i < other.digits.size()) ? other.digits[i] : 0;
            ans.digits[i] = a ^ b;
        }
        ans.remove_leading_zeros();
        return ans;
    }

    // bitwise not (negation)
    BigInteger operator~() const
    {
        BigInteger ans;
        ans.digits.resize(digits.size(), 0);
        for (size_t i = 0; i < digits.size(); i++)
            ans.digits[i] = 9 - digits[i];
        ans.remove_leading_zeros();
        return ans;
    }

    // logical and
    bool operator&&(const BigInteger &other) const
    {
        return !is_zero() && !other.is_zero();
    }

    // logical or
    bool operator||(const BigInteger &other) const
    {
        return !is_zero() || !other.is_zero();
    }

    // logical not
    bool operator!() const
    {
        return is_zero();
    }

    // check if big integer is zero or not
    bool is_zero() const
    {
        return digits.size() == 1 && digits[0] == 0;
    }

    // increment operator (++BigInt)
    BigInteger &operator++()
    {
        *this = *this + BigInteger("1");
        return *this;
    }

    // decrement operator (--BigInt)
    BigInteger &operator--()
    {
        *this = *this - BigInteger("1");
        return *this;
    }

    // output
    friend ostream &operator<<(ostream &os, const BigInteger &number)
    {
        if (number.sign == -1)
            os << '-';
        for (auto it = number.digits.rbegin(); it != number.digits.rend(); it++)
            os << *it;
        return os;
    }

private:
    vector<int> digits;
    int sign;
    void parse(const string &number)
    {
        sign = (number[0] == '-') ? -1 : 1;
        int start = (number[0] == '-' || number[0] == '+') ? 1 : 0;
        for (int i = number.size() - 1; i >= start; i--)
            digits.push_back(number[i] - '0');
    }

    static BigInteger add(const BigInteger &a, const BigInteger &b)
    {
        BigInteger ans;
        int carry = 0;
        size_t n = max(a.digits.size(), b.digits.size());
        for (size_t i = 0; i < n || carry; i++)
        {
            int sum = carry;
            if (i < a.digits.size())
                sum += a.digits[i];
            if (i < b.digits.size())
                sum += b.digits[i];
            ans.digits.push_back(sum % 10);
            carry = sum / 10;
        }
        return ans;
    }

    static BigInteger subtract(const BigInteger &a, const BigInteger &b)
    {
        BigInteger ans;
        int carry = 0;
        for (size_t i = 0; i < a.digits.size(); i++)
        {
            int diff = a.digits[i] - (i < b.digits.size() ? b.digits[i] : 0) - carry;
            carry = 0;
            if (diff < 0)
            {
                diff += 10;
                carry = 1;
            }
            ans.digits.push_back(diff);
        }
        ans.remove_leading_zeros();
        return ans;
    }

    static BigInteger multiply(const BigInteger &a, const BigInteger &b)
    {
        BigInteger ans;
        ans.digits.resize(a.digits.size() + b.digits.size());
        for (size_t i = 0; i < a.digits.size(); i++)
        {
            for (size_t j = 0; j < b.digits.size(); j++)
            {
                ans.digits[i + j] += a.digits[i] * b.digits[j];
                if (ans.digits[i + j] >= 10)
                {
                    ans.digits[i + j + 1] += ans.digits[i + j] / 10;
                    ans.digits[i + j] %= 10;
                }
            }
        }
        ans.remove_leading_zeros();
        return ans;
    }

    static BigInteger divide(const BigInteger &a, const BigInteger &b)
    {
        if (b == BigInteger("0"))
            throw invalid_argument("Division by zero");

        BigInteger dividend = a.abs();
        BigInteger divisor = b.abs();
        BigInteger ans("0");
        BigInteger current("0");

        for (int i = dividend.digits.size() - 1; i >= 0; i--)
        {
            current = current * BigInteger(10) + BigInteger(dividend.digits[i]);
            int count = 0;
            while (current >= divisor)
            {
                current = current - divisor;
                count++;
            }
            ans.digits.push_back(count);
        }

        reverse(ans.digits.begin(), ans.digits.end());
        ans.remove_leading_zeros();
        return ans;
    }

    static int abs_compare(const BigInteger &a, const BigInteger &b)
    {
        if (a.digits.size() != b.digits.size())
            return a.digits.size() - b.digits.size();
        for (int i = a.digits.size() - 1; i >= 0; i--)
        {
            if (a.digits[i] != b.digits[i])
                return a.digits[i] - b.digits[i];
        }
        return 0;
    }

    void remove_leading_zeros()
    {
        while (digits.size() > 1 && digits.back() == 0)
            digits.pop_back();
        if (digits.size() == 1 && digits[0] == 0)
            sign = 1;
    }

    BigInteger abs() const
    {
        BigInteger ans = *this;
        ans.sign = 1;
        return ans;
    }
};

BigInteger factorial(int n)
{
    static unordered_map<int, BigInteger> fact_memo;
    if (n == 0 || n == 1)
        return BigInteger("1");
    if (fact_memo.count(n))
        return fact_memo[n];
    return fact_memo[n] = BigInteger(n) * factorial(n - 1);
}

// BigInteger catalan(int n)
// {
//     return factorial(2 * n) / (factorial(n + 1) * factorial(n));
// }
BigInteger catalan(int n)
{
    vector<BigInteger> ans(n + 1);
    ans[0] = BigInteger("1");

    for (int i = 1; i <= n; i++)
    {
        for (int j = 0; j < i; j++)
            ans[i] = ans[i] + (ans[j] * ans[i - j - 1]);
    }
    return ans[n];
}

pair<BigInteger, BigInteger> fib_matrix(BigInteger n)
{
    if (n == BigInteger("0"))
        return {BigInteger("0"), BigInteger("1")};
    auto p = fib_matrix(n / BigInteger("2"));
    BigInteger c = p.first * (BigInteger("2") * p.second - p.first);
    BigInteger d = p.first * p.first + p.second * p.second;
    if (n % BigInteger("2") == BigInteger("0"))
        return {c, d};
    else
        return {d, c + d};
}

BigInteger fibonacci(BigInteger n)
{
    return fib_matrix(n).first;
}

BigInteger power(const BigInteger &base, const BigInteger &exponent)
{
    if (exponent == BigInteger("0"))
        return BigInteger("1");

    BigInteger ans("1"), exp = exponent, b = base;

    while (exp > BigInteger("0"))
    {
        if ((exp % BigInteger("2")) == BigInteger("1"))
        {
            ans = ans * b;
        }
        b = b * b;
        exp = exp / BigInteger("2");
    }

    return ans;
}

int main()
{
    while (true)
    {
        cout << "\n----- MENU -----" << endl;
        cout << "1. Factorial" << endl;
        cout << "2. Fibonacci" << endl;
        cout << "3. Catalan Number" << endl;
        cout << "4. Power (Base ^ Exponent)" << endl;
        cout << "5. Exit" << endl;
        cout << "----------------" << endl;
        cout << "Enter your choice (1-5): ";
        int choice;
        cin >> choice;

        if (!cin)
        { // Input validation for non-integer inputs
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number between 1 and 5.\n";
            continue;
        }

        if (choice == 5)
        {
            cout << "Exiting program... Goodbye!" << endl;
            break;
        }

        int n, a, b;
        BigInteger result;

        switch (choice)
        {
        case 1:
            cout << "Enter a number for factorial: ";
            cin >> n;
            if (n < 0)
            {
                cout << "Factorial is not defined for negative numbers." << endl;
            }
            else
            {
                auto start = high_resolution_clock::now();
                result = factorial(n);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(end - start);
                cout << "Factorial(" << n << ") = " << result << endl;
                cout << "Time taken: " << duration.count() << " ms" << endl;
            }
            break;

        case 2:
            cout << "Enter a number for Fibonacci: ";
            cin >> n;
            if (n < 0)
            {
                cout << "Fibonacci is not defined for negative numbers." << endl;
            }
            else
            {
                auto start = high_resolution_clock::now();
                result = fibonacci(BigInteger(n));
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(end - start);
                cout << "Fibonacci(" << n << ") = " << result << endl;
                cout << "Time taken: " << duration.count() << " ms" << endl;
            }
            break;

        case 3:
            cout << "Enter a number for Catalan number: ";
            cin >> n;
            if (n < 0)
            {
                cout << "Catalan number is not defined for negative numbers." << endl;
            }
            else
            {
                auto start = high_resolution_clock::now();
                result = catalan(n);
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(end - start);
                cout << "Catalan(" << n << ") = " << result << endl;
                cout << "Time taken: " << duration.count() << " ms" << endl;
            }
            break;

        case 4:
            cout << "Enter base: ";
            cin >> a;
            if (a < 0)
            {
                cout << "Negative base is not handled in this version." << endl;
                break;
            }
            cout << "Enter exponent: ";
            cin >> b;
            if (b < 0)
            {
                cout << "Negative exponents are not handled in this version." << endl;
                break;
            }
            else
            {
                auto start = high_resolution_clock::now();
                result = power(BigInteger(a), BigInteger(b));
                auto end = high_resolution_clock::now();
                auto duration = duration_cast<milliseconds>(end - start);
                cout << a << " ^ " << b << " = " << result << endl;
                cout << "Time taken: " << duration.count() << " ms" << endl;
            }
            break;

        default:
            cout << "Invalid choice, please try again." << endl;
            continue;
        }
    }

    return 0;
}

#ifndef EXP_H
#define EXP_H

#include "linkedlist.hpp"
#include <iostream>

const std::string VALID_CHARS = "0123456789.xX^+-";


typedef struct ExpData{
    double _coef;
    int    _expn;
} ExpData;

class Expression : public LinkedList<ExpData> {
    friend std::ostream &operator<< (std::ostream &out, const Expression &exp);
public:
    Expression();
    explicit Expression(std::string src);
    void insert(const ExpData &data);
    std::string toPlainString(bool space=true, bool caret=false) const;
    std::string toDblLine(const std::string &info="", bool newline = false, bool space=true) const;
    Expression devirate() const;
    double evaluate(double val) const;
    Expression operator+(const Expression &exp) const;
    Expression operator-(const Expression &exp) const;
    Expression operator*(const Expression &exp) const;
    Expression operator+(const ExpData &data) const;
    Expression operator-(const ExpData &data) const;
    Expression operator*(const ExpData &data) const;
    Expression operator/(const ExpData &data) const;
    Expression operator+(double val) const;
    Expression operator-(double val) const;
    Expression operator*(double val) const;
    Expression operator/(double val) const;
    Expression &operator+=(const Expression &exp);
    Expression &operator-=(const Expression &exp);
    Expression &operator*=(const Expression &exp);
    Expression &operator+=(const ExpData &data);
    Expression &operator-=(const ExpData &data);
    Expression &operator*=(const ExpData &data);
    Expression &operator/=(const ExpData &data);
    Expression &operator+=(double val);
    Expression &operator-=(double val);
    Expression &operator*=(double val);
    Expression &operator/=(double val);
};

#endif // !EXP_H

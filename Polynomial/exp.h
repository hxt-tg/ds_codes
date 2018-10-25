#ifndef EXP_H
#define EXP_H

#include "linkedlist.hpp"
#include <iostream>

typedef struct ExpData{
    double _coef;
    int    _expn;
} ExpData;

class Expression : public LinkedList<ExpData> {
    friend std::ostream &operator<< (std::ostream &out, const Expression &exp);
public:
    Expression();
    explicit Expression(std::string src);
    void insert(ExpData &data);
    std::string toPlainString(bool space=true, bool caret=false) const;
    std::string toDblLine(const std::string &info="", bool newline = false, bool space=true) const;
    Expression devirate();
    double evaluate(double val);
    Expression operator+(Expression &exp);
    Expression operator-(Expression &exp);
    Expression operator*(Expression &exp);
    Expression operator+(ExpData &data);
    Expression operator-(ExpData &data);
    Expression operator*(ExpData &data);
    Expression operator/(ExpData &data);
    Expression operator+(double val);
    Expression operator-(double val);
    Expression operator*(double val);
    Expression operator/(double val);
    Expression &operator+=(Expression &exp);
    Expression &operator-=(Expression &exp);
    Expression &operator*=(Expression &exp);
    Expression &operator+=(ExpData &data);
    Expression &operator-=(ExpData &data);
    Expression &operator*=(ExpData &data);
    Expression &operator/=(ExpData &data);
    Expression &operator+=(double val);
    Expression &operator-=(double val);
    Expression &operator*=(double val);
    Expression &operator/=(double val);
};

#endif // !EXP_H

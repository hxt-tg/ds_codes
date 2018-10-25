#include "exp.h"
#include <string>
#include <sstream>

const std::string VALID_CHARS = "0123456789.x^+-";

bool checkExp(std::string src) {
    if (src.length() == 0) err("Empty expression.");
    enum { NUM_MODE, OP_MODE } chk_mode = OP_MODE; /* check_mode */
    int num_len = 0; /* check number overflow */
    int chk_X = 0, chk_dot = 0, chk_up = 0;
    for (auto p = src.begin(); p < src.end(); p++) {
        /* Check invalid character */
        if (VALID_CHARS.find(*p) < 0) err("Invalid char '" + std::to_string(*p) + "'.");

        /* Definite error condition */
        if (*p == '-' || *p == '+') {
            /* Enter operator mode */
            chk_mode = OP_MODE;
            chk_X = 0;
            chk_dot = 0;
            chk_up = 0;
            if (p < src.end() - 1 && (p[1] == '+' || p[1] == '-')) {
                /* [+,-][+,-] */
                err("Lost operand.");
            }
        } else {
            /* Enter number mode */
            /* Record first enter NUM_MODE position */
            chk_mode = NUM_MODE;
            if (*p == '.') {
                if (p < src.end() - 1 && (p[1] == 'x' || p[1] == 'X' || p[1] == '+' || p[1] == '-')) {
                    /* .i	.<op> */
                    err("Unexpected dot.");
                }
                chk_dot++;
            }
            if (*p == '^')
                chk_up++;
            // Record first 'X' position to process <Excepted operator>	
            if (*p == 'x' || *p == 'X') {
                if (p < src.end()-1 && (p[1] >= '0' && p[1] <= '9')) {
                    err("Expected symbol '^'.");
                }
                if (p < src.end() - 1 && p[1] == '.') {
                    err("Unexpected dot.");
                }
                chk_X++;
            }
        }
        /* Error condition */
        /* ..	.<num>. */
        if (chk_dot > 1)
            err("Invalid dot.");
        /* ii	i<num>i */
        if (chk_X > 1)
            err("Lost operator.");
        if (chk_up > 1)
            err("Invalid symbol '^'.");
    }
    return true;
}

std::string preProcess(std::string src) {
    std::string dest = "";
    for (auto p = src.begin(); p < src.end(); p++)
        if (*p == 'x') dest += 'X';
        else dest += *p;
    if (src[src.size() - 1] == 'X') dest += '1';
    src = dest;
    dest = "";
    if (src[0] == 'X') dest += '1'; 
    if (src[0] == '-') dest += '0';
    for (auto p = src.begin(); p < src.end(); p++) {
        if (*p == '^') continue; 
        dest += *p;
        /* "<op>X" -> "<op>1X" */
        if ((*p == '+' || *p == '-') && p[1] == 'X')
            dest += '1';
        if (*p == 'X' && (p[1] == '-' || p[1] == '+')) 
            dest += '1';
    }
    return dest;
}

ExpData parseTerm(std::string term) {
    char sgn = '+';
    if (term[0] == '+' || term[0] == '-') {
        sgn = term[0];
        term = term.substr(1);
    }
    int xpos = term.find('X');
    ExpData data = { 0, 0 };
    if (xpos < 0) data._coef = std::stof(term);
    else {
        data._coef = std::stof(term.substr(0, xpos));
        data._expn = std::stoi(term.substr(xpos + 1));
    }
    if (sgn == '-') data._coef *= -1;
    return data;
}


/* Expression methods */
Expression::Expression() : LinkedList<ExpData>() {}

Expression::Expression(std::string src) 
    : LinkedList<ExpData>() {
    if (!checkExp(src)) return ;
    src = preProcess(src);
    std::string termBuf = "";

    for (auto p = src.begin(); p < src.end(); p++) {
        if (p == src.end()-1 || *p == '+' || *p == '-') {
            if (p == src.end() - 1) termBuf += *p;
            ExpData data = parseTerm(termBuf);
            Expression::insert(data);
            termBuf = "";
            termBuf += *p;
        } else termBuf += *p;
    }
}

void Expression::insert(ExpData &data) {
    unsigned int i = 0;
    LinkedNode<ExpData> *tp = head()->next;
    while (i<size() && tp->data._expn > data._expn) {
        i++;
        tp = tp->next;
    }
    if (i<size() && tp->data._expn == data._expn) {
        /* If term still exist after add its coef. */
        if (std::fabs(tp->data._coef + data._coef) > 1e-7)
            tp->data._coef += data._coef;
        else
            LinkedList<ExpData>::remove(tp);
    } else {
        LinkedList<ExpData>::insert(data, i);
    }
}

std::string Expression::toPlainString(bool space, bool caret) const {
    std::stringstream ss;
    const std::string spaceStr = space ? " " : "";
    LinkedNode<ExpData> *tp = head()->next;
    for (unsigned int i = 0; i < size(); tp = tp->next, i++) {
        if (std::fabs(tp->data._coef) < 1e-7) continue;
        if (i == 0)
            if (fabs(tp->data._coef - 1.0)<1e-7 || fabs(tp->data._coef + 1.0)<1e-7)
                ss << (tp->data._coef>0 ? "" : "-") << (tp->data._expn == 0 ? "1" : "");
            else
                ss << tp->data._coef << spaceStr;
        else
            if (fabs(tp->data._coef - 1.0)<1e-7 || fabs(tp->data._coef + 1.0)<1e-7)
                ss << (tp->data._coef>0 ? "+" : "-") << spaceStr << (tp->data._expn == 0 ? "1" : "");
            else
                ss << (tp->data._coef>0 ? "+" : "-") << spaceStr << fabs(tp->data._coef) << spaceStr;
        if (tp->data._expn) {
            ss << "X";
            if (tp->data._expn < 0 || tp->data._expn > 1) ss << (caret ? "^" : "") << tp->data._expn;
            ss << spaceStr;
        }
    }
    if (ss.str().size() == 0) ss << "0";
    return ss.str();
}

std::string Expression::toDblLine(const std::string &info, bool newline, bool space) const {
    std::string src = toPlainString(space, false);
    std::stringstream ss_up, ss_down;
    bool isPrintUp = false;
    ss_down << info;
    for (size_t i = 0; i < info.size(); i++)
        ss_up << ' ';
    for (auto p = src.begin(); p < src.end(); p++) {
        if (*p == 'X') {
            ss_up << ' ';
            ss_down << 'X';
            int j = 1;		/* Traverse ss_up */
            if (p[j] == '-') {
                ss_up << '-';
                ss_down << ' ';
                j++;
            }
            while (p[j] >= '0' && p[j] <= '9') {
                if (!isPrintUp) isPrintUp = true;
                ss_up << p[j];
                ss_down << ' ';
                j++;
            }
            p += j-1;
        } else {
            ss_up << ' ';
            ss_down << *p;
        }
    }
    return (isPrintUp ? ss_up.str()+"\n" : "") + ss_down.str() + (newline ? "\n" : "");
}

std::ostream &operator<<(std::ostream &out, const Expression &exp) {
    out << exp.toPlainString(false, true);
    return out;
}

Expression Expression::devirate() {
    Expression exp;
    ExpData data;
    LinkedNode<ExpData> *tp = head()->next;
    for (unsigned int i = 0; i < size(); i++, tp = tp->next) {
        data._coef = tp->data._coef * tp->data._expn;
        if (fabs(data._coef)<1e-7) continue;
        data._expn = tp->data._expn - 1;
        exp.insert(data);
    }
    return exp;
}

double Expression::evaluate(double val) {
    double ans = 0;
    LinkedNode<ExpData> *tp = head()->next;
    for (unsigned int i = 0; i < size(); i++, tp = tp->next) 
        ans += std::pow(val, tp->data._expn) * tp->data._coef;
    return ans;
}

Expression Expression::operator+(Expression &exp) {
    Expression ans(*this);
    ans += exp;
    return ans;
}

Expression Expression::operator-(Expression &exp) {
    Expression ans(*this);
    ans -= exp;
    return ans;
}

Expression Expression::operator*(Expression &exp) {
    Expression ans(*this);
    ans *= exp;
    return ans;
}

Expression Expression::operator+(ExpData &data) {
    Expression ans(*this);
    ans += data;
    return ans;
}

Expression Expression::operator-(ExpData &data) {
    Expression ans(*this);
    ans -= data;
    return ans;
}

Expression Expression::operator*(ExpData &data) {
    Expression ans(*this);
    ans *= data;
    return ans;
}

Expression Expression::operator/(ExpData &data) {
    Expression ans(*this);
    ans /= data;
    return ans;
}

Expression Expression::operator+(double val) {
    Expression ans(*this);
    ans += val;
    return ans;
}

Expression Expression::operator-(double val) {
    Expression ans(*this);
    ans -= val;
    return ans;
}

Expression Expression::operator*(double val) {
    Expression ans(*this);
    ans *= val;
    return ans;
}

Expression Expression::operator/(double val) {
    Expression ans(*this);
    ans /= val;
    return ans;
}

Expression &Expression::operator+=(Expression &exp) {
    LinkedNode<ExpData> *tp = exp.head()->next;
    for (unsigned int i = 0; i < exp.size(); i++, tp = tp->next)
        *this += tp->data;
    return *this;
}

Expression &Expression::operator-=(Expression &exp) {
    LinkedNode<ExpData> *tp = exp.head()->next;
    for (unsigned int i = 0; i < exp.size(); i++, tp = tp->next)
        *this -= tp->data;
    return *this;
}

Expression &Expression::operator*=(Expression &exp) {
    Expression e(*this);
    LinkedList<ExpData>::~LinkedList();
    LinkedNode<ExpData> *ta = e.head()->next, *tb = exp.head()->next;
    ExpData d;
    for (unsigned int i = 0; i < e.size(); i++, ta = ta->next) {
        for (unsigned int j = 0; j < exp.size(); j++, tb = tb->next) {
            d._coef = ta->data._coef * tb->data._coef;
            d._expn = ta->data._expn + tb->data._expn;
            insert(d);
        }
        tb = exp.head()->next;
    }
    return *this;
}

Expression &Expression::operator+=(ExpData &data) {
    this->insert(data);
    return *this;
}

Expression &Expression::operator-=(ExpData &data) {
    ExpData d = { data._coef*-1, data._expn };
    this->insert(d);
    return *this;
}

Expression &Expression::operator*=(ExpData &data) {
    Expression exp(*this);
    LinkedList<ExpData>::~LinkedList();
    ExpData d;
    LinkedNode<ExpData> *tp = exp.head()->next;
    for (unsigned int i = 0; i < exp.size(); i++, tp = tp->next) {
        d._coef = tp->data._coef * data._coef;
        d._expn = tp->data._expn + data._expn;
        insert(d);
    }
    return *this;
}

Expression &Expression::operator/=(ExpData &data) {
    if (std::fabs(data._coef) < 1e-7) err("Cannot divide by 0.");
    LinkedNode<ExpData> *tp = head()->next;
    for (unsigned int i = 0; i < size(); i++, tp = tp->next) {
        tp->data._coef /= data._coef;
        tp->data._expn -= data._expn;
    }
    return *this;
}

Expression &Expression::operator+=(double val) {
    if (std::fabs(val) < 1e-7) return *this;
    ExpData d = { val, 0 };
    insert(d);
    return *this;
}

Expression &Expression::operator-=(double val) {
    if (std::fabs(val) < 1e-7) return *this;
    ExpData d = { -val, 0 };
    insert(d);
    return *this;
}

Expression &Expression::operator*=(double val) {
    if (std::fabs(val) < 1e-7) {
        LinkedList<ExpData>::~LinkedList();
        return *this;
    }
    LinkedNode<ExpData> *tp = head()->next;
    for (unsigned int i = 0; i < size(); i++, tp = tp->next)
        tp->data._coef *= val;
    return *this;
}

Expression &Expression::operator/=(double val) {
    if (std::fabs(val) < 1e-7) err("Cannot divide by 0.");
    LinkedNode<ExpData> *tp = head()->next;
    for (unsigned int i = 0; i < size(); i++, tp = tp->next)
        tp->data._coef /= val;
    return *this;
}

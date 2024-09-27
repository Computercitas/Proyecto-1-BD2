#include <vector>
#include <string>

using namespace std;

class Stm {
public:
    virtual ~Stm() = default;
    virtual std::string accept(class ImpVisitor* v) = 0;
};

class Exp {
public:
    virtual ~Exp() = default;
    virtual std::string accept(class ImpVisitor* v) = 0;
};

class IdExp : public Exp {
public:
    IdExp(std::string id);
    ~IdExp();
    std::string accept(class ImpVisitor* v);
    std::string getId() const;

private:
    std::string id;
};

class ParenthExp : public Exp {
public:
    ParenthExp(Exp *e);
    ~ParenthExp();
    std::string accept(class ImpVisitor* v);

private:
    Exp* e;
};

class Program {
public:
    Program();
    ~Program();
    void add(Stm* s);
    std::string accept(class ImpVisitor* v);

private:
    std::vector<Stm*> slist;
};

class ImpVisitor {
public:
    virtual std::string visit(IdExp* e) = 0;
    virtual std::string visit(ParenthExp* e) = 0;
    virtual std::string visit(Program* p) = 0;
};

class ConcreteVisitor : public ImpVisitor {
public:
    std::string visit(IdExp* e);
    std::string visit(ParenthExp* e);
    std::string visit(Program* p);
};


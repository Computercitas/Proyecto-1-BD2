#include "gramatica.hh"


IdExp::IdExp(std::string id) : id(std::move(id)) {}

IdExp::~IdExp() {}

string IdExp::accept(ImpVisitor* v) {
    return v->visit(this);
}

ParenthExp::ParenthExp(Exp *e) : e(e) {}

ParenthExp::~ParenthExp() {}

string ParenthExp::accept(ImpVisitor* v) {
    return v->visit(this);
}

Program::Program() : slist() {}

Program::~Program() {
    for (auto s : slist) {
        delete s;
    }
}

void Program::add(Stm* s) {
    slist.push_back(s);
}

string Program::accept(ImpVisitor* v) {
    return v->visit(this);
}

string ConcreteVisitor::visit(IdExp* e) {
    return "Visiting IdExp: " + e->getId();
}

string ConcreteVisitor::visit(ParenthExp* e) {
    return "Visiting ParenthExp";
}

string ConcreteVisitor::visit(Program* p) {
    return "Visiting Program";
}

string IdExp::getId() const {
    return id;
}

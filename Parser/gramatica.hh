#ifndef GRAMATICA_H
#define GRAMATICA_H

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


//----------------------------------------------------------------
class CreateTableStatement : public Stm {
public:
    string table_name;
    string file_path;
    string key_column;

    CreateTableStatement(string table, string file, string key)
        : table_name(table), file_path(file), key_column(key) {}

    string accept(class ImpVisitor* v) override;
};

class InsertStatement : public Stm {
public:
    string table_name;
    vector<string> values;

    InsertStatement(string table, vector<string> vals)
        : table_name(table), values(vals) {}

    string accept(class ImpVisitor* v) override;
};

class SelectStatement : public Stm {
public:
    string table_name;
    string column;
    string value;
    bool range_query; // Para manejar BETWEEN
    string value2; // Para el segundo valor en BETWEEN

    SelectStatement(string table, string col, string val, bool range = false, string val2 = "")
        : table_name(table), column(col), value(val), range_query(range), value2(val2) {}

    string accept(class ImpVisitor* v) override;
};

class DeleteStatement : public Stm {
public:
    string table_name;
    string column;
    string value;

    DeleteStatement(string table, string col, string val)
        : table_name(table), column(col), value(val) {}

    string accept(class ImpVisitor* v) override;
};

// ------------------------------

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
    virtual ~Program() = default;
    void add(Stm* s);
    std::string accept(class ImpVisitor* v);

private:
    std::vector<Stm*> slist;
};

class ImpVisitor {
public:
    virtual std::string visit(CreateTableStatement* e) = 0;
    virtual std::string visit(InsertStatement* e) = 0;
    virtual std::string visit(SelectStatement* e) = 0;
    virtual std::string visit(DeleteStatement* e) = 0;
    virtual std::string visit(IdExp* e) = 0; // Añadir para IdExp
    virtual std::string visit(ParenthExp* e) = 0; // Añadir para ParenthExp
    virtual std::string visit(Program* p) = 0; // Añadir este método
};

class ConcreteVisitor : public ImpVisitor {
public:
    std::string visit(IdExp* e);
    std::string visit(ParenthExp* e);
    std::string visit(Program* p);
};


#endif // GRAMATICA_H
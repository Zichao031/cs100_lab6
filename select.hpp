#ifndef __SELECT_HPP__
#define __SELECT_HPP__

#include <cstring>

class Select
{
public:
    virtual ~Select() = default;

    // Return true if the specified row should be selected.
    virtual bool select(const Spreadsheet* sheet, int row) const = 0;
};

// A common type of criterion for selection is to perform a comparison based on
// the contents of one column.  This class contains contains the logic needed
// for dealing with columns. Note that this class is also an abstract base
// class, derived from Select.  It introduces a new select function (taking just
// a string) and implements the original interface in terms of this.  Derived
// classes need only implement the new select function.  You may choose to
// derive from Select or Select_Column at your convenience.
class Select_Column: public Select
{
protected:
    int column;
public:
    Select_Column(const Spreadsheet* sheet, const std::string& name)
    {
        column = sheet->get_column_by_name(name);
    }

    virtual bool select(const Spreadsheet* sheet, int row) const
    {
        return select(sheet->cell_data(row, column));
    }

    // Derived classes can instead implement this simpler interface.
    virtual bool select(const std::string& s) const = 0;
};


// To Do
// Select_Contains
class Select_Contains: public Select {
protected:
    int column;
    std::string s;
public:
    // set contains function
    Select_Contains (Spreadsheet* sheet, const std::string& name, const std::string& b) {
        column = sheet->get_column_by_name(name);
        s = b;
    }
    // check if contain
    virtual bool select(const Spreadsheet* sheet, int row) const {
        std::string Data = sheet->cell_data(row, column);
        if (Data.find(s) != std::string::npos)
            return true;
        else
            return false;
    }

};


//Select_Not
class Select_Not: public Select {
protected:
    Select* val;
public:
    // set Not function
    Select_Not (Select* value) {
        val = value;
    }

    //release memory
    ~Select_Not() {
        delete val;
    }


    virtual bool select(const Spreadsheet* sheet, int row) const {
        bool selected_val = val->select(sheet, row);
        if (selected_val)
            return false;
        else
            return true;
    }
};


//Select_And
class Select_And: public Select {
protected:
    Select* val1;
    Select* val2;
public:
    // set Select_And function
    Select_And(Select* value1, Select* value2) {
        val1 = value1;
        val2 = value2;
    }

    ~Select_And() {
        delete val1;
        delete val2;
    }
    virtual bool select(const Spreadsheet* sheet, int row) const {
        bool selected_val1 = val1->select(sheet, row);
        bool selected_val2 = val2->select(sheet, row);
        if (selected_val1 && selected_val2)
            return true;
        else
            return false;
    }
};


//Select_Or
class Select_Or: public Select {
protected:
    Select* val1;
    Select* val2;
public:
    Select_Or(Select* value1, Select* value2) {
        val1 = value1;
        val2 = value2;
    }
    ~Select_Or() {
        delete val1;
        delete val2;
    }
    virtual bool select(const Spreadsheet* sheet, int row) const {
        bool selected_val1 = val1->select(sheet, row);
        bool selected_val2 = val2->select(sheet, row);
        if (selected_val1 || selected_val2)
            return true;
        else
            return false;
    }
};

#endif //__SELECT_HPP__

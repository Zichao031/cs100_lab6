#include "spreadsheet.hpp"
#include "spreadsheet.cpp"
#include "select.hpp"
#include "gtest/gtest.h"
#include <sstream>
#include <iostream>
#include <string>

TEST(SelectTest, PrintAll1) {
  Spreadsheet sheet;
  sheet.set_column_names({"First", "Last", "Age", "Major"});
  sheet.add_row({"Amanda","Andrew","22","business"});
  sheet.add_row({"Brian","Becker","21","computer science"});
  std::stringstream ss;
  sheet.print_selection(ss);
  std::string s = ss.str();
  EXPECT_EQ("Amanda Andrew 22 business \nBrian Becker 21 computer science \n", s);
}


TEST(SelectTest, PrintContains1){
  Spreadsheet sheet;
  sheet.set_column_names({"First","Last","Age","Major"});
  sheet.add_row({"Amanda","Andrews","22","business"});
  sheet.add_row({"Brian","Becker","21","computer science"});
  sheet.add_row({"Carol","Conners","21","computer science"});
  sheet.add_row({"Joe","Jackson","21","mathematics"});
  sheet.add_row({"Sarah","Summers","21","computer science"});
  sheet.add_row({"Diane","Dole","20","computer engineering"});
  sheet.add_row({"David","Dole","22","electrical engineering"});
  sheet.add_row({"Dominick","Dole","22","communications"});
  sheet.add_row({"George","Genius","9","astrophysics"});
  std::stringstream ss;
  sheet.set_selection(new Select_Contains(&sheet, "Age","20"));
  sheet.print_selection(ss);
  std::string s = ss.str();
  EXPECT_EQ("Diane Dole 20 computer engineering \n", s);
}

TEST(SelectTest, PrintNotContains1) {
  Spreadsheet sheet;
  sheet.set_column_names({"First","Last","Age","Major"});
  sheet.add_row({"Amanda","Andrews","22","business"});
  sheet.add_row({"Diane","Dole","20","computer engineering"});
  std::stringstream ss;
  sheet.set_selection(new Select_Not(new  Select_Contains(&sheet,"Age","22")));
  sheet.print_selection(ss);
  std::string s = ss.str();
  EXPECT_EQ("Diane Dole 20 computer engineering \n", s);
}

TEST(SelectTest, PrintAnd1) {
  Spreadsheet sheet;
  sheet.set_column_names({"First","Last","Age","Major"});
  sheet.add_row({"Amanda","Andrews","22","business"});
  sheet.add_row({"Diane","Dole","20","computer engineering"});
  sheet.add_row({"Brian","Becker","21","computer science"});
  sheet.add_row({"Carol","Conners","21","computer science"});
  std::stringstream ss;
  sheet.set_selection(new Select_And(new  Select_Contains(&sheet, "Last", "B"), new  Select_Contains(&sheet, "First", "B")));
  sheet.print_selection(ss);
  std::string s = ss.str();
  EXPECT_EQ("Brian Becker 21 computer science \n", s);
}


TEST(SelectTest, PrintOr1) {
  Spreadsheet sheet;
  sheet.set_column_names({"First","Last","Age","Major"});
  sheet.add_row({"Amanda","Andrews","22","business"});
  sheet.add_row({"Diane","Dole","20","computer engineering"});
  sheet.add_row({"Brian","Becker","21","computer science"});
  sheet.add_row({"Carol","Conners","21","computer science"});
  std::stringstream ss;
  sheet.set_selection(new Select_Or(new  Select_Contains(&sheet, "Last", "B"), new  Select_Contains(&sheet, "First", "D")));
  sheet.print_selection(ss);
  std::string s = ss.str();
  EXPECT_EQ("Diane Dole 20 computer engineering \nBrian Becker 21 computer science \n", s);
}

TEST(SelectTest, EmptyValue) {
  Spreadsheet sheet;
  sheet.set_column_names({"Category", "Name"});
  sheet.add_row({"cat", "maomao"});
  sheet.add_row({"dog", "gougou"});
  sheet.set_selection(new Select_Contains(&sheet, "Category", ""));
  std::stringstream ss;
  sheet.print_selection(ss);
  std::string s = ss.str();
  EXPECT_EQ("cat maomao \ndog gougou \n", s);
}

TEST(SelectTest, TestCaseSensitive1) {
  Spreadsheet sheet;
  sheet.set_column_names({"Category", "Name"});
  sheet.add_row({"Cat", "A"});
  sheet.add_row({"dog", "b"});
  std::stringstream ss;
  sheet.set_selection(new Select_And(new Select_Contains(&sheet, "Category", "cat"), new Select_Contains(&sheet, "Category","dog")));
  sheet.print_selection(ss);
  std::string s = ss.str();
  EXPECT_EQ("", s);
}

TEST(SelectTest, TestCaseSensitive2) {
Spreadsheet sheet;
sheet.set_column_names({"Category", "Name"});
sheet.add_row({"Cat", "A"});
sheet.add_row({"dog", "b"});
std::stringstream ss;
sheet.set_selection(new Select_Or(new Select_Contains(&sheet, "Category", "cat"), new Select_Contains(&sheet, "Category","dog")));
sheet.print_selection(ss);
std::string s = ss.str();
EXPECT_EQ("dog b \n", s);
}

TEST(SelectTest, TestCaseSensitive3) {
Spreadsheet sheet;
sheet.set_column_names({"Category", "Name"});
sheet.add_row({"apple", "A"});
sheet.add_row({"snapple", "b"});
sheet.add_row({"Apple", "c"});
std::stringstream ss;
sheet.set_selection(new Select_Contains(&sheet, "Category", "apple"));
sheet.print_selection(ss);
std::string s = ss.str();
EXPECT_EQ("apple A \nsnapple b \n", s);
}

//TEST(SelectTest, TestNotExistColumn) {
//Spreadsheet sheet;
//sheet.set_column_names({"Category", "Name"});
//sheet.add_row({"apple", "A"});
//sheet.add_row({"snapple", "b"});
//sheet.add_row({"Apple", "c"});
//std::stringstream ss;
//sheet.set_selection(new Select_Contains(&sheet, "Categ", "apple"));
//sheet.print_selection(ss);
//std::string s = ss.str();
//EXPECT_EQ("", s);
//}

TEST(SelectTest, TestTwoSameColumn) {
Spreadsheet sheet;
sheet.set_column_names({"Category", "Name"});
sheet.add_row({"apple", "A"});
sheet.add_row({"apple", "A"});
sheet.add_row({"Apple", "c"});
std::stringstream ss;
sheet.set_selection(new Select_Contains(&sheet, "Categ", "apple"));
sheet.print_selection(ss);
std::string s = ss.str();
EXPECT_EQ("apple A \napple A \n", s);
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

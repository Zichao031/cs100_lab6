#include "spreadsheet.hpp"
#include "spreadsheet.cpp"
#include "select.hpp"
#include "gtest/gtest.h"
#include <sstream>
#include <iostream>
#include <string>

TEST(SelectTest, PrintAll) {
  Spreadsheet sheet;
  sheet.set_column_names({"First","Last","Age","Major"});
  sheet.add_row({"Amanda","Andrews","22","business"});
  std::stringstream ss;
  sheet.print_selection(ss);
  std::string s = ss.str();
  EXPECT_EQ("Amanda Andrews 22 business \n", s);
}

TEST(SelectTest, PrintContains) {
  Spreadsheet sheet;
  sheet.set_column_names({"First","Last","Age","Major"});
  sheet.add_row({"Amanda","Andrews","22","business"});
  sheet.add_row({"Diane","Dole","20","computer engineering"});
  std::stringstream ss;
  sheet.set_selection(new Select_Contains(&sheet,"Last","Andrews"));
  sheet.print_selection(ss);
  std::string s = ss.str();
  EXPECT_EQ("Amanda Andrews 22 business \n", s);
}

TEST(SelectTest, PrintNotContains) {
  Spreadsheet sheet;
  sheet.set_column_names({"First","Last","Age","Major"});
  sheet.add_row({"Amanda","Andrews","22","business"});
  sheet.add_row({"Diane","Dole","20","computer engineering"});
  std::stringstream ss;
  sheet.set_selection(new Select_Not(new  Select_Contains(&sheet,"Last","Dole")));
  sheet.print_selection(ss);
  std::string s = ss.str();
  EXPECT_EQ("Amanda Andrews 22 business \n", s);
}

TEST(SelectTest, PrintAnd) {
  Spreadsheet sheet;
  sheet.set_column_names({"First","Last","Age","Major"});
  sheet.add_row({"Amanda","Andrews","22","business"});
  sheet.add_row({"Diane","Dole","20","computer engineering"});
  std::stringstream ss;
  sheet.set_selection(new Select_And(new  Select_Contains(&sheet,"Last","A"),new  Select_Contains(&sheet,"First","A")));
  sheet.print_selection(ss);
  std::string s = ss.str();
  EXPECT_EQ("Amanda Andrews 22 business \n", s);
}

TEST(SelectTest, PrintOr) {
  Spreadsheet sheet;
  sheet.set_column_names({"First","Last","Age","Major"});
  sheet.add_row({"Amanda","Andrews","22","business"});
  sheet.add_row({"Diane","Dole","20","computer engineering"});
  std::stringstream ss;
  sheet.set_selection(new Select_Or(new  Select_Contains(&sheet,"Last","Dole"),new  Select_Contains(&sheet,"First","Amanda")));
  sheet.print_selection(ss);
  std::string s = ss.str();
  EXPECT_EQ("Amanda Andrews 22 business \nDiane Dole 20 computer engineering \n", s);
}





int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

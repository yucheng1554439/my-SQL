#include "gtest/gtest.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <set>
#include <vector>

//------------------------------------------------------------------------------------------
//Files we are testing:
#include "../../includes/sql/sql.h"

const vector<string> command_list = {

// /*00*/     "make table employee fields  last,       first,         dep,      salary, year",
// /*01*/     "insert into employee values Blow,       Joe,           CS,       100000, 2018",
// /*02*/     "insert into employee values Blow,       JoAnn,         Physics,  200000, 2016",
// /*03*/     "insert into employee values Johnson,    Jack,          HR,       150000, 2014",
// /*04*/     "insert into employee values Johnson,    \"Jimmy\",     Chemistry,140000, 2018",

/*05*/     "make table student fields  fname,          lname,    major,    age",
/*06*/     "insert into student values Flo,            Yao, Art, 20",
/*07*/     "insert into student values Bo,      Yang, CS, 28",
/*08*/     "insert into student values \"Sammuel L.\", Jackson, CS, 40",
/*09*/     "insert into student values \"Billy\",     Jackson, Math,27",
/*10*/     "insert into student values \"Mary Ann\",   Davis,Math,30",
/*10*/     "insert into student values \"Mary Ann\",   Davi,Math,30",
/*10*/     "insert into student values \"Mary Ann\",   Da,Math,30",
            "insert into student values Johnson, \"Mary Ann\", \"Biology\"",
// /*11*/     "select * from employee",
// /*12*/     "select last, first, age from employee",
// /*13*/     "select last from employee",
// /*14*/     "select first from employee where year <= 2016 and dep = Physics and first = JoAnn and last = Blow and salary > 20000",
// /*15*/     "select * from employee where last=Blow and major=\"JoAnn\"",

// /*16*/     "select * from student",
// /*17*/     "select * from student where (major=CS or major=Art)",
/*18*/     "select * from student where (fname = \"Sammuel L.\" and fname = Billy) or (major = CS and major = Math) or age = 27",
// /*19*/     "select * from student where lname>J and (major=CS or major=Art)"
// /*20*/     "select * from student where age <= 20"

};

const int MAKE_TABLE_COMMANDS = 11;
const int SELECT_COMMANDS = 20;

bool test_stub(bool debug = false)
{
  if (debug){

    SQL sql;
     Table t;
     cout << ">" << command_list[0] << endl;
     sql.command(command_list[0]);
     cout << "basic_test: table created." << endl<<endl;

     for (int i = 0; i < command_list.size(); i++)
     {
          cout << ">" << command_list[i] << endl;
          cout<< "here is yo table \n" << sql.command(command_list[i]);
          cout << "basic_test: records selected: "<<sql.select_recnos() << endl;

     }


  }
  return true;
}

bool test_steps(bool debug = false)
{
  if (debug){

      // vector<long>v1 = {5,3,1};
      // vector<long>v2 = {6,4,2,1, 0};
      // Resultset r1;
      // string s = "or";
      // r1 = Resultset(v1, v2, s);
      // cout<< "result : " << r1.get_resultset_vtrlong();

     }
  return true;
}

TEST(TEST_STUB, TestStub) {
  
  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_stub(true));
}

TEST(TEST_STEPS, TestSteps) {
  
  //EXPECT_EQ(0, <your individual test functions are called here>);

  EXPECT_EQ(1, test_steps(false));
}

int main(int argc, char **argv) {
//   if (argc > 1)
//   {
//     debug = !strcmp(argv[1], "debug");
//   }

  ::testing::InitGoogleTest(&argc, argv);
  std::cout<<"\n\n----------running basic_test.cpp---------\n\n"<<std::endl;
  return RUN_ALL_TESTS();
}
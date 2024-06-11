#include <iostream>
#include <iomanip>
#include "includes/sql/sql.h"

using namespace std;

int main(int argv, char **argc)
{
    cout << "\n\n"
         << endl;

    SQL database;
    cout << "Welcome to SQL Database" << endl
         << endl;
    bool onPlay = true;

    string command;
    // cout << database.command(command) << endl;

    /*
    command = "select * from footballers where lname";
    // cout << database.command(command) << endl;

    try
    {
        cout << database.command(command) << endl;
    }
    catch (const std::exception &e)
    {
        cout << "SQL command not recognized" << endl
             << endl;
    }*/

        while (true)
    {
        try
        {
            cout << "Insert an SQL command: " << endl;
            cout << ">> ";
            getline(cin, command);

            if (command == "")
                continue;

            if (command == "exit")
                break;

            cout << database.command(command) << endl;
        }
        catch (const std::exception &e)
        {
            cout << "SQL command not recognized" << '\n\n';
        }
    }

    cout << "\n\nThank you, come again" << endl;

    cout << "\n\n\n=====================" << endl;
    return 0;
}
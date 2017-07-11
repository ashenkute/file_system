//
// filesystemClient.cpp
// Alazar Shenkute
// 05/01/14
// client code for FILE_SYSTEM class
// a-Unix like file system
// supports cammands -- mkdir(), mkfile(), rmdir(), rmfile(), chdir(), pwd(), ls(), lsr()
#include "filesystem.h"
#include <iostream>
#include <fstream>
#include <string>
#define SUCCESS 1
#define FAILURE 0
using namespace std;
int main( int argc, char **argv )
{
		/*if( argc < 3 ){
			cout << " Invalid number of arguments ";
			return 0;
		}
		ifstream in( argv[1], ios::in );
		ofstream out( argv[2], ios::out );*/

		FILE_SYSTEM slash;
		fstream in( "input2.txt", ios::in );
		fstream out( "output2bn.txt", ios::out );
		char str[82];
		const char *p;	// for c.str(),
		string str2;
		string commands[] = {"mkdir", "mkfile", "rmdir", "rmfile", "chdir", "pwd", "ls", "lsr", "Q"};
		if( !in || !out )
		{
			cout << " File(s) did not open successfully";
			return 0;
		}
		char *ptr;
		int i = 0; // for commands[i]
		string command, name;
		getline( in, str2 );	// get a line
		while( !in.eof() )	
		{
			p = str2.c_str();		// convert to C-string so we can use strtok()
			strcpy( str, p );		// make a copy just to be safe
			string command[2];		// command[0] is the command and command[1] is name, if any
			out << str << "\t";
			ptr = strtok( str, " " );
			while( ptr )	// until strtok() return NULL
			{
				string aCPPstring( ptr );	// convert to a string object
				command[i] = ptr;// save it
				i++;
				ptr = strtok( NULL, " " );
			}
			i = 0;
			int j = 0;
			if( command[0] == commands[j++] )
			{
				slash.mkdir( command[1], out );
			}
			else if( command[0] == commands[j++])
			{
				slash.mkfile( command[1], out );
			}
			else if( command[0] == commands[j++])
			{
				slash.rmdir( command[1], out );
			}
			else if( command[0] == commands[j++])
			{
				slash.rmfile( command[1], out );
			}
			else if( command[0] == commands[j++])
			{
				slash.chdir( command[1], out );
			}else if( command[0] == commands[j++])
			{
				slash.pwd( out );
				out << "\n";
			}
			else if( command[0] == commands[j++])
			{
				slash.ls( out );
			}else if( command[0] == commands[j++])
			{
				slash.lsr( out );
			}else
			{
				out << "command not found " << endl;
			}
			getline( in, str2 );
		}
		slash.chdir("/", out );
		slash.pwd( out );
		slash.lsr( out );
		in.close();
		out.close();
		return 0;
}


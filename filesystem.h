//
// filesystem.h
// Alazar Shenkute
// 05/01/14
// header file for FILE_SYSTEM class
//
#ifndef FILESYSTEM_H_
#define FILESYSTEM_H_
#include<iomanip>
#include <iostream>
#include <string>
using namespace std;
// TREE_NODE class
class TREE_NODE
{
public:
	char type;		// type of node - directory or file
	string name;	// name of file/directory
	TREE_NODE *next;
	TREE_NODE *parent;
	TREE_NODE *children;
	 // NULL all pointers pointers
	TREE_NODE( char t, string n ) : type(t), name(n), next(NULL), parent(NULL), children(NULL) {}
};
// FILE_SYSTEM class
class FILE_SYSTEM
{
private:
	TREE_NODE *root;	// root of the tree
    TREE_NODE *cwd;	// current working directory
	void rmdirHelper( TREE_NODE * );	// helper for rmdir()
	TREE_NODE* findNode( TREE_NODE *, string, TREE_NODE *prev, TREE_NODE *next );
	void helpLsr( TREE_NODE *, ostream &out, int indent );
	void helpLs( TREE_NODE*, ostream& );
	int helpMakeNode( string, ostream&, char );
	void helpPwd( TREE_NODE *, ostream& );

public:
	FILE_SYSTEM();
	~FILE_SYSTEM();
	void pwd( ostream& );
	void mkdir( string , ostream& );
	void rmdir( string, ostream&  );
	void rmfile( string, ostream& );
	void chdir( string, ostream& );
	void mkfile( string, ostream& );
	void ls( ostream& );
	void lsr( ostream& );
};
#endif


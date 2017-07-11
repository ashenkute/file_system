//
// filesystem.cpp
// Alazar Shenkute
// 05/01/14
// 
// implementation file for FILE_SYSTEM class
// 
#include "filesystem.h"
#include<iomanip>
#include <iostream>
#include <fstream>
#include <string>
#define SUCCESS 1
#define FAILURE 0
using namespace std;
// constructor creates a directory node with the name "/"
FILE_SYSTEM::FILE_SYSTEM()
{
	root = new TREE_NODE( 'D', "/" );
	root->parent = root;
	cwd = root;	// initially cwd is the root, TREE_NODE() sets everything else to NULL
}
FILE_SYSTEM::~FILE_SYSTEM()
{
	rmdirHelper( root );
}
void FILE_SYSTEM::pwd( ostream &out )
{
	helpPwd( cwd, out );
}
// helper for pwd, prints the path from the cwd all the way to the root;
void FILE_SYSTEM::helpPwd( TREE_NODE *p, ostream &out )
{
	out << setw(10);
	if( p != NULL && p == root ){
		out << "/";
		return;
	}
	helpPwd( p->parent , out );
	out << p->name << "/";
}

// takes a pointer to the cwd and returns a pointer to node if match found
TREE_NODE *FILE_SYSTEM::findNode( TREE_NODE *p, string name, TREE_NODE *prev, TREE_NODE *next )
{
	TREE_NODE *q;
	if( name == ".." )
	{
		return cwd->parent;
	}
	if( p!=NULL && p->name == name ){
		return p;
	}
	if( p->children != NULL )
	{
		while( p->children!=NULL && p->children->name!= name ){
			q = p;
			p = p->children;
		}

		if( p->children!=NULL && p->children->name == name ){
			prev = q != NULL ? q->children : NULL ;
			next = p->next;
			return p->children;
		}
	}
	if( p->next != NULL )
	{
		while( p->next!=NULL && p->next->name!= name ){
				q = p;
				p = p->next;
			}
	}
	prev = q;
	next = p->next;
	return p == NULL ? NULL : p->next;
}
// interface to user
// mkdir passes  2 argument and a 'D' to helpMakeNode()
void FILE_SYSTEM::mkdir( string name, ostream &out )
{
	if( helpMakeNode( name, out, 'D' ) == SUCCESS ){
		out << "\t" << name << " -- Directory successfully made." << endl;
	}else{
		out << "\t" << name << " -- Name already exist in File System" << endl;
	}
}
// interface to user
// mkfile passes 2 arguments and a 'F' to helpMakeNode()
void FILE_SYSTEM::mkfile( string name, ostream &out )
{
	if( helpMakeNode( name, out, 'F') == SUCCESS ){
		out << "\t" << name << " -- File successfully made " << endl;
	}else{
		out <<  "\t" << name << " -- Name already exist in File System" << endl;
	}
}
// helper for mkfile() and mkdir()
int FILE_SYSTEM::helpMakeNode( string name, ostream &out, char type )
{
	TREE_NODE *p = cwd, *ptr = NULL, *q = NULL, *e = NULL;
	// check if name already exist in cwd
	ptr = findNode( p, name, NULL, NULL );
	if( ptr == NULL )	// if name doesn't exist in cwd
	{
		e = new TREE_NODE( type, name );	// allocate a node
		if( p->children == NULL ){	// if the directory is a leaf node
			p->children = e;
		}
		else
		{
			for( p = p->children; p!=NULL && p->type < e->type; q = p, p = p->next );
			if( q == NULL ){	// at the front
				e->next = p;
				cwd->children = e;

			}
			else if( p == NULL ){	// at the end of the list
				q->next = e;
			}
			else
			{
				q->next = e;
				e->next = p;
			}
		}
		if( type == 'D' ){	// if its a directory, then it has a thread pointer
			e->parent = cwd;
		}
		return SUCCESS;
	}
	else{
		return FAILURE;
	}
}
// private method
// helper for rmdir(), takes a parent and deletes all children nodes
void FILE_SYSTEM::rmdirHelper( TREE_NODE *p )
{
	if( p!= NULL )
	{
		if( p->next!= NULL ){
			return rmdirHelper( p->next );
		}
		if( p->children!= NULL ){
			return rmdirHelper( p->children );
		}
		delete p;
	}
}
void FILE_SYSTEM::rmdir( string name, ostream &out )
{
	TREE_NODE *prev, *next;
	TREE_NODE *s = findNode( root, name, prev, next );
	// can't delete cwd, cwd->parent and root
	if( s == NULL || s == cwd || s == cwd->parent || s == root || s->type == 'F' )
	{
		out << "\t" << name << " doesn't exist in File System or can't delete cwd, root or cwd's parent." << endl;
	}
	else
	{
		if( prev == NULL && next == NULL )	// removing a leaf node ?
		{
			s->parent->children = NULL;
		}
		else if( prev!=NULL && next != NULL )	// middle node ?
		{
			prev->next = next;
		}
		else if( s->parent->children == s && s->next != NULL )
		{
			s->parent->children = s->next;
		}
		else
		{
			prev->next = NULL;
		}
		s->children != NULL ? rmdirHelper( s ) : delete s;
		out << "\t" << name << " removed from cwd." << endl;
		s->parent = NULL;
	}
	return;
}
void FILE_SYSTEM::rmfile( string name, ostream &out )
{
	TREE_NODE *prev, *next;
	TREE_NODE *s = findNode( root, name, prev, next );
	// can't delete cwd, cwd->parent and root
	if( s == NULL || s == cwd || s == cwd->parent || s == root || s->type == 'D' )
	{
		out << "\t" << name << " doesn't exist in File System or can't delete cwd, root or cwd's parent." << endl;
	}
	else
	{
		if( prev == NULL && next == NULL )	// removing a leaf node ?
		{
			s->parent->children = NULL;
		}
		else if( prev!=NULL && next != NULL )	// middle node ?
		{
			prev->next = next;
		}
		else if( s->parent->children == s && s->next != NULL )
		{
			s->parent->children = s->next;
		}
		else
		{
			prev->next = NULL;
		}
		delete s;
		out << "\t" << name << " removed from cwd." << endl;
	}
	return;
}
void FILE_SYSTEM::chdir( string name, ostream &out )
{
	TREE_NODE *tempcwd = cwd;
	char str[82], *ptr;
	const char *p;
	if( name == "/" ){
		cwd = root;
		out << "\tCurrent working directory changed to " << cwd->name << "\n";
		return;
	}
	if( name == ".." ){
		cwd = cwd->parent;
		out << "\tCurrent working directory changed to " << cwd->name << "\n";
		return;
		}

	else
	{	// convert to a C-string and use strtok() to tokenize
		p = name.c_str();	// convert to a C-string
		strcpy( str, p );	// make a copy
		TREE_NODE *s;
		if( str[0] == '/' ){
			cwd = root;
		}
		ptr = strtok( str, "/" );
		while( ptr )
		{
			string aCPPstring( ptr );	// convert to a string object
			s = findNode( root, ptr , NULL, NULL);	// find node in FILE_SYSTEM
			// if node exist && it is a directory, move tempcwd

			if( s!= NULL && s->type!= 'F'  )
			{
				cwd = s;
			}
			else
			{
				out << "\tNo such directory " << name << "\n";
				cwd = tempcwd;
				return;
			}
			ptr = strtok( NULL, "/" );
		}
		//cwd = tempcwd;
		out << "\tCurrent working directory changed to ";
		this->pwd( out );
		out << "\n";
	}
}
// lsr takes an ostream object and displays all files in this directory and all sub-directories
void FILE_SYSTEM::lsr( ostream &out )
{
	int indent = 5;
	helpLsr( cwd, out, indent );
}
void FILE_SYSTEM::helpLsr( TREE_NODE *p, ostream &out, int indent )
{
	if( p!=NULL && p->type == 'F' ){	// if its a file just display it

		out << "\t" <<setw(indent) << p->name << endl;
	}else if( p!=NULL )
	{
		out << "\t" << setw(indent) << p->name << endl;
	}
	if( p->next!=NULL ){
		helpLsr( p->next, out, ( ++indent ) );
	}
	if( p->children!=NULL ){
		helpLsr( p->children, out, ( ++indent) );
	}
}
void FILE_SYSTEM::ls( ostream &out )
{
	helpLs( cwd->children, out );
}
void FILE_SYSTEM::helpLs( TREE_NODE *p, ostream &out )
{
	if( p!=NULL && p->type == 'F' ){
		out << setw(20) << p->name << endl;
	}
	else if( p!=NULL )
	{
		out << setw(23) << p->name << endl;
	}
	if( p->next!=NULL )
	{
		helpLs( p->next, out );
	}
}


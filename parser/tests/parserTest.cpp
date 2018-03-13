
#include <string>
#include <cassert>
#include <iostream>
#include "../Parser.h"
#include "../../shared/Document.h"
#include "../../shared/ProducerConsumerQueue.h"

using namespace std;

void testSimple ( );
void testComplex ( );

int main ( )
	{
	cout << "Testing Parser ... " << endl << endl;
	testSimple ();
	testComplex ();
	cout << "Parser Tests Passed! :D" << endl;

	}

void testSimple ( )
	{

	ProducerConsumerQueue < string >  urlFrontierTest;
	ParsedUrl url = ParsedUrl("testurl.com");
	char docString[10240];
	strcpy(docString, "<title>This Cat Title Cat</title>");
	Document document ( url, docString);

	Parser parser ( &urlFrontierTest );
	auto dictionary = parser.execute ( &document );

	assert ( dictionary != nullptr );
	assert ( dictionary->size () == 2);
	assert ( dictionary->find ( "cat" ) != dictionary->end () );
	assert ( dictionary->find ( "title" ) != dictionary->end () );
	assert ( dictionary->find ( "this" ) == dictionary->end () );
	assert ( dictionary->at ( "cat" )[ 0 ] == 0 && dictionary->at ( "cat" )[ 1 ] == 2 );
	assert ( dictionary->at ( "title" )[ 0 ] == 1 );

	delete dictionary;

	}
void testComplex ( )
	{

	ProducerConsumerQueue < string >  urlFrontierTest;
	ifstream file("../tests/cats.html");
	string temp;
	string docString = "<title>Joe the Cat</title>\n";
	docString += "<a href=\"https://www.w3schools.com/html/\">Visit our HTML tutorial</a>\n";
	while ( std::getline ( file, temp ) )
		{
		docString += temp;
		}

	ParsedUrl url = ParsedUrl("https://www.w3schools.com/tests/cats.html");
	char * writable = new char[docString.size( ) + 1];
	std::copy(docString.begin( ), docString.end( ), writable);
	writable[ docString.size( ) ] = '\0';

	Document document ( url, writable );

	Parser parser ( &urlFrontierTest );
	auto dictionary = parser.execute ( &document );

	assert ( dictionary != nullptr );
	assert ( dictionary->size () == 3);

	assert ( dictionary->find ( "cat" ) != dictionary->end () );
	assert ( dictionary->find ( "story" ) != dictionary->end () );
	assert ( dictionary->find ( "joe" ) != dictionary->end () );

	assert ( dictionary->find ( "the" ) == dictionary->end () );
	assert ( dictionary->find ( "of" ) == dictionary->end () );

	delete dictionary;
	delete[] writable;

	}
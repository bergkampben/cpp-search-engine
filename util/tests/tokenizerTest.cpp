//
// Created by Veronica Day on 2/13/18.
//

#include <string>
#include <vector>
#include "../Tokenizer.h"
#include <iostream>
#include <cassert>

using namespace std;

void testExecute ( string original );


int main ( )
	{

	cout << "Beginning testing for TokenizerTest_unit" << endl << endl;

	string original = "It is a long established fact that a reader will be distracted by the readable content of a page when looking at its layout. "
			"The point of using Lorem Ipsum is that it has a more-or-less normal distribution of letters, as opposed to using 'Content here, content here',"
			"making it look like readable English. ";

	testExecute ( original );

	cout << "\nTests passed for TokenizerTest_unit :D" << endl;

	}

void testExecute ( string original )
	{
	Tokenizer myTokenizer;
	myTokenizer.execute ( original );

	auto dict = myTokenizer.get ( );

	for ( auto it = dict->begin ( ); it != dict->end ( ); it++ )
		{
		cout << it->first << ':';
		for ( int i = 0; i < it->second.size ( ); ++i )
			{
			cout << it->second[ i ] << " ";
			}
		cout << std::endl;
		}

	}

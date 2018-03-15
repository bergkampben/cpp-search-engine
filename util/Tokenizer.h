//
// Created by anvia on 1/31/2018.
//
#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include "stringProcessing.h"

using namespace std;

struct wordData {
	int offset;
	int frequency = 0;
	//total num words/unique words??
};
class Tokenizer
	{
public:
	Tokenizer ( )
		{
		docIndex = new unordered_map< string, vector<wordData>>;
		}

	unordered_map< string, vector< int>> *get ( ) const
		{
		return docIndex;
		}
	//add type of word parameter, ie paragraph, url etc
	void execute ( string originalText, int offset )
		{

		vector< string > splitText = splitStr ( originalText, ' ' );
		string lowerString = "";
		int vectorLength = 0;
		for ( int i = 0; i < splitText.size ( ); ++i )
			{
			lowerString = toLower ( splitText[ i ] );
			if ( !isStopWord ( lowerString ) )
				{
				wordData currentWord;
				currentWord.offset = offset;
				vectorLength = ( *docIndex )[ lowerString ].size( );
				( *docIndex )[ lowerString ].push_back ( currentWord );
				( *docIndex )[ lowerString ][ vectorLength - 1 ].frequency += 1;
				++offset;
				}
			}
		}

private:
	unordered_map< string, vector<wordData>> *docIndex;
	};

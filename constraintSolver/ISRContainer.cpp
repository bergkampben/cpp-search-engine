//
// Created by Jake Close on 3/13/18.
//

#include "ISRContainer.h"


ISRContainer::ISRContainer( Tuple * top )  : root( top )
	{

	compile( );
	}



void ISRContainer::compile( )
	{


	Contained = recurviseCompile( root );


	}

ISR * ISRContainer::recurviseCompile( Tuple * root )
	{
	vector< ISR * > words;
	if( root->Type == WordTupleType )
		{

		string currentTerm;
		terms.push_back( currentTerm );
		return new ISRWord( currentTerm );
		}

	else
		{
		for( auto child : root->Next )
			words.push_back( recurviseCompile( child ) );
		}

		if( root->Type == AndTupleType )
			return  new ISRAnd ( words );
		else
			return  new ISROr ( words );

	}

void ISRContainer::Solve( )
	{
	while(Contained->GetCurrentLocation() != MAX_Location)
		{
		auto url = Contained->GetEndDocument()->getCurrentDoc().url;
		cout << url << endl;
		Location BeginningfDocument = Contained->GetISRToBeginningOfDocument( );
		PassToRanker( BeginningfDocument );

		Contained->NextDocument( );



/*
 * beg = GetBeginning of Doc
 * Pass Terms to ranker
 *
 * vector<words>
 *
 * Ranker:
 * for term in terms
 * IsrWord word = new ISR(term)
 * Term.seek(beg)
 * words.push(word)
 * rank(words)
 *
 * NextDocument()
 */
		}


	}

void ISRContainer::PassToRanker( Location docBeginning )
	{

	vector<ISRWord* > toRanker;
	for ( auto term : Terms )
		{

		ISRWord * isrWord = new ISRWord ( term ) ;
		isrWord->Seek( docBeginning );
		toRanker.push_back( isrWord );

		}

	ranker.rank ( toRanker )
	}


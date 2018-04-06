//
// Created by Jake Close on 3/13/18.
//

#include "ISROr.h"


ISROr::ISROr ( vector<ISR * > InputTerms ) : Terms( InputTerms )
	{

	for (auto currentTerm : InputTerms)
		{
		currentTerm->First();
		Location currentLocation = currentTerm->currentLocation;
		if (currentLocation < nearestStartLocation) {
			nearestTerm = currentTerm;
			nearestStartLocation = currentLocation;

		}
		if (currentLocation > nearestEndLocation) {
			nearestEndLocation = currentLocation;
		}
		++NumberOfTerms;
		currentTerm++;

		}
	}


Location ISROr::GetStartLocation ( )
	{
	return nearestStartLocation;
	}

Location ISROr::GetCurrentLocation(){
	return nearestStartLocation;
	}


Location ISROr::GetEndLocation ( )
	{
	return nearestEndLocation;
	}


Location ISROr::First()
	{
	//Fixme
	Location x;
	return x;
	}

/*
Returns the location of the next document that is a match
*/
Location ISROr::Next ( )
	{
	Location nearestEnd = this->nearestTerm->GetEndDocument( );

	for(auto Term : Terms)
	{
		Location newSeekLocation = Term->Seek( nearestEnd + 1 );
		if ( newSeekLocation < nearestStartLocation )
		{
			nearestStartLocation = newSeekLocation;
			nearestTerm = Term;
		}
	}

	return this->nearestTerm->currentLocation;

	}

Location ISROr::NextDocument()
	{
	//Fixme
	Location x;
	return x;
	}


Location ISROr::Seek ( Location target )
	{

	// Seek all the ISRs to the first occurrence beginning at// the target location. Return null if there is no match.
	// The document is the document containing the nearest term.
	//seek past target locations,
	//seek all terms in or past starting location, take the ones that nears
	//the document that the nearest term is in is the document ur in
	//updates private members




	return 1;

	}


Location ISROr::GetEndDocument()
	{
	//Fixme
	Location x;
	return x;
	}

/*
ISR *ISROr::GetCurrentEndDoc ( )
	{

	return this->nearestTerm->GetDocumentISR( );

	}
*/


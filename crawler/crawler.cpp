//
// Created by Ben Bergkamp on 1/31/18.
//

#include "crawler.h"
#include <unordered_map>

void Crawler::SpawnSpiders( size_t num_spiders, unordered_map < string, int > *docMapLookup )
	{
	for ( size_t i = 0; i < num_spiders; i++ )
		{
		Spider *temp = new Spider( this->mode, this->urlFrontier, docMapLookup );
		temp->StartThread( );
		this->spiders.push_back( temp );
		}

	}

void Crawler::WaitOnAllSpiders()
	{
	for ( Spider *spider : spiders )
		{
		spider->WaitForFinish( );
		}
	}

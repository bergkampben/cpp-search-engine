//
// Created by Ben Bergkamp on 1/31/18.
//

#include "crawler.h"

void Crawler::SpawnSpiders( size_t num_spiders, unordered_map < string, int > *docMapLookup , unordered_map < size_t, int > *duplicateUrlMap  )
	{
	for ( size_t i = 0; i < num_spiders; i++ )
		{
		Spider *temp = new Spider( this->mode, this->urlFrontier, docMapLookup, duplicateUrlMap );
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

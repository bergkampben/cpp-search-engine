//
// Created by Jake Close on 2/8/18.
//

#pragma once

#include "url.h"
#include <string>
#include <vector>
#include <pthread.h>
#include "../util/util.h"

using namespace std;

namespace filepath
	{
		const char *DOC_MAP = "/docMap.txt";
	}


pthread_mutex_t docMap_mutex = PTHREAD_MUTEX_INITIALIZER;

class Document
	{
private:
	ParsedUrl url;
	char *docString;
	long docID;
	bool lastCrawlStatus;
	int lastCrawlDate;
	int lastCrawlWordCount;

	//add more info fields here

public:

	Document( ParsedUrl url_in ) : url((url_in)), docString( nullptr )
		{ }

	Document( ParsedUrl url_in, char *docStringIn ) : url((url_in)), docString( docStringIn )
		{ }

	string DocToString();

	int WriteToDocMap();


	static void PrintDocMap( string url, int location );
	};
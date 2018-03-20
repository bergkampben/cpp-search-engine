#pragma once


#include<string>
#include <pthread.h>
#include <fstream>
#include "../shared/ProducerConsumerQueue.h"
#include "../shared/ThreadClass.h"
#include<iostream>
#include <unordered_map>
#include "StreamReader.h"
#include "../util/util.h"
#include "../parser/Parser.h"




using namespace std;


class Spider : public ThreadClass
	{

public:

	Spider( string mode_in, ProducerConsumerQueue < ParsedUrl > *url_q_in,
			  unordered_map < string, int > *doc_map_lookup_in, unordered_map < size_t, int > *duplicate_url_map_in )
			: mode( mode_in ), urlFrontier( url_q_in ), docMapLookup( doc_map_lookup_in ), parser( url_q_in), duplicateUrlMap(duplicate_url_map_in)
		{
		};


	//Takes a url off of the url frontier
	ParsedUrl getUrl();

	virtual void FuncToRun();

	//Makes request to given url
	// if successful, writes file to disk, stores location to memeber value
	// else return false and error information, retry if necessary
	StreamReader *request( ParsedUrl url );

	bool writeDocToDisk(ParsedUrl url);

	bool shouldURLbeCrawled( ParsedUrl URL );
	size_t hash(const char * s);
	int getRobots(ParsedUrl url );
	bool checkRobots(ParsedUrl url);


private:

	int locationOnDisk;
	ProducerConsumerQueue < ParsedUrl > *urlFrontier;
	unordered_map < size_t, int > *duplicateUrlMap;
	string mode;
	unordered_map < string, int > *docMapLookup;
	Parser parser;

	};
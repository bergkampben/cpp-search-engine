//
// Created by Ben Bergkamp on 4/16/18.
//

#include "ParsedUrl.h"

ParsedUrl::ParsedUrl() {}

ParsedUrl::ParsedUrl ( string input_url )
	{
// Assumes url points to static text but
// does not check.
	try
	{

		char *temp_CompleteUrl,
				*temp_Service,
				*temp_Host,
				*temp_Domain,
				*temp_Path,
				*temp_AnchorText,
				*temp_pathBuffer;

//intialize anchor text to ""
		char *null = new char[2];
		strcpy( null, string( "" ).c_str( ) );
		temp_AnchorText = null;

		char *url = new char[input_url.length( ) + 1];
		strcpy( url, input_url.c_str( ) );

		temp_CompleteUrl = url;

		temp_pathBuffer = new char[strlen( url ) + 1];
		char *f, *t;
		for ( t = temp_pathBuffer, f = url; ( *t++ = *f++ ); );

		temp_Service = temp_pathBuffer;


		const char Colon = ':', Slash = '/', HashTag = '#', Period = '.', QuestionMark = '?';
		char *p;
		for ( p = temp_pathBuffer; *p && *p != Colon; p++ );

		if ( *p )
		{
// Mark the end of the Service.
			*p++ = 0;

			if ( *p == Slash )
				p++;
			if ( *p == Slash )
				p++;

			temp_Host = p;

			for ( ; *p && *p != Slash; p++ );

			if ( *p )
// Mark the end of the Host.
				*p++ = 0;

//char * domainBuffer = new char[ 20 ];
//get the domain:
			char *i = temp_Host;
			temp_Domain = nullptr;
			if(i)
			{
				for ( ; *i; i++ )
				{
					if ( *i == Period )
						temp_Domain = i;
				}
			}

// Whatever remains is the Path. // need to remove fragments

			temp_Path = p;
			for ( ; *p && *p != QuestionMark; p++ );

			for ( ; *p && *p != HashTag; p++ );




			if ( *p )
// Mark the end of the Path, remove fragments.
				*p++ = 0;
		}
		else
			temp_Host = temp_Path = p;


		CompleteUrl = string(temp_CompleteUrl, strlen(temp_CompleteUrl));
//remove question marks


		CompleteUrl.erase(remove_if(CompleteUrl.begin(),
									CompleteUrl.end(),
									[](unsigned char x){return isspace(x);}),
						  CompleteUrl.end());

		CompleteUrl = util::removeAllStr(CompleteUrl, "?");
		CompleteUrl = util::removeAllStr(CompleteUrl, "#");





//size_t newLine = CompleteUrl.find("\\n");
//remove newline chars and white space










		Service = string(temp_Service, strlen(temp_Service));
		Host = string(temp_Host, strlen(temp_Host));

		if(Service == "http" || Service == "https")
		{
			if(  temp_Domain != nullptr )
				Domain = string(temp_Domain, strlen(temp_Domain));

			Path = string(temp_Path, strlen(temp_Path));
			Path = util::removeAllStr(Path, "?");

//vector<string> noquestionmakr =	splitStr(Path, '?', false);
			AnchorText = string(temp_AnchorText, strlen(temp_AnchorText));
			pathBuffer = temp_pathBuffer;

			setScore( );
		}
		else
			isValid = false;



	}
	catch (exception e)
	{
		cerr << "Error constructing a ParsedUrl from string url "<< endl;


	}

	}

void ParsedUrl::printUrl ( )
	{
	cout << "Complete URL: " << CompleteUrl << endl;
	cout << "Service: " << Service << endl;
	cout << "Host: " << Host << endl;
	cout << "Domain: " << Domain << endl;
	cout << "Path: " << Path << endl;
	cout << "Score: " << Score << endl;


	}

void ParsedUrl::setScore()
	{
	double lengthOfUrl = CompleteUrl.length();
	if(lengthOfUrl > 250)
		isValid = false;


	Score +=  lengthOfUrl/ 100;

	if(lengthOfUrl > 4)
	{

		if(this->Domain.length() )

		{
			if ( Domain == ORG )
				Score += .5;
			else if ( Domain == EDU  )
				Score += .5;
			else if ( Domain ==  GOV )
				Score += .75;
			else if ( Domain ==  COM )
				Score += .5;
			else if ( Domain ==  NET )
				Score += 3;
			else if ( Domain ==  INT )
				Score += 4;
			else if ( Domain ==  MIL )
				Score += 5;
			else
				Score += 10;
		}

	}
	}

string ParsedUrl::getDomain ( )
	{
	return Domain;
	}

string ParsedUrl::getService ( )
	{
	return Service;
	}

string ParsedUrl::getCompleteUrl ( )
	{
	return CompleteUrl;
	}

string ParsedUrl::getHost ( )
	{
	return Host;
	}

string ParsedUrl::getPath ( )
	{
	return Path;
	}

double ParsedUrl::getScore ( )
	{
	return Score;
	}

void ParsedUrl::updateScore( double time )
	{

	Score +=   time;
	}

string ParsedUrl::getAnchorText ( )
	{
	return AnchorText;
	}

void ParsedUrl::setAnchorText ( std::string anchorText )
	{
	AnchorText = anchorText;
	}

ParsedUrl::~ParsedUrl ( )
	{
	pathBuffer = 0;
	delete[] pathBuffer;
	}
//
// Created by Ben Bergkamp on 3/13/18.
//

#include "LocalReader.h"


bool LocalReader::request()
	{
	//FIXME
	//open the file?
	return true;
	}

bool LocalReader::fillBuffer( char *buf, size_t buf_size )
	{

	//FIXME
	strcpy( buf, util::getFileMap( fileName ));
	return true;

	}

string LocalReader::PageToString()
	{
	//FIXME
	string s( "fix me" );
	return s;
	}

ParsedUrl LocalReader::getUrl()
	{
	//FIXME
	ParsedUrl url( "" );
	return url;
	}


bool LocalReader::checkStatus()
	{
	return true;
	}

void LocalReader::closeReader()
	{
	//FIXME
	//close the file?
	}
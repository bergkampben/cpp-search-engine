//
// Created by Ben Bergkamp on 3/13/18.
//

#include "LocalReader.h"


bool LocalReader::request ( )
	{
	fd = util::getFileDescriptor(fileName, "R");
	return fd != -1;
	}

bool LocalReader::fillBuffer ( char *buf, size_t buf_size )
	{
	return read(fd, buf, buf_size) == buf_size;
	}

string LocalReader::PageToString ( )
	{
	string temp = "";
	char buf[10240];
	int bytes = 0;

	while(	(bytes = read(fd, buf, 10240) ) > 0)
	{
		temp += string(buf, bytes );
	}
	return temp;
	}

ParsedUrl LocalReader::getUrl ( )
	{
	ParsedUrl url(fileName); //Fixme
	return url;
	}

bool LocalReader::checkStatus ( )
	{
	return true;
	}

void LocalReader::closeReader ( )
	{
	close(fd);
	}
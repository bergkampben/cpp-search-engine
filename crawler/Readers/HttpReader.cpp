//
// Created by Ben Bergkamp on 3/13/18.
//

#include "HttpReader.h"

std::runtime_error HTTPConnectionError( "Error connecting HTTP to url" );

bool HttpReader::request ( )
	{
	try
		{
		sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
		assert( sock != -1 );

		// Get the host address.

		struct hostent *host = gethostbyname( url.Host );
		if ( host == nullptr )
			throw HTTPConnectionError;
		assert( host );

		struct sockaddr_in address;
		memset( &address, 0, sizeof( address ) );
		address.sin_family = AF_INET;
		address.sin_port = htons( 80 );
		memcpy( &address.sin_addr, host->h_addr, host->h_length );

		// Connect to the host.

		int connectResult = connect( sock, ( struct sockaddr * ) &address,
		                             sizeof( address ) );
		assert( connectResult == 0 );

		// Send a GET message for the desired page.

		cout << "Socket Reader is pulling from the web" << endl;

		string getMessage = "GET ";
		getMessage += url.CompleteUrl;
		getMessage += " HTTP/1.1\r\nHost: ";
		getMessage += url.Host;
		getMessage += "\r\nConnection: close\r\n\r\n";

		cout << getMessage << endl;
		send( sock, getMessage.c_str( ), getMessage.length( ), 0 );

		bool isSuccess = checkStatus( );
		return isSuccess;

		}
	catch ( std::exception & e )
		{
		cerr << "Error trying to connect to Host" << endl;
		return false;
		}
	}

bool HttpReader::fillBuffer ( char *buf, size_t buf_size )
	{
	return ( recv( sock, buf, buf_size, 0 ) == buf_size );
	}

string HttpReader::PageToString ( )
	{

	string temp = "";
	char buf[10240];
	int bytes = 0;

	while ( ( bytes = recv( sock, buf, 10240, 0 ) ) > 0 )
		{
		temp += string( buf, bytes );
		}
	return temp;
	}

ParsedUrl HttpReader::getUrl ( )
	{
	return url;
	}


bool HttpReader::checkStatus ( )
	{
	string code = "";
	char buff[12];
	int bytes = 0;

	bytes = recv( sock, buff, 12, 0 );

	if ( strncmp( buff, "HTTP/1.1 200", 11 ) == 0 )
		return true;
	else if ( strncmp( buff, "HTTP/1.1 302", 11 ) == 0 )
		{
		cerr << "URL REDIRECTION" << endl;
		return false;
		}
	cerr << "Bad Request of TYPE::  " << buff << endl;
	return false;
	}


void HttpReader::closeReader ( )
	{
	close( sock );
	}
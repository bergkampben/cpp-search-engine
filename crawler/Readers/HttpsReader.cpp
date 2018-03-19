//
// Created by Ben Bergkamp on 3/13/18.
//

#include "HttpsReader.h"

void HttpsReader::request()
	{
	struct hostent *host = gethostbyname( url.Host );
	assert( host );

	struct sockaddr_in address;
	memset( &address, 0, sizeof( address ) );
	address.sin_family = AF_INET;
	address.sin_port = htons( 443 );
	memcpy( &address.sin_addr, host->h_addr, host->h_length );

	// Create a TCP/IP socket.

	sock = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	assert( sock != -1 );

	// Connect the socket to the host address.

	int connectResult = connect( sock, ( struct sockaddr * )&address,
								 sizeof( address ) );
	assert( connectResult == 0 );

	// Build an SSL layer and set it to read/write
	// to the socket we've connected.


	SSL_library_init( );

	ctx = SSL_CTX_new( SSLv23_method( ) );
	assert( ctx );
	ssl = SSL_new( ctx );
	assert( ssl );

	SSL_set_fd( ssl, sock );

	// Establish an SSL connection.

	int sslConnectResult = SSL_connect( ssl );
	assert( sslConnectResult == 1 );

	// Send a GET message for the desired page through the SSL.

	string getMessage = "GET ";
	getMessage += url.CompleteUrl;
	getMessage += " HTTP/1.1\r\nHost: ";
	getMessage += url.Host;
	getMessage += "\r\nConnection: close\r\n\r\n";

	cout << getMessage << endl;
	SSL_write( ssl, getMessage.c_str( ), getMessage.length( ) );

	}

bool HttpsReader::fillBuffer(char * buf, size_t buf_size)
	{
	return (SSL_read( ssl, buf, buf_size ) == buf_size);
	}

string HttpsReader::PageToString()
	{
	int total_size = 0;
	int buf_size = 10240;
	int current_size = buf_size;
	char *ssl_buffer = new char[buf_size];
	char *front = ssl_buffer;
	int bytes;

	while ((bytes = SSL_read(ssl, front, buf_size)) > 0) {

		total_size += bytes;
		current_size += buf_size;
		char *temp = new char[current_size];
		strcpy(temp, ssl_buffer);

		front = temp + strlen(ssl_buffer);
		delete[] ssl_buffer;
		ssl_buffer = temp;
	}

	return string(ssl_buffer, total_size);

	}

void HttpsReader::closeReader()
	{
	SSL_shutdown(ssl);
	SSL_free(ssl);
	SSL_CTX_free(ctx);
	close(sock);
	}


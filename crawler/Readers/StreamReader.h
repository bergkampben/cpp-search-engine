//
// Created by Jake Close on 2/13/18.
//

#pragma once

#include "../../shared/url.h"
#include "../../util/util.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <iostream>
#include <string.h>
#include <string>
#include <cassert>
#include <openssl/ssl.h>

using namespace std;


class StreamReader
	{
public:
	StreamReader() {};

	virtual void request() = 0;
	virtual bool fillBuffer(char * buf, size_t buf_size) = 0;
	virtual string PageToString() = 0;
	virtual void closeReader() = 0;


	};

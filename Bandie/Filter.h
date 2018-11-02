#pragma once

#include <stdio.h>
#include <WinSock2.h>
#include "include/windivert.h"

#define MAXBUF  0xFFFF

class Filter
{
public:
	static void Start();
	static void Stop();
private:
	static HANDLE handle, thread;
	static void start();
};


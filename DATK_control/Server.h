#include <stdio.h>
#include <winsock2.h>
#include <string>
#include <iostream>
#include <chrono>
#include "CM01.h"

#define SEND_SIZE 256
#define BUFFER_SIZE 256

class Server {
	public:
		int Inf();
		int Connect();
		int Receive();
		int Send(CM01* pModule);
		double receive_num;
};

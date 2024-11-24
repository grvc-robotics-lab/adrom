/*
 * Copyright (c) 2024 Alejandro Suarez, asuarezfm@us.es
 *
 * University of Seville - Robotics, Vision and Control Group
 *
 * Date: November 2024
 */

#ifndef MODULEINTERFACE_H_
#define MODULEINTERFACE_H_


// Standard library
#include <iostream>
#include <thread>
#include <fstream>
#include <string>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <fcntl.h>


// Specific library


// Constant definition
#define MAX_SIZE_USER_BUFFER	32


using namespace std;


class ModuleInterface
{
public:

	typedef struct
	{
		uint8_t header[3];		// "MSG" or "REQ" character sequence for messages and requests
		uint8_t moduleID;		// Module identifier
		uint8_t hostID;			// Host identifier
		uint8_t msgCode;		// Message code
		uint8_t msgOption;		// Message option
		uint8_t userBuffer[MAX_SIZE_USER_BUFFER];
		uint16_t userBufferSize;
		uint8_t checksum;		// XOR of all bytes except header and checksum
	} __attribute__((packed)) DATA_PACKET_MODULE_MSG;

	
	/***************** PUBLIC VARIABLES *****************/
	
	
	/***************** PUBLIC METHODS *****************/
	
	/*
	 * Constructor
	 * */
	ModuleInterface(uint8_t _moduleID, const string &_moduleInterfaceName);

	/*
	 * Destructor
	 * */
	virtual ~ModuleInterface();
	
	/*
	 * Open the UDP socket interface
	 */
	int openUDPSocket(const string &_moduleIP_Address, int _moduleUDP_TxPort, int _moduleUDP_RxPort);
	
	/*
	 * Send a request to the module
	 */
	int sendModuleRequest(uint8_t requestCode, uint8_t requestOption);
	
	/*
	 * Return 1 if signal was received from module, and zero in other case. Calling this function resets the flagSignalReceived
	 */
	int isMessageReceived();
	
	/*
	 * Return the signal code when received (zero by default).
	 */
	void getMessage(int & _msgCode, int & _msgOption);
	
	/*
	 * Close the UDP socket interface
	 */
	int closeUDPSocket();
	

private:
	/***************** PRIVATE VARIABLES *****************/
	string moduleInterfaceName;
	
	thread udpRxThread;
	
	struct sockaddr_in addrHost;
    struct hostent * host;
	string moduleIP_Address;
	int moduleUDP_TxPort;
	int moduleUDP_RxPort;
	int socketPublisher;
	
	uint8_t managerID;
	uint8_t moduleID;
	uint8_t msgCode;
	uint8_t msgOption;
	int flagMessageReceived;
	int flagTerminateThread;
	
	
	/***************** PRIVATE METHODS *****************/
	
	
	void udpRxThreadFunction();
	

};

#endif



/*
 * Copyright (c) 2024 Alejandro Suarez, asuarezfm@us.es
 *
 * University of Seville - Robotics, Vision and Control Group
 *
 * Date: Novemebr 2024
 */

#include "ModuleInterface.h"


/*
 * Constructor
 * */
ModuleInterface::ModuleInterface(uint8_t _moduleID, const string &_moduleInterfaceName)
{
	this->moduleID = _moduleID;
	this->moduleInterfaceName = _moduleInterfaceName;
	
	// Init variables
	this->moduleIP_Address = "";
	this->moduleUDP_TxPort = -1;
	this->moduleUDP_RxPort = -1;
	this->socketPublisher = -1;
	
	this->msgCode = 0;
	this->msgOption = 0;
	this->flagMessageReceived = 0;
	this->flagTerminateThread = 0;
}


/*
 * Destructor
 * */
ModuleInterface::~ModuleInterface()
{
}


/*
 * Open the UDP socket interface
 */
int ModuleInterface::openUDPSocket(const string &_moduleIP_Address, int _moduleUDP_TxPort, int _moduleUDP_RxPort)
{
	int errorCode = 0;
	
	
	// Open the UDP socket for sending requests to the modules
	this->socketPublisher = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(socketPublisher < 0)
    {
    	errorCode = 1;
    	cout << endl << "ERROR: [in ModuleInterface::openUDPSocket] could not open socket." << endl;
   	}
   	else
	{
	   	host = gethostbyname(_moduleIP_Address.c_str());
	    if(host == NULL)
		{
		    errorCode = 2;
			close(socketPublisher);
		    cout << "ERROR: [in ModuleInterface::openUDPSocket] could not get host by name" << endl;
		}
		else
		{
			// Set the address of the host
			bzero((char*)&addrHost, sizeof(struct sockaddr_in));
			this->addrHost.sin_family = AF_INET;
			bcopy((char*)host->h_addr, (char*)&addrHost.sin_addr.s_addr, host->h_length);
			this->addrHost.sin_port = htons(_moduleUDP_TxPort);
		}
	}
	
	if(errorCode == 0)
	{
		// Copy the IP and UDP ports
		this->moduleIP_Address = _moduleIP_Address;
		this->moduleUDP_TxPort = _moduleUDP_TxPort;
		this->moduleUDP_RxPort = _moduleUDP_RxPort;
		
		// Init the thread for receiving the messages from the module
		udpRxThread = thread(&ModuleInterface::udpRxThreadFunction, this);
		udpRxThread.detach();
	}
	
	
	return errorCode;
}
	

/*
 * Send a request to the module
 */
int ModuleInterface::sendModuleRequest(uint8_t _requestCode, uint8_t _requestOption)
{
	DATA_PACKET_MODULE_MSG dataPacketMouduleRequest;
	int bytesSent = 0;
	int errorCode = 0;
	
	// Set the fields of the request data packet
	dataPacketMouduleRequest.header[0] = 'R';
	dataPacketMouduleRequest.header[1] = 'E';
	dataPacketMouduleRequest.header[2] = 'Q';
	dataPacketMouduleRequest.moduleID = this->moduleID;
	dataPacketMouduleRequest.msgCode = _requestCode;
	dataPacketMouduleRequest.msgOption = _requestOption;
	
	// Send the request data packet
	bytesSent = sendto(this->socketPublisher, (char*)&dataPacketMouduleRequest, sizeof(DATA_PACKET_MODULE_MSG), 0, (struct sockaddr*)&addrHost, sizeof(struct sockaddr));
	if(bytesSent != sizeof(DATA_PACKET_MODULE_MSG))
	{
		errorCode = 1;
		cout << "ERROR: [in ModuleInterface::sendModuleRequest] could not send data." << endl;
	}
	
	
	return errorCode;
}


/*
 * Return 1 if signal was received from module. Calling this function resets the flagMessageReceived
 */
int ModuleInterface::isMessageReceived()
{
	return this->flagMessageReceived;
}

	
/*
 * Return the signal code when received (zero by default).
 */
void ModuleInterface::getMessage(int & _msgCode, int & _msgOption)
{
	_msgCode = this->msgCode;
	_msgOption = this->msgOption;
	
	this->flagMessageReceived = 0;
}


void ModuleInterface::udpRxThreadFunction()
{
	DATA_PACKET_MODULE_MSG * dataPacketModuleMessage;
	struct sockaddr_in addrReceiver;
	struct sockaddr_in addrSender;
	socklen_t addrLength;
	int socketReceiver = -1;
	int dataReceived = 0;
	char buffer[1024];
	int error = 0;
	
	// Open the socket in datagram mode
	socketReceiver = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(socketReceiver < 0) 
	{
		error = 1;
		cout << endl << "ERROR: [in ModuleInterface::udpRxThreadFunction] could not open socket." << endl;
	}

	// Set listenning address and port for server
	bzero((char*)&addrReceiver, sizeof(struct sockaddr_in));
	addrReceiver.sin_family = AF_INET;
	addrReceiver.sin_addr.s_addr = INADDR_ANY;
	addrReceiver.sin_port = htons(this->moduleUDP_RxPort);

	// Associates the address to the socket
	if(bind(socketReceiver, (struct sockaddr*)&addrReceiver, sizeof(addrReceiver)) < 0)
	{
		error = 1; 
		cout << endl << "ERROR: [in ModuleInterface::udpRxThreadFunction] could not associate address to socket." << endl;
	}
	else
	{
		// Set the socket as non blocking
		fcntl(socketReceiver, F_SETFL, O_NONBLOCK);
	}


	/******************************** THREAD LOOP START ********************************/
	
	while(error == 0 && flagTerminateThread == 0)
	{	
		dataReceived = recvfrom(socketReceiver, buffer, 1023, 0, (struct sockaddr*)&addrSender, &addrLength);
		if (dataReceived == sizeof(DATA_PACKET_MODULE_MSG))
		{
			// Check if message is correct
			if(buffer[0] == 'M' && buffer[1] == 'S' && buffer[2] == 'G')
			{
				// Get the signal from the data packet
				dataPacketModuleMessage = (DATA_PACKET_MODULE_MSG*)buffer;
				this->moduleID = dataPacketModuleMessage->moduleID;
				this->msgCode = dataPacketModuleMessage->msgCode;
				this->msgOption = dataPacketModuleMessage->msgOption;
				
				// Set the signal received flag
				this->flagMessageReceived = 1;
			}
		}
		else
		{
			// Wait 10 ms
			usleep(10000);
		}
	}
	
	/******************************** THREAD LOOP END ********************************/
	
	// Close the socket
	close(socketReceiver);
	
}

	
/*
 * Close the UDP socket interface
 */
int ModuleInterface::closeUDPSocket()
{
	int errorCode = 0;


	this->flagTerminateThread = 1;
	usleep(10000);	// Waits 10 ms to termiante thread

	
	return errorCode;
}


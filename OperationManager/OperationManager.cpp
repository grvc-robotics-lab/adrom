/*
 * Copyright (c) 2024 Alejandro Suarez, asuarezfm@us.es
 *
 * University of Seville - Robotics, Vision and Control Group
 *
 * Date: November 2024
 */

#include "OperationManager.h"


/*
 * Constructor
 * */
OperationManager::OperationManager()
{
	// Init variables
	flagOperationComplete = 0;
	flagAbortOperation = 0;
	flagTerminateOperation = 0;
}


/*
 * Destructor
 * */
OperationManager::~OperationManager()
{
}


/*
 * Open interfaces.
 */
int OperationManager::openInterfaces()
{
	int errorCode = 0;
	
	
	// this->mifVoCoR = new ModuleInterface(MODULE_ID_VOCOR, "VoCoR_Interface");
	
	this->mifLiCAS = new ModuleInterface(MODULE_ID_LiCAS, "LiCAS_Interface");
	this->mifLiCAS->openUDPSocket("192.168.0.166", 24001, 25001);	// Transmit through port 24001, Receive from port 25001
	
	this->mifVision = new ModuleInterface(MODULE_ID_VISION, "Vision_Interface");
	this->mifVision->openUDPSocket("192.168.0.166", 24003, 25003);
	
	// this->mifArduPilot = new ModuleInterface(MODULE_ID_ARDUPILOT, "ArduPilot_Interface");
	
	// this->mifPlanner = new ModuleInterface(MODULE_ID_PLANNER, "Planner_Interface");
	
	
	
	return errorCode;
}
	
/*
 * Get operation parameters (Supply Point, Delivery Point, packet ID)
 */
int OperationManager::getOperationParams(const string &_supplyPointName, const string &_deliveryPointName, const string &_parcelID)
{
	int errorCode = 0;
	
	
	this->supplyPointName = _supplyPointName;
	this->deliveryPointName = _deliveryPointName;
	this->parcelID = _parcelID;
	
	
	return errorCode;
}
	
/*
 * Start operation
 */
int OperationManager::startOperation()
{
	int msgCode_LiCAS = 0, msgOption_LiCAS = 0;
	int errorCode = 0;
	
	
	cout << "Starting operation:" << endl;
	cout << "\tSupply Point:\t" << this->supplyPointName << endl;
	cout << "\tDelivery Point:\t" << this->deliveryPointName << endl;
	cout << "\tParcel ID:\t" << this->parcelID << endl;
	
	
	// Phase 0: generate trajectory plan
	
	// Phase 1: Take-off from TOLP
	
	cout << "LiCAS Request: go to rest position...";
	this->mifLiCAS->sendModuleRequest(1, 0);
	while(this->mifLiCAS->isMessageReceived() == 0)
		usleep(10000);	// Wait 100 ms
	this->mifLiCAS->getMessage(msgCode_LiCAS, msgOption_LiCAS);
	if(msgCode_LiCAS == 1)
		cout << "DONE." << endl;
	else
		cout << "Request not correctly executed." << endl;
	
	usleep(2000000);
	
	cout << "LiCAS Request: go to operation position...";
	this->mifLiCAS->sendModuleRequest(2, 0);
	while(this->mifLiCAS->isMessageReceived() == 0)
		usleep(100000);	// Wait 100 ms
	this->mifLiCAS->getMessage(msgCode_LiCAS, msgOption_LiCAS);
	if(msgCode_LiCAS == 2)
		cout << "DONE." << endl;
	else
		cout << "Request not correctly executed." << endl;
	
	usleep(2000000);
	
	// Phase 2: Approach to Supply Point
	
	// Phase 3: Enable vision module and localize parcel
	cout << "Enabling vision module...";
	uint8_t parcelID = 3;
	this->mifVision->sendModuleRequest(1, parcelID);
	cout << "DONE." << endl;
	
	usleep(2000000);
	
	// Phase 4: Correct position based on vision module
	
	// Phase 5: Retrieve parcel
	cout << "Grasping parcel...";
	this->mifLiCAS->sendModuleRequest(3, 0);
	while(this->mifLiCAS->isMessageReceived() == 0)
		usleep(100000);	// Wait 100 ms
	this->mifLiCAS->getMessage(msgCode_LiCAS, msgOption_LiCAS);
	if(msgCode_LiCAS == 3)
		cout << "DONE." << endl;
	else
		cout << "Request not correctly executed." << endl;
	
	// Phase 6: Go to Delivery Point
	
	// Phase 7: Drop parcel
	
	// Phase 8: Go back to TOLP
	
	return errorCode;
}
	
/*
 * Terminate operation.
 */
int OperationManager::terminateOperation()
{
	int errorCode = 0;
	
	
	
	
	return errorCode;
}
	
/*
 * Close interfaces.
 */
int OperationManager::closeInterfaces()
{
	int errorCode = 0;
	
	
	// this->mifVoCoR->closeUDPSocket();
	this->mifLiCAS->closeUDPSocket();
	this->mifVision->closeUDPSocket();
	// this->mifArduPilot->closeUDPSocket();
	// this->mifPlanner->closeUDPSocket();
	
	
	
	return errorCode;
}



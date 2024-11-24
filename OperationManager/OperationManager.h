/*
 * Copyright (c) 2024 Alejandro Suarez, asuarezfm@us.es
 *
 * University of Seville - Robotics, Vision and Control Group
 *
 * Date: Novemeber 2024
 */

#ifndef OPERATIONMANAGER_H_
#define OPERATIONMANAGER_H_


// Standard library
#include <iostream>
#include <stdio.h>
#include <sys/time.h>


// Specific library
#include "../ModuleInterface/ModuleInterface.h"


// Constant definition
#define MODULE_ID_VOCOR			1
#define MODULE_ID_LiCAS			2
#define MODULE_ID_VISION		3
#define MODULE_ID_ARDUPILOT		4
#define MODULE_ID_PLANNER		5


using namespace std;


class OperationManager
{
public:

	/***************** PUBLIC VARIABLES *****************/
	
	
	/***************** PUBLIC METHODS *****************/
	
	/*
	 * Constructor
	 * */
	OperationManager();

	/*
	 * Destructor
	 * */
	virtual ~OperationManager();
	
	/*
	 * Open interfaces.
	 */
	int openInterfaces();
	
	/*
	 * Get operation parameters (Supply Point, Delivery Point, packet ID)
	 */
	int getOperationParams(const string &_supplyPointName, const string &_deliveryPointName, const string &_parcelID);
	
	/*
	 * Start operation
	 */
	int startOperation();
	
	/*
	 * Terminate operation.
	 */
	int terminateOperation();
	
	/*
	 * Close interfaces.
	 */
	int closeInterfaces();
	

private:
	/***************** PRIVATE VARIABLES *****************/

	ModuleInterface * mifVoCoR;
	ModuleInterface * mifLiCAS;
	ModuleInterface * mifVision;
	ModuleInterface * mifArduPilot;
	ModuleInterface * mifPlanner;
	
	string supplyPointName;
	string deliveryPointName;
	string parcelID;
	
	
	/***************** PRIVATE METHODS *****************/
	
	int flagOperationComplete;
	int flagAbortOperation;
	int flagTerminateOperation;
	

};

#endif



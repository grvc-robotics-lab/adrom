# adrom
Aerial Delivery Robot Operations Manager

# Description

This software module is intended to manage the realization of parcel grasping and delivery operations using an aerial robot with on-board mapping-localization, perception, and manipulation.

The operation consists of the following phases:

	(1) Take-off from Take-Off and Landing Position (TOLP, fixed position)
	(2) Approach to supply point (SP, coordinates as input parameter)
	(3) Correct position relative to parcel to be grasped based on vision
	(4) Grasp parcel with manipulator (parcel ID input parameter)
	(5) Go to delivery point (DP, coordinates as input parameter)
	(6) Drop parcel
	(7) Go back to TOLP
	
Input parameters are:

- The label identifying the XYZ-yaw coordinates of the supply point
- The label identifying the XYZ-yaw coordinates of the delivery point
- The label identifying the ID of the parcel to be grasped

The involved modules managed the the ADROM are:

- The Voice Command for Robot (VoCoR) interface, used to obtain the input parameters from the user voice instruction.
- The Multi-Rotor Path Planner (MRPP) that generates the trajectory from TOLP to SP, from SP to DP, and from DP to TOLP.
- The Multi-Rotor Flight Manager (MRFM) that receives the input trajectory according to the operation phase.
- The LiCAS Control Program (LCP) that conducts the parcel grasping from the SP and drop on the DP.
- The Vision Module (VM) that provides the grasping points for the arms and position corrections for the multi-rotor.

Communications between the ADROM and the software modules will be implemented through a message service with UDP sockets (see ModuleInterface.h). Messages correspond to C-style structures representing a data packet sent through the UDP socket.


# Requirements

No particular library is required, only standard C/C++ libraries and the cmake tool for compiling.

The ModuleTemplate project is intended to be easily integrated in any Linux system, removing any dependency from non standard libraries.


# Installation

Download or clone the project in your desired folder. Create a build forlder (mkdir build), go inside the folder (cd build), generate compilation files (cmake ..) and compile (make). The executable can be fund in the buil/Main folder.


# Usage

To run the module, it is necessary to provide as input arguments the labels of the supply point, the delivery point, and the parcel ID. Example:

./ADROM SPX DPY ParcelZ

The IP addresses and UDP ports of each module are defined within the OperationsManager class.

# Customization

This software is designed to be customized as required by the project. It is important to ensure that the data structure within ModuleInterface.h representing the data packet sent/received is exactly the same in for both sender and receiver. Otherwise, messages/requests will not be correctly interchanged.


# Contact

This sofware has been developed by Alejandro Suarez (asuarezfm@us.es) from the GRVC Robotics Lab of the University of Seville (Spain). Please, cite or include link of this repository in your publications.


# Disclaimer

This open-source software comes with absolutely no warranty. 

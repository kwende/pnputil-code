// UninstallDevice.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <windows.h>
#include <SetupAPI.h>
#include <string>
#include <iostream>
#include <newdev.h>
#include <cfgmgr32.h>

int main()
{
	// get the device info set for all devices of all classes
	// that are current attached. 
	auto deviceInfoSet = SetupDiGetClassDevs(
		nullptr,
		nullptr,
		nullptr,
		DIGCF_ALLCLASSES
	); 

	if (deviceInfoSet != INVALID_HANDLE_VALUE)
	{
		std::cout << "Got handle to device info set." << std::endl;

		SP_DEVINFO_DATA infoData; 
		::ZeroMemory(&infoData, sizeof(SP_DEVINFO_DATA)); 
		infoData.cbSize = sizeof(SP_DEVINFO_DATA); 

		// now enumerate the devices. 
		for (int i = 0; SetupDiEnumDeviceInfo(deviceInfoSet, i, &infoData); i++)
		{
			CHAR szHardwareInfo[2048]; 
			szHardwareInfo[0] = 0; 
			if (SetupDiGetDeviceRegistryPropertyA(deviceInfoSet, &infoData, SPDRP_FRIENDLYNAME, nullptr, (PBYTE)szHardwareInfo, sizeof(szHardwareInfo), nullptr))
			{
				auto deviceName = std::string(szHardwareInfo); 
				//std::cout << deviceName << std::endl; 
				if (deviceName.find("Ocuvera") != std::string::npos)
				{
					szHardwareInfo[0] = 0;
					if (SetupDiGetDeviceRegistryPropertyA(deviceInfoSet, &infoData, SPDRP_HARDWAREID, nullptr, (PBYTE)szHardwareInfo, sizeof(szHardwareInfo), nullptr))
					{
						std::cout << "Found " << deviceName << " with hardware ID " << szHardwareInfo << std::endl;
						if (SetupDiRemoveDevice(deviceInfoSet, &infoData))
						{
							std::cout << "Successfully removed the device." << std::endl;

	/*						if (UpdateDriverForPlugAndPlayDevicesA(nullptr, szHardwareInfo, "C:\\Windows\\INF\\audioendpoint.inf", INSTALLFLAG_FORCE, nullptr))
							{
								std::cout << "Succesfully rescanned and associated the device." << std::endl; 
							}
							else
							{
								std::cout << "Failed. Get last error is " << GetLastError() << std::endl; 
							}*/
						}
						else
						{
							std::cout << "Failed to uninstall. Error " << GetLastError() << std::endl;
						}
					}
				}
			}
		}

		DEVINST root = 0; 
		if (CM_Locate_DevNodeA(&root, nullptr, 0) == CR_SUCCESS)
		{
			if (CM_Reenumerate_DevNode(root, CM_REENUMERATE_SYNCHRONOUS) == CR_SUCCESS)
			{
				std::cout << "Successfully re-enumerated the device list." << std::endl; 
			}
			else
			{
				std::cout << "Failed to re-enumerate the device list." << std::endl; 
			}
		}
		else
		{
			std::cout << "Failed to locate the root device node." << std::endl; 
		}
	}
}

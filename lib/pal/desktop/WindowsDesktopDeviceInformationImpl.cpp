#define LOG_MODULE DBG_API
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif
#include "pal/PAL.hpp"
#include "pal/DeviceInformationImpl.hpp"
#include "WindowsEnvironmentInfo.h"

ARIASDK_LOG_INST_COMPONENT_NS("DeviceInfo", "Win32 Desktop Device Information")

#include <winsock2.h>
#include <iphlpapi.h>
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <locale>
#include <codecvt>

#pragma comment(lib, "IPHLPAPI.lib")

#define MALLOC(x) HeapAlloc(GetProcessHeap(), 0, (x))
#define FREE(x) HeapFree(GetProcessHeap(), 0, (x))

#define SYSINFO         "SYSTEM\\CurrentControlSet\\Control\\SystemInformation"
#define MANUFACTURER    "SystemManufacturer"
#define MODEL           "SystemProductName"
#define HARDWARE_ID     "ComputerHardwareId"

namespace PAL_NS_BEGIN {

    /* Value returned on computers with no network adapter available */
    static const char *netIfGuid = "{deadbeef-fade-dead-c0de-cafebabefeed}";
    static const char *manufacturer = "Unknown Manufacturer";
    static const char *model = "Unknown Model";

    /**
     * Returns the GUID of the 1st network adapter.
     */
    const char * getDeviceId()
    {
        ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
        PIP_ADAPTER_INFO pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
        if (pAdapterInfo != NULL)
        {
            DWORD result = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
            if (result == ERROR_NO_DATA)
                goto _exit;
            if (ulOutBufLen > sizeof(IP_ADAPTER_INFO))
            {
                FREE(pAdapterInfo);
                // redo the alloc with a bigger buffer size
                pAdapterInfo = (IP_ADAPTER_INFO *)MALLOC(ulOutBufLen);
                result = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen);
                if (result == ERROR_NO_DATA)
                    goto _exit;
            }
            if (pAdapterInfo != NULL)
            {
                if (pAdapterInfo->AdapterName != NULL)
                {
                    std::string adapterName { pAdapterInfo->AdapterName };
                    std::transform(adapterName.begin(), adapterName.end(), adapterName.begin(), ::tolower);
                    netIfGuid = _strdup(adapterName.c_str());
                }
                FREE(pAdapterInfo);
            }
        }
        _exit:
        return netIfGuid;
    }

    // Helper functions.
    PowerSource GetCurrentPowerSource()
    {
        PowerSource result = PowerSource_Unknown;
        SYSTEM_POWER_STATUS systemPowerStatus;
        if (GetSystemPowerStatus(&systemPowerStatus)) {
            // ACLineStatus - The AC power status.This member can be one of the following values.
            // Value    Meaning
            // 0        Offline
            // 1        Online
            // 255      Unknown status
            if (systemPowerStatus.ACLineStatus == 1) {
                result = PowerSource_Charging;
            } else
                if (systemPowerStatus.ACLineStatus == 0) {
                    result = PowerSource_Battery;
                }
        }
        return result;
    }

    static inline std::string to_string(const std::wstring& wstr)
    {
        using convert_typeX = std::codecvt_utf8<wchar_t>;
        std::wstring_convert<convert_typeX, wchar_t> converterX;
        return converterX.to_bytes(wstr);
    }

    std::string DeviceInformationImpl::GetDeviceTicket() const
    {
        return m_deviceTicket;
    }

    ///// IDeviceInformation API
    DeviceInformationImpl::DeviceInformationImpl() :m_registredCount(0),
            m_info_helper()
    {
        m_os_architecture = WindowsEnvironmentInfo::GetProcessorArchitecture();

        m_device_id = getDeviceId();

        char buff[256] = { 0 };
        DWORD size = sizeof(buff);

        // Detect manufacturer
        m_manufacturer = manufacturer;
        if (ERROR_SUCCESS == RegGetValueA(HKEY_LOCAL_MACHINE, (SYSINFO), (MANUFACTURER), RRF_RT_REG_SZ, NULL, &buff, &size)) {
            const std::string tmp(buff);
            m_manufacturer = tmp;
        }
        LOG_TRACE("Device Manufacturer=%s", m_manufacturer.c_str());

        // Detect model
        size = sizeof(buff);
        m_model = model;
        if (ERROR_SUCCESS == RegGetValueA(HKEY_LOCAL_MACHINE, (SYSINFO), (MODEL), RRF_RT_REG_SZ, NULL, &buff, &size)) {
            std::string tmp(buff);
            m_model = tmp;
        }
        LOG_TRACE("Device Model=%s", m_model.c_str());

        m_powerSource = GetCurrentPowerSource();

    }
    DeviceInformationImpl::~DeviceInformationImpl()
    {
    }

    IDeviceInformation* DeviceInformationImpl::Create()
    {
        return  new DeviceInformationImpl();
    }

} PAL_NS_END


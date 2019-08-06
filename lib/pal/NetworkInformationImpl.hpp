// Copyright (c) Microsoft Corporation. All rights reserved.
#ifndef NETWORKINFORMATIONIMPL_HPP
#define NETWORKINFORMATIONIMPL_HPP

#include "pal/PAL.hpp"
#include "Enums.hpp"
#include "INetworkInformation.hpp"
#include "InformationProviderImpl.hpp"

#include "IPropertyChangedCallback.hpp"

#include <string>

using namespace MAT;

namespace PAL_NS_BEGIN {

    class NetworkInformationImpl : public INetworkInformation
    {
    public:
        static INetworkInformation* Create(bool isNetDetectEnabled);

        // IInformationProvider API
        virtual int  RegisterInformationChangedCallback(IPropertyChangedCallback* pCallback) { m_registredCount++; return m_info_helper.RegisterInformationChangedCallback(pCallback); }
        virtual void UnRegisterInformationChangedCallback(int callbackToken) { --m_registredCount; m_info_helper.UnRegisterInformationChangedCallback(callbackToken); }

        // INetworkInformation API
        virtual std::string const& GetNetworkProvider() { return m_provider; };
        virtual NetworkType GetNetworkType() { return m_type; }
        virtual NetworkCost GetNetworkCost() { return m_cost; }

        virtual bool IsEthernetAvailable() { return false; }
        virtual bool IsWifiAvailable() { return false; }
        virtual bool IsWwanAvailable() { return false; }

        NetworkInformationImpl(bool isNetDetectEnabled);
        virtual ~NetworkInformationImpl();

    protected:
        std::string m_provider;
        NetworkType m_type;
        NetworkCost m_cost;

        InformatonProviderImpl m_info_helper;
        int m_registredCount;
        bool m_isNetDetectEnabled;

        // Disable copy constructor and assignment operator.
        NetworkInformationImpl(NetworkInformationImpl const& other) = delete;
        NetworkInformationImpl& operator=(NetworkInformationImpl const& other) = delete;
    };

} PAL_NS_END
#endif

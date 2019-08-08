// Copyright (c) Microsoft. All rights reserved.
#ifndef EVENTPROPERTIES_HPP
#define EVENTPROPERTIES_HPP

#define MAT_C_API

#include "Version.hpp"

#include "EventProperty.hpp"
#include "ctmacros.hpp"
#include "Enums.hpp"

#include <stdint.h>
#include <string>
#include <map>

#ifdef MAT_C_API
#include "mat.h"
#endif

namespace ARIASDK_NS_BEGIN
{

    struct EventPropertiesStorage; 

    /// <summary>
    /// The EventProperties class encapsulates event properties.
    /// </summary>
    class MATSDK_LIBABI EventProperties
    {
    public:
        /// <summary>
        /// Constructs an EventProperties object, taking a string for the property name.
        /// You must supply a non-empty name whenever you supply any custom properties for the event via <b>EventProperties</b>.
        /// </summary>
        EventProperties(const std::string& name);

        /// <summary>
        /// Constructs an EventProperties object (the default constructor).
        /// You must supply a non-empty name whenever you supply any custom properties for the event via <b>EventProperties</b>.
        /// </summary>
        EventProperties();

        /// <summary>
        /// The EventProperties copy constructor.
        /// </summary>
        EventProperties(EventProperties const& copy);

        /// <summary>
        /// The EventProperties equals operator overload.
        /// </summary>		
        EventProperties& operator=(EventProperties const& copy);

        /// <summary>
        /// Constructs an EventProperties object from a map of string to EventProperty.<br>
        /// You must supply a non-empty name whenever you supply any custom properties for the event via <b>EventProperties</b>.
        /// </summary>
        EventProperties(const std::string& name, const std::map<std::string, EventProperty> &properties);

        /// <summary>
        /// Adds a map of <string, EventProperty> to EventProperties.
        /// </summary>
        EventProperties& operator+=(const std::map<std::string, EventProperty> &properties);

        /// <summary>
        /// Assigns a map of <string, EventProperty> to EventProperties.
        /// </summary>
        EventProperties& operator=(const std::map<std::string, EventProperty> &properties);

        /// <summary>
        /// An EventProperties constructor using a C++11 initializer list.
        /// </summary>
        EventProperties(const std::string& name, std::initializer_list<std::pair<std::string const, EventProperty> > properties);

        /// <summary>
        /// An EventProperties assignment operator using C++11 initializer list.
        /// </summary>
        EventProperties& operator=(std::initializer_list<std::pair<std::string const, EventProperty> > properties);

        /// <summary>
        /// Sets the name of an event, given a string for the event name.
        /// You must supply a non-empty name whenever you supply any custom properties for the event via <b>EventProperties</b>.
        /// </summary>
        /// <param name="name">A string that contains the name of the event.</param>
        bool SetName(const std::string& name);

        /// <summary>
        /// Gets the name of an event. An empty string is returned if the name was never set.
        /// </summary>
        /// <returns>Name of the event</returns>
        const std::string& GetName() const;

        /// <summary>
        /// Sets the base type of an event.
        /// </summary>
        /// <param name="recordType">Base Type of event record.</param>
        bool SetType(const std::string& recordType);

        /// <summary>
        /// Gets the Base Type of an event.
        /// </summary>
        /// <returns>A string that contains the type of the event.</returns>
        const std::string& GetType() const;

        /// <summary>
        /// [optional] Sets the timestamp of an event, in milliseconds.
        /// <b>Note:</b> This method overrides the default timestamp generated by the telemetry system.
        /// </summary>
        /// <param name="timestampInEpochMillis">The UNIX timestamp in milliseconds. This is the amount of time since 00:00:00 
        /// Coordinated Universal Time (UTC), January, 1, 1970 (not counting leap seconds).</param>
        void SetTimestamp(const int64_t timestampInEpochMillis);

        /// <summary>
        /// Gets the timestamp of an event, in milliseconds.
        /// Zero is returned when the time stamp was not specified with SetTimestamp().
        /// </summary>
        /// <returns>The timestamp of the event, specified in milliseconds.</returns>
        int64_t GetTimestamp() const;

        /// <summary>
        /// [optional] Sets the transmit priority of an event.
        /// <b>Note:</b> If you don't specify a value, then the default priority is used.
        /// </summary>
        /// <param name="priority">The transmit priority.</param>
        void SetPriority(EventPriority priority);

        /// <summary>
        /// Gets the transmit priority of the event.
        /// </summary>
        /// <returns>The transmit priority.<returns>
        EventPriority GetPriority() const;

        /// <summary>
        /// [optional] Sets the transmit Latency of the event.
        /// </summary>
        /// <param name="latency">Event latency.</param>
        void SetLatency(EventLatency latency);

        /// <summary>
        /// Get the transmit Latency of the event.
        /// </summary>
        /// <returns>Transmit Latency of the event<returns>
        EventLatency GetLatency() const;

        /// <summary>
        /// [optional] Specify Persistence priority of an event.
        /// Default Persistence priority will be used for persisting the event if none was specified. 
        /// </summary>
        /// <param name="priority">Persistence of the event</param>
        void SetPersistence(EventPersistence persistence);

        /// <summary>
        /// Get the transmit Latency of the event.
        /// </summary>
        /// <returns>Transmit Latency of the event<returns>
        EventPersistence GetPersistence() const;

        /// <summary>
        /// [optional] Specify popSample of an event.
        /// </summary>
        /// <param name="priority">popSample of the event</param>
        void SetPopsample(double popSample);

        /// <summary>
        /// Get the popSample of the event.
        /// </summary>
        /// <returns>popSample of the event<returns>
        double GetPopSample() const;

        /// <summary>
        /// [optional] Specify Policy Bit flags for UTC usage of an event.
        /// Default values will be used for transmitting the event if none was specified. 
        /// </summary>
        /// <param name="priority">Transmit priority of the event</param>
        void SetPolicyBitFlags(uint64_t policyBitFlags);

        /// <summary>
        /// Get the Policy bit flags for UTC usage of the event.
        /// </summary>
        /// <returns>Transmit priority of the event<returns>
        uint64_t GetPolicyBitFlags() const;

        /// <summary>
        /// Sets the diagnostic level of an event. This is equivalent to:
        ///  ...
        ///  SetProperty(COMMONFIELDS_EVENT_LEVEL, level);
        ///  ...
        /// </summary>
        void SetLevel(uint8_t level)
        {
            SetProperty(COMMONFIELDS_EVENT_LEVEL, level);
        }

        /// <summary>
        /// Specify a property for an event.
        /// It either creates a new property if none exists or overwrites the existing one.
        /// </summary>
        /// <param name='name'>Name of the property</param>
        /// <param name='value'>Value of the property</param>
        /// <param name='piiKind'>PIIKind of the property</param>
        void SetProperty(const std::string& name, EventProperty value);

        /// <summary>
        /// Specify a property for an event.
        /// It either creates a new property if none exists or overwrites the existing one.
        /// </summary>
        void SetProperty(const std::string& name, char const*  value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC);

        /// <summary>
        /// Specify a property for an event.
        /// It either creates a new property if none exists or overwrites the existing one.
        /// </summary>
        void SetProperty(const std::string& name, const std::string&  value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC);

        /// <summary>
        /// Specify a property for an event.
        /// It either creates a new property if none exists or overwrites the existing one.
        /// </summary>
        void SetProperty(const std::string& name, double       value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC);

        /// <summary>
        /// Specify a property for an event.
        /// It either creates a new property if none exists or overwrites the existing one.
        /// </summary>
        void SetProperty(const std::string& name, int64_t      value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC);

        /// <summary>
        /// Specify a property for an event.
        /// It either creates a new property if none exists or overwrites the existing one.
        /// </summary>
        void SetProperty(const std::string& name, bool         value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC);

        /// <summary>
        /// Specify a property for an event.
        /// It either creates a new property if none exists or overwrites the existing one.
        /// </summary>
        void SetProperty(const std::string& name, time_ticks_t value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC);

        /// <summary>
        /// Specify a property for an event.
        /// It either creates a new property if none exists or overwrites the existing one.
        /// </summary>
        void SetProperty(const std::string& name, GUID_t       value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC);

        /// <summary>
        /// Specify a property for an event. It either creates a new property if none exists or overwrites the existing one.<br>
        /// All integer types are currently being converted to int64_t.
        /// </summary>
        void SetProperty(const std::string& name, int8_t       value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC) { SetProperty(name, (int64_t)value, piiKind, category); }

        /// <summary>
        /// Specify a property for an event. It either creates a new property if none exists or overwrites the existing one.<br>
        /// All integer types are currently being converted to int64_t.
        void SetProperty(const std::string& name, int16_t      value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC) { SetProperty(name, (int64_t)value, piiKind, category); }

        /// <summary>
        /// Specify a property for an event. It either creates a new property if none exists or overwrites the existing one.<br>
        /// All integer types are currently being converted to int64_t.
        void SetProperty(const std::string& name, int32_t      value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC) { SetProperty(name, (int64_t)value, piiKind, category); }

        /// <summary>
        /// Specify a property for an event. It either creates a new property if none exists or overwrites the existing one.<br>
        /// All integer types are currently being converted to int64_t.
        void SetProperty(const std::string& name, uint8_t      value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC) { SetProperty(name, (int64_t)value, piiKind, category); }

        /// <summary>
        /// Specify a property for an event. It either creates a new property if none exists or overwrites the existing one.<br>
        /// All integer types are currently being converted to int64_t.
        void SetProperty(const std::string& name, uint16_t     value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC) { SetProperty(name, (int64_t)value, piiKind, category); }

        /// <summary>
        /// Specify a property for an event. It either creates a new property if none exists or overwrites the existing one.<br>
        /// All integer types are currently being converted to int64_t.
        void SetProperty(const std::string& name, uint32_t     value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC) { SetProperty(name, (int64_t)value, piiKind, category); }

        /// <summary>
        /// Specify a property for an event. It either creates a new property if none exists or overwrites the existing one.<br>
        /// All integer types are currently being converted to int64_t.
        void SetProperty(const std::string& name, uint64_t     value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC) { SetProperty(name, (int64_t)value, piiKind, category); }

        /// <summary>
        /// Specify a property for an event.
        /// It either creates a new property if none exists or overwrites the existing one.
        /// </summary>
        void SetProperty(const std::string& name, std::vector<std::string>& value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC);

        /// <summary>
        /// Specify a property for an event.
        /// It either creates a new property if none exists or overwrites the existing one.
        /// </summary>
        void SetProperty(const std::string& name, std::vector<GUID_t>& value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC);

        /// <summary>
        /// Specify a property for an event.
        /// It either creates a new property if none exists or overwrites the existing one.
        /// </summary>
        void SetProperty(const std::string& name, std::vector<double>& value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC);

        /// <summary>
        /// Specify a property for an event.
        /// It either creates a new property if none exists or overwrites the existing one.
        /// </summary>
        void SetProperty(const std::string& name, std::vector<int64_t>& value, PiiKind piiKind = PiiKind_None, DataCategory category = DataCategory_PartC);

        /// <summary>
        /// Get the properties bag of an event.
        /// </summary>
        /// <returns>Properties bag of the event</returns>
        const std::map<std::string, EventProperty>& GetProperties(DataCategory category = DataCategory_PartC) const;

        /// <summary>
        /// Get the Pii properties bag of an event.
        /// </summary>
        /// <returns>Pii Properties bag of the event</returns>
        const std::map<std::string, std::pair<std::string, PiiKind> > GetPiiProperties(DataCategory category = DataCategory_PartC) const;

        /// <summary>
        /// Erase property from event.
        /// </summary>
        size_t erase(std::string key, DataCategory category = DataCategory_PartC);

        virtual ~EventProperties();

#ifdef MAT_C_API
        /// Implementation of ABI-safe packing of EventProperties object
        evt_prop* pack();
        bool unpack(evt_prop *packed, size_t size);
#endif

    private:
        EventPropertiesStorage* m_storage;
    };

} ARIASDK_NS_END

#endif
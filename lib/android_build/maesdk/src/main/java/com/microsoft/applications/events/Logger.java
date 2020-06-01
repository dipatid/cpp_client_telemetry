package com.microsoft.applications.events;

import java.util.ArrayList;
import java.util.Date;
import java.util.Map;
import java.util.UUID;

class Logger implements ILogger {
    private final long m_nativePtr;

    Logger(long nativePtr) {
        m_nativePtr = nativePtr;
    }

    private native long nativeGetSemanticContext(long nativePtr);

    /**
     * Gets an ISemanticContext interface through which you can specify the semantic context for this logger instance.
     *
     * @return An instance of the ISemanticContext interface
     */
    @Override
    public ISemanticContext getSemanticContext() {
        return new SemanticContext(nativeGetSemanticContext(m_nativePtr));
    }

    private static native int nativeSetContextStringValue(long nativePtr, String name, String value, int piiKind);

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * a string that contains the property value,
     * and tags the property with its PiiKind (Personal Identifiable Information kind).
     * @param name A string that contains the name of the property.
     * @param value A string that contains the property value.
     * @param piiKind One of the ::PiiKind enumeration values.
     */
    @Override
    public void setContext(final String name, final String value, final PiiKind piiKind) {
        if (name == null || name.trim().isEmpty())
            throw new IllegalArgumentException("name is null or empty");
        if (value == null)
            throw new IllegalArgumentException("value is null");
        if (piiKind == null)
            throw new IllegalArgumentException("piiKind is null");

        nativeSetContextStringValue(m_nativePtr, name, value, piiKind.getValue());
    }

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * a string that contains the property value,
     * and tags the property with default PiiKind_None.
     * @param name A string that contains the name of the property.
     * @param value A string that contains the property value.
     */
    @Override
    public void setContext(final String name, final String value) {
        setContext(name, value, PiiKind.None);
    }

    private static native int nativeSetContextDoubleValue(long nativePtr, String name, double value, int piiKind);

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * a double that contains the property value,
     * and tags the property with its PiiKind (Personal Identifiable Information kind).
     * @param name A string that contains the name of the property.
     * @param value A double that contains the property value.
     * @param piiKind One of the ::PiiKind enumeration values.
     */
    @Override
    public void setContext(final String name, final double value, final PiiKind piiKind) {
        if (name == null || name.trim().isEmpty())
            throw new IllegalArgumentException("name is null or empty");
        if (piiKind == null)
            throw new IllegalArgumentException("piiKind is null");

        nativeSetContextDoubleValue(m_nativePtr, name, value, piiKind.getValue());
    }

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * a double that contains the property value,
     * and tags the property with default PiiKind_None.
     * @param name A string that contains the name of the property.
     * @param value A double that contains the property value.
     */
    @Override
    public void setContext(final String name, final double value) {
        setContext(name, value, PiiKind.None);
    }

    private static native int nativeSetContextLongValue(long nativePtr, String name, long value, int piiKind);

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * an int64_t that contains the property value,
     * and tags the property with its PiiKind (Personal Identifiable Information kind).
     * @param name A string that contains the name of the property.
     * @param value A long that contains the property value.
     * @param piiKind One of the ::PiiKind enumeration values.
     */
    @Override
    public void setContext(final String name, final long value, final PiiKind piiKind) {
        if (name == null || name.trim().isEmpty())
            throw new IllegalArgumentException("name is null or empty");
        if (piiKind == null)
            throw new IllegalArgumentException("piiKind is null");

        nativeSetContextLongValue(m_nativePtr, name, value, piiKind.getValue());
    }

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * an int64_t that contains the property value,
     * and tags the property with default PiiKind_None.
     * @param name A string that contains the name of the property.
     * @param value A long that contains the property value.
     */
    @Override
    public void setContext(final String name, final long value) {
        setContext(name, value, PiiKind.None);
    }

    private static native int nativeSetContextIntValue(long nativePtr, String name, int value, int piiKind);

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * an int32_t that contains the property value,
     * and tags the property with its PiiKind (Personal Identifiable Information kind).
     * @param name A string that contains the name of the property.
     * @param value An int that contains the property value.
     * @param piiKind One of the ::PiiKind enumeration values.
     */
    @Override
    public void setContext(String name, final int value, final PiiKind piiKind) {
        if (name == null || name.trim().isEmpty())
            throw new IllegalArgumentException("name is null or empty");
        if (piiKind == null)
            throw new IllegalArgumentException("piiKind is null");

        nativeSetContextIntValue(m_nativePtr, name, value, piiKind.getValue());
    }

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * an int32_t that contains the property value,
     * and tags the property with default PiiKind_None.
     * @param name A string that contains the name of the property.
     * @param value An int that contains the property value.
     */
    @Override
    public void setContext(String name, final int value) {
        setContext(name, value, PiiKind.None);
    }

    private static native int nativeSetContextBoolValue(long nativePtr, String name, boolean value, int piiKind);

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * a boolean that contains the property value,
     * and tags the property with its PiiKind (Personal Identifiable Information kind).
     * @param name A string that contains the name of the property.
     * @param value A boolean that contains the property value.
     * @param piiKind One of the ::PiiKind enumeration values.
     */
    @Override
    public void setContext(final String name, final boolean value, final PiiKind piiKind) {
        if (name == null || name.trim().isEmpty())
            throw new IllegalArgumentException("name is null or empty");
        if (piiKind == null)
            throw new IllegalArgumentException("piiKind is null");

        nativeSetContextBoolValue(m_nativePtr, name, value, piiKind.getValue());
    }

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * a boolean that contains the property value,
     * and tags the property with default PiiKind_None.
     * @param name A string that contains the name of the property.
     * @param value A boolean that contains the property value.
     */
    @Override
    public void setContext(final String name, final boolean value) {
        setContext(name, value, PiiKind.None);
    }

    private static native int nativeSetContextTimeTicksValue(long nativePtr, String name, long value, int piiKind);

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * a .NET time_ticks_t that contains the property value,
     * and tags the property with its PiiKind (Personal Identifiable Information kind).
     * @param name A string that contains the name of the property.
     * @param value A TimeTicks that contains the property value.
     * @param piiKind One of the ::PiiKind enumeration values.
     */
    private void setContext(final String name, final TimeTicks value, final PiiKind piiKind) {
        if (name == null || name.trim().isEmpty())
            throw new IllegalArgumentException("name is null or empty");
        if (value == null)
            throw new IllegalArgumentException("value is null");
        if (piiKind == null)
            throw new IllegalArgumentException("piiKind is null");

        nativeSetContextTimeTicksValue(m_nativePtr, name, value.getTicks(), piiKind.getValue());
    }

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * a Date that contains the property value,
     * and tags the property with default PiiKind_None.
     * @param name A string that contains the name of the property.
     * @param value The property's Date value.
     * @param piiKind One of the ::PiiKind enumeration values.
     */
    @Override
    public void setContext(final String name, final Date value, PiiKind piiKind) {
        setContext(name, new TimeTicks(value), piiKind);
    }

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * a Date that contains the property value,
     * and tags the property with default PiiKind_None.
     * @param name A string that contains the name of the property.
     * @param value The property's Date value.
     */
    @Override
    public void setContext(final String name, final Date value) {
        setContext(name, value, PiiKind.None);
    }

    private static native int nativeSetContextGuidValue(long nativePtr, String name, String value, int piiKind);

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * a UUID/GUID that contains the property value,
     * and tags the property with its PiiKind (Personal Identifiable Information kind).
     * @param name A string that contains the name of the property.
     * @param value A UUID/GUID that contains the property value.
     * @param piiKind One of the ::PiiKind enumeration values.
     */
    @Override
    public void setContext(final String name, final UUID value, final PiiKind piiKind) {
        if (name == null || name.trim().isEmpty())
            throw new IllegalArgumentException("name is null or empty");
        if (value == null)
            throw new IllegalArgumentException("value is null");
        if (piiKind == null)
            throw new IllegalArgumentException("piiKind is null");

        nativeSetContextGuidValue(m_nativePtr, name, value.toString(), piiKind.getValue());
    }

    /**
     * Adds (or overrides) a property of the context associated with this logger instance,
     * taking a string that contains the name of the context,
     * a UUID/GUID that contains the property value,
     * and tags the property with default PiiKind_None.
     * @param name A string that contains the name of the property.
     * @param value A UUID/GUID that contains the property value.
     */
    @Override
    public void setContext(final String name, final UUID value) {
        setContext(name, value, PiiKind.None);
    }

    private native void nativeSetContextEventProperty(long nativePtr, String name, EventProperty prop);

    /**
     * Populate event property using EventProperty value object.
     * @param name Property name.
     * @param prop Property value object.
     */
    @Override
    public void SetContext(final String name, final EventProperty prop) {
        if (name == null || name.trim().isEmpty())
            throw new IllegalArgumentException("name is null or empty");
        if (prop == null)
            throw new IllegalArgumentException("prop is null");

        nativeSetContextEventProperty(m_nativePtr, name, prop);
    }

    private native void nativeSetParentContext(long nativeLoggerPtr, long nativeSemanticContextPtr);

    /**
     * Allows the logger to inherit the alternate parent context.
     *
     * Default context wiring rules:
     *  host loggers inherit their common host LogManager context.
     *  guest loggers do not inherit their host LogManager context due to privacy reasons.
     *
     * @param context he context.
     */
    @Override
    public void setParentContext(final ISemanticContext context) {
        if (context == null)
            throw new IllegalArgumentException("context is null");

        nativeSetParentContext(m_nativePtr, ((SemanticContext)context).getNativeSemanticContextPtr());
    }

    private native void nativeLogAppLifecycle(long nativeLoggerPtr, int appLifecycleState,
                                              String eventName, String eventType, int eventLatency, int eventPersistence,
                                              double eventPopSample, long eventPolicyBitflags, long timestampInMillis,
                                              Object[] eventPropertyStringKey, Object[] eventPropertyValue);

    /**
     * Logs the state of the application lifecycle.
     *
     * @param state The state in the application's lifecycle, specified by one of the AppLifecycleState enum values.
     * @param properties Properties of this AppLifecycle event, specified using an EventProperties object.
     */
    @Override
    public void logAppLifecycle(final AppLifecycleState state, final EventProperties properties) {
        if (state == null)
            throw new IllegalArgumentException("state is null");
        if (properties == null)
            throw new IllegalArgumentException("properties is null");

        EventPropertiesStorage storage = properties.getStorage();
        String eventName = storage.eventName;
        String eventType = storage.eventType;
        EventLatency eventLatency = storage.eventLatency;
        EventPersistence eventPersistence = storage.eventPersistence;
        double eventPopSample = storage.eventPopSample;
        long eventPolicyBitflags = storage.eventPolicyBitflags;
        long timestampInMillis = storage.timestampInMillis;

        ArrayList<String> eventPropertyStringKey = new ArrayList<>();
        ArrayList<EventProperty> eventPropertyValue = new ArrayList<>();
        for (Map.Entry<String, EventProperty> entry : storage.properties.entrySet()) {
            eventPropertyStringKey.add(entry.getKey());
            eventPropertyValue.add(entry.getValue());
        }

        nativeLogAppLifecycle(m_nativePtr, state.getValue(),
                eventName, eventType, eventLatency.getValue(), eventPersistence.getValue(), eventPopSample,
                eventPolicyBitflags, timestampInMillis, eventPropertyStringKey.toArray(), eventPropertyValue.toArray());
    }

    private native void nativeLogSession(long nativeLoggerPtr, int sessionState,
                                              String eventName, String eventType, int eventLatency, int eventPersistence,
                                              double eventPopSample, long eventPolicyBitflags, long timestampInMillis,
                                              Object[] eventPropertyStringKey, Object[] eventPropertyValue);

    /**
     * Logs the state of the application session.
     *
     * @param state The state in the application's lifecycle, as one of the SessionState enumeration values.
     * @param properties Properties of this session event, specified using an EventProperties object.
     */
    @Override
    public void logSession(final SessionState state, final  EventProperties properties) {
        if (state == null)
            throw new IllegalArgumentException("state is null");
        if (properties == null)
            throw new IllegalArgumentException("properties is null");

        EventPropertiesStorage storage = properties.getStorage();
        String eventName = storage.eventName;
        String eventType = storage.eventType;
        EventLatency eventLatency = storage.eventLatency;
        EventPersistence eventPersistence = storage.eventPersistence;
        double eventPopSample = storage.eventPopSample;
        long eventPolicyBitflags = storage.eventPolicyBitflags;
        long timestampInMillis = storage.timestampInMillis;

        ArrayList<String> eventPropertyStringKey = new ArrayList<>();
        ArrayList<EventProperty> eventPropertyValue = new ArrayList<>();
        for (Map.Entry<String, EventProperty> entry : storage.properties.entrySet()) {
            eventPropertyStringKey.add(entry.getKey());
            eventPropertyValue.add(entry.getValue());
        }

        nativeLogSession(m_nativePtr, state.getValue(),
                eventName, eventType, eventLatency.getValue(), eventPersistence.getValue(), eventPopSample,
                eventPolicyBitflags, timestampInMillis, eventPropertyStringKey.toArray(), eventPropertyValue.toArray());

    }

    private native void nativeLogEventName(long nativeLoggerPtr, String name);

    /**
     * Logs the custom event with the specified name.
     *
     * @param name A string that contains the name of the custom event.
     */
    @Override
    public void logEvent(final String name) {
        if (name == null || name.trim().isEmpty())
            throw new IllegalArgumentException("name is null or empty");

        nativeLogEventName(m_nativePtr, name);
    }


    private native void nativeLogEventProperties(long nativeLoggerPtr,
                                         String eventName, String eventType, int eventLatency, int eventPersistence,
                                         double eventPopSample, long eventPolicyBitflags, long timestampInMillis,
                                         Object[] eventPropertyStringKey, Object[] eventPropertyValue);

    /**
     * Logs a custom event with the specified name and properties.
     *
     * @param properties Properties of this custom event, specified using an EventProperties object.
     */
    @Override
    public void logEvent(final EventProperties properties) {
        if (properties == null)
            throw new IllegalArgumentException("properties is null");

        EventPropertiesStorage storage = properties.getStorage();
        String eventName = storage.eventName;
        String eventType = storage.eventType;
        EventLatency eventLatency = storage.eventLatency;
        EventPersistence eventPersistence = storage.eventPersistence;
        double eventPopSample = storage.eventPopSample;
        long eventPolicyBitflags = storage.eventPolicyBitflags;
        long timestampInMillis = storage.timestampInMillis;

        ArrayList<String> eventPropertyStringKey = new ArrayList<>();
        ArrayList<EventProperty> eventPropertyValue = new ArrayList<>();
        for (Map.Entry<String, EventProperty> entry : storage.properties.entrySet()) {
            eventPropertyStringKey.add(entry.getKey());
            eventPropertyValue.add(entry.getValue());
        }

        nativeLogEventProperties(m_nativePtr,
                eventName, eventType, eventLatency.getValue(), eventPersistence.getValue(), eventPopSample,
                eventPolicyBitflags, timestampInMillis, eventPropertyStringKey.toArray(), eventPropertyValue.toArray());

    }

    private native void nativeLogFailure(long nativeLoggerPtr, String signature, String detail,
                                                 String eventName, String eventType, int eventLatency, int eventPersistence,
                                                 double eventPopSample, long eventPolicyBitflags, long timestampInMillis,
                                                 Object[] eventPropertyStringKey, Object[] eventPropertyValue);

    /**
     * Logs a failure event - such as an application exception.
     *
     * @param signature A string that contains the signature that identifies the bucket of the failure.
     * @param detail A string that contains a description of the failure.
     * @param properties Properties of this failure event, specified using an EventProperties object.
     */
    @Override
    public void logFailure(final String signature, final String detail, final EventProperties properties) {
        if (signature == null || signature.trim().isEmpty())
            throw new IllegalArgumentException("signature is null or empty");
        if (detail == null || detail.trim().isEmpty())
            throw new IllegalArgumentException("detail is null or empty");
        if (properties == null)
            throw new IllegalArgumentException("properties is null");

        EventPropertiesStorage storage = properties.getStorage();
        String eventName = storage.eventName;
        String eventType = storage.eventType;
        EventLatency eventLatency = storage.eventLatency;
        EventPersistence eventPersistence = storage.eventPersistence;
        double eventPopSample = storage.eventPopSample;
        long eventPolicyBitflags = storage.eventPolicyBitflags;
        long timestampInMillis = storage.timestampInMillis;

        ArrayList<String> eventPropertyStringKey = new ArrayList<>();
        ArrayList<EventProperty> eventPropertyValue = new ArrayList<>();
        for (Map.Entry<String, EventProperty> entry : storage.properties.entrySet()) {
            eventPropertyStringKey.add(entry.getKey());
            eventPropertyValue.add(entry.getValue());
        }

        nativeLogFailure(m_nativePtr, signature, detail,
                eventName, eventType, eventLatency.getValue(), eventPersistence.getValue(), eventPopSample,
                eventPolicyBitflags, timestampInMillis, eventPropertyStringKey.toArray(), eventPropertyValue.toArray());
    }


    private native void nativeLogFailureWithCategoryId(long nativeLoggerPtr, String signature, String detail, String category, String id,
                                         String eventName, String eventType, int eventLatency, int eventPersistence,
                                         double eventPopSample, long eventPolicyBitflags, long timestampInMillis,
                                         Object[] eventPropertyStringKey, Object[] eventPropertyValue);

    /**
     * Logs a failure event - such as an application exception.
     *
     * @param signature A string that contains the signature that identifies the bucket of the failure.
     * @param detail A string that contains a description of the failure.
     * @param category A string that contains the category of the failure - such as an application error,
     *                 application not responding, or application crash
     * @param id A string that contains the identifier that uniquely identifies this failure.
     * @param properties Properties of this failure event, specified using an EventProperties object.
     */
    @Override
    public void logFailure(final String signature, final String detail, final String category,
                           final String id, final EventProperties properties) {

        if (signature == null || signature.trim().isEmpty())
            throw new IllegalArgumentException("signature is null or empty");
        if (detail == null || detail.trim().isEmpty())
            throw new IllegalArgumentException("detail is null or empty");
        if (properties == null)
            throw new IllegalArgumentException("properties is null");

        EventPropertiesStorage storage = properties.getStorage();
        String eventName = storage.eventName;
        String eventType = storage.eventType;
        EventLatency eventLatency = storage.eventLatency;
        EventPersistence eventPersistence = storage.eventPersistence;
        double eventPopSample = storage.eventPopSample;
        long eventPolicyBitflags = storage.eventPolicyBitflags;
        long timestampInMillis = storage.timestampInMillis;

        ArrayList<String> eventPropertyStringKey = new ArrayList<>();
        ArrayList<EventProperty> eventPropertyValue = new ArrayList<>();
        for (Map.Entry<String, EventProperty> entry : storage.properties.entrySet()) {
            eventPropertyStringKey.add(entry.getKey());
            eventPropertyValue.add(entry.getValue());
        }

        nativeLogFailureWithCategoryId(m_nativePtr, signature, detail, category, id,
                eventName, eventType, eventLatency.getValue(), eventPersistence.getValue(), eventPopSample,
                eventPolicyBitflags, timestampInMillis, eventPropertyStringKey.toArray(), eventPropertyValue.toArray());
    }

    /**
     * Logs a page view event,
     * taking a string that contains the event identifier,
     * a string that contains a friendly name for the page,
     * and properties of the event.<br>
     * <b>Note:</b> A page view event is normally the result of a user action on a UI page
     * such as a search query, a content request, or a page navigation.
     *
     * @param id A string that contains an identifier that uniquely identifies this page.
     * @param pageName A string that contains the friendly name of the page.
     * @param properties Properties of this page view event, specified using an EventProperties object.
     */
    @Override
    public void logPageView(final String id, final String pageName, final EventProperties properties) {
        //ToDO
    }

    /**
     * Logs a page view event,
     * taking a string that contains the event identifier,
     * a string that contains a friendly name for the page,
     * a string that contains the page category,
     * a string that contains the page's URI,
     * a string that contains the referring page's URI,
     * and properties of the event.<br>
     * <b>Note:</b> A page view event is normally the result of a user action on a UI page
     * such as a search query, a content request, or a page navigation.
     *
     * @param id A string that contains the identifier that uniquely identifies this page.
     * @param pageName A string that contains the friendly name of the page.
     * @param category A string that contains the category to which this page belongs.
     * @param uri A string that contains the URI of this page.
     * @param referrerUri A string that contains the URI of the page that refers to this page.
     * @param properties Properties of this page view event, specified using an EventProperties object.
     */
    @Override
    public void logPageView(final String id, final String pageName, final String category,
                            final String uri, final String referrerUri, final EventProperties properties){
        //ToDO
    }

    /**
     * Logs a page action event,
     * taking a string that contains the page view identifier,
     * the action type,
     * and the action event properties.
     *
     * @param pageViewId A string that contains an identifier that uniquely identifies the page view.
     * @param actionType The generic type of the page action, specified as one of the ::ActionType enumeration values.
     * @param properties Properties of this page action event, specified using an EventProperties object.
     */
    @Override
    public void logPageAction(final String pageViewId, final ActionType actionType, final EventProperties properties) {
        //ToDO
    }

    /**
     * Logs a detailed page action event,
     * taking a reference to the page action data,
     * and the action event properties.
     *
     * @param pageActionData Detailed information about the page action, contained in a PageActionData object.
     * @param properties Properties of this page action event, contained in an EventProperties object.
     */
    @Override
    public void logPageAction(final PageActionData pageActionData, final EventProperties properties) {
        //ToDO
    }

    /**
     * Logs a sampled metric event - such as a performance counter,
     * taking a name for the sampled metric,
     * a double that contains the value of the sampled metric,
     * a string that contains the units of measure of the sampled metric,
     * and a reference to an EventProperties object to hold the values.
     *
     * @param name A string that contains the name of the sampled metric.
     * @param value A double that holds the value of the sampled metric.
     * @param units A string that contains the units of the metric value.
     * @param properties Properties of this sampled metric event, specified using an EventProperties object.
     */
    @Override
    public void logSampledMetric(final String name, double value, final String units, final EventProperties properties) {
        //ToDO
    }

    /**
     * Logs a sampled metric event - such as a performance counter,
     * taking a name for the sampled metric,
     * a double that contains the value of the sampled metric,
     * a string that contains the units of measure of the sampled metric,
     * a string that contains the name of the metric instance,
     * a string that contains the name of the object class,
     * a string that contains the object identifier,
     * and a reference to an EventProperties object to hold the values.
     *
     * @param name A string that contains the name of the sampled metric.
     * @param value A double that contains the value of the sampled metric.
     * @param units A string that contains the units of the metric value.
     * @param instanceName A string that contains the name of this metric instance. E.g., <i>performance counter</i>.
     * @param objectClass A string that contains the object class for which this metric tracks.
     * @param objectId A string that contains the object identifier for which this metric tracks.
     * @param properties Properties of this sampled metric event, specified using an EventProperties object.
     */
    @Override
    public void logSampledMetric(final String name, double value, final String units, final String instanceName,
                                 final String objectClass, final String objectId, final EventProperties properties) {
        //ToDO
    }

    /**
     * Logs a precomputed aggregated metric event. For example, <i>queue length</i>.
     * @param name A string that contains the name of the aggregated metric.
     * @param duration A long that contains the duration (in microseconds) over which this metric is aggregated.
     * @param count A long that contains the count of the aggregated metric observations.
     * @param properties Properties of this aggregated metric event, specified using an EventProperties object.
     */
    @Override
    public void logAggregatedMetric(final String name, long duration, long count, final EventProperties properties) {
        //ToDO
    }

    /**
     * Logs a precomputed aggregated metrics event,
     * taking a reference to an AggregatedMetricData object,
     * and a reference to a EventProperties object.
     *
     * @param metricData Detailed information about the aggregated metric, contained in an AggregatedMetricData object.
     * @param properties Properties of this aggregated metric event, specified in an EventProperties object.
     */
    @Override
    public void logAggregatedMetric(final AggregatedMetricData metricData, final EventProperties properties) {
        //ToDO
    }

    /**
     * Logs a trace event for troubleshooting.
     *
     * @param level Level of the trace, as one of the TraceLevel enumeration values.
     * @param message A string that contains the a description of the trace.
     * @param properties Properties of this trace event, specified using an EventProperties object.
     */
    @Override
    public void logTrace(final TraceLevel level, final String message, final EventProperties properties) {
        //ToDO
    }

    /**
     * Logs a user's state.
     *
     * @param state he user's reported state, specified using one of the ::UserState enumeration values.
     * @param timeToLiveInMillis A long that contains the duration (in milliseconds) for which the state reported is valid.
     * @param properties Properties of this user state event, specified using an EventProperties object.
     */
    @Override
    public void logUserState(final UserState state, final long timeToLiveInMillis, final EventProperties properties) {
        //ToDO
    }

    /**
     * Set default diagnostic level of this logger instance.
     *
     * @param level Diagnostic level.
     */
    @Override
    public void setLevel(final int level) {
        //ToDO
    }
}

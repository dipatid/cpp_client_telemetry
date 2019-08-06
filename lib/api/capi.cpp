/* Copyright (c) Microsoft. All rights reserved. */
#include "mat/config.h"
#ifdef _WIN32
#define MATSDK_DECLSPEC __declspec(dllexport)
#endif

#include "LogManagerProvider.hpp"
#include "mat.h"
#include "utils/Utils.hpp"

#include "PAL.hpp"

#include "CommonFields.h"

#include <mutex>
#include <map>
#include <cstdint>

static const char * libSemver = TELEMETRY_EVENTS_VERSION;

using namespace MAT;

static std::mutex mtx;
static std::map<evt_handle_t, capi_client> clients;

/// <summary>
/// Convert from C API handle to internal C API client struct.
///
/// This method may be used for C API flow debugging, i.e. to obtain the
/// underlying instance of ILogManager and attach a debug event callback.
///
/// NOTE: method is not guaranteed to be ABI-stable and should not be used
/// across dynamic / shared library boundary. Underlying ILogManager /
/// ILogConfiguration are implemented in C++ and do not provide ABI-stable
/// guarantee from one SDK version to another.
/// </summary>
capi_client * MAT::capi_get_client(evt_handle_t handle)
{
    LOCKGUARD(mtx);
    const auto it = clients.find(handle);
    return (it != clients.cend()) ? &(it->second) : nullptr;
}

/// <summary>
/// Remove C API handle from active client tracking struct.
/// </summary>
void remove_client(evt_handle_t handle)
{
    LOCKGUARD(mtx);
    clients.erase(handle);
}

#define VERIFY_CLIENT_HANDLE(client, ctx)                       \
    if (ctx==nullptr)                                           \
    {                                                           \
        return EFAULT; /* bad address */                        \
    };                                                          \
    auto client = MAT::capi_get_client(ctx->handle);            \
    if ((client == nullptr) || (client->logmanager == nullptr)) \
    {                                                           \
        return ENOENT;                                          \
    };

evt_status_t mat_open(evt_context_t *ctx)
{
    if (ctx == nullptr)
    {
        return EFAULT; /* bad address */
    };

    char* config = static_cast<char *>(ctx->data);
    if ((config == nullptr) || (config[0] == 0))
    {
        // Invalid configuration
        return EFAULT;
    }

    evt_handle_t code = static_cast<evt_handle_t>(hashCode(config));
    bool isHashFound = false;
    // Find the next available spare hashcode
    do
    {
        auto client = MAT::capi_get_client(code);
        if (client != nullptr)
        {
            if (client->ctx_data == config)
            {
                // Guest instance with the same config is already open
                return EALREADY;
            }
            // hash code is assigned to another client, increment and retry
            // with the next empty slot
            code++;
            continue;
        };
        isHashFound = true;
    } while (!isHashFound);

    // JSON configuration must start with {
    if (config[0] == '{')
    {
        // Create new configuration object from JSON
        clients[code].config = MAT::FromJSON(config);
    }
    else
    {
        // Assume that the config string is a token, not JSON.
        // That approach allows to consume the lightweght C API without JSON parser compiled in.
        std::string moduleName = "CAPI-Client-";
        moduleName += std::to_string(code);
        clients[code].config =
        {
            { "name", moduleName },
            { "version", "1.0.0" },
            { "config",
                {
                    { "host", "*" },
                    { "scope", CONTEXT_SCOPE_NONE }
                }
            },
            { CFG_STR_PRIMARY_TOKEN, config }
        };
    }

    // Remember the original config string. Needed to avoid hash code collisions
    clients[code].ctx_data = config;

    status_t status = static_cast<status_t>(EFAULT);
    clients[code].logmanager = LogManagerProvider::CreateLogManager(clients[code].config, status);

    // Verify that the instance pointer is valid
    if (clients[code].logmanager == nullptr)
    {
        status = static_cast<status_t>(EFAULT);
    }
    ctx->result = static_cast<evt_status_t>(status);
    ctx->handle = code;
    return ctx->result;
}

/**
 * Marashal C struct to C++ API
 */
evt_status_t mat_log(evt_context_t *ctx)
{
    VERIFY_CLIENT_HANDLE(client, ctx);

    ILogConfiguration & config = client->config;
    evt_prop *evt = static_cast<evt_prop*>(ctx->data);
    EventProperties props;
    props.unpack(evt, ctx->size);

    auto m = props.GetProperties();
    EventProperty &prop = m[COMMONFIELDS_IKEY];
    std::string token = prop.as_string;
    props.erase(COMMONFIELDS_IKEY);

    // Privacy feature for OTEL C API client:
    //
    // C API customer that does not explicitly pass down JSON
    //   config["config]["scope"] = COMMONFIELDS_SCOPE_ALL;
    //
    // should not be able to capture the host's context vars.
    std::string scope = CONTEXT_SCOPE_NONE;
    {
        MAT::VariantMap &config_map = config["config"];
        const auto & it = config_map.find("scope");
        if (it != config_map.cend())
        {
            scope = static_cast<const char *>(it->second);
            // Specifying "*" in JSON config allows Guest C API logger to capture Host context variables
            if (scope == CONTEXT_SCOPE_ALL)
            {
                scope = CONTEXT_SCOPE_EMPTY;
            }
        }
    }

    const auto & it = m.find(COMMONFIELDS_EVENT_SOURCE);
    std::string source = ((it != m.cend()) && (it->second.type == EventProperty::TYPE_STRING)) ? it->second.as_string : "";

    ILogger *logger = client->logmanager->GetLogger(token, source, scope);
    if (logger == nullptr)
    {
        ctx->result = EFAULT; /* invalid address */
    }
    else
    {
        // TODO: [MG] - verify guest configuration and decide if we need to overwrite
        logger->SetParentContext(nullptr);
        logger->LogEvent(props);
        ctx->result = EOK;
    }
    return ctx->result;
}

evt_status_t mat_close(evt_context_t *ctx)
{
    VERIFY_CLIENT_HANDLE(client, ctx);
    const auto result = static_cast<evt_status_t>(LogManagerProvider::Release(client->logmanager->GetLogConfiguration()));
    remove_client(ctx->handle);
    ctx->result = result;
    return result;
}

evt_status_t mat_pause(evt_context_t *ctx)
{
    VERIFY_CLIENT_HANDLE(client, ctx);
    const auto result = static_cast<evt_status_t>(client->logmanager->PauseTransmission());
    ctx->result = result;
    return result;
}

evt_status_t mat_resume(evt_context_t *ctx)
{
    VERIFY_CLIENT_HANDLE(client, ctx);
    const auto result = static_cast<evt_status_t>(client->logmanager->ResumeTransmission());
    ctx->result = result;
    return result;
}

evt_status_t mat_upload(evt_context_t *ctx)
{
    VERIFY_CLIENT_HANDLE(client, ctx);
    const auto result = static_cast<evt_status_t>(client->logmanager->UploadNow());
    ctx->result = result;
    return result;
}

evt_status_t mat_flush(evt_context_t *ctx)
{
    VERIFY_CLIENT_HANDLE(client, ctx);
    const auto result = static_cast<evt_status_t>(client->logmanager->Flush());
    ctx->result = result;
    return result;
}

extern "C" {

    /**
     * Simple stable backwards- / forward- compatible ABI interface
     */
    evt_status_t EVTSDK_LIBABI_CDECL evt_api_call_default(evt_context_t *ctx)
    {
        evt_status_t result = EFAIL;

        if (ctx != nullptr)
        {
            switch (ctx->call)
            {
            case EVT_OP_LOAD:
                result = ENOTSUP;
                break;

            case EVT_OP_UNLOAD:
                result = ENOTSUP;
                break;

            case EVT_OP_OPEN:
                result = mat_open(ctx);
                break;

            case EVT_OP_CLOSE:
                result = mat_close(ctx);
                break;

            case EVT_OP_CONFIG:
                result = ENOTSUP;
                break;

            case EVT_OP_LOG:
                result = mat_log(ctx);
                break;

            case EVT_OP_PAUSE:
                result = mat_pause(ctx);
                break;

            case EVT_OP_RESUME:
                result = mat_resume(ctx);
                break;

            case EVT_OP_UPLOAD:
                result = mat_upload(ctx);
                break;

            case EVT_OP_FLUSH:
                result = mat_flush(ctx);
                break;

            case EVT_OP_VERSION:
                // TODO: [MG] - add handling of ctx->data passed by caller inline stub.
                // If there is API version mismatch between the stub and lib impl, then
                // depending on version passed down to SDK - lib may need to figure out
                // how to handle the mismatch. For now the onus of verifying for SDK
                // compatibility is on API caller.

                // Inlined stub version passed by the caller - compiled in executable.
                LOG_TRACE("header  version: %s", ctx->data);

                // Actual SDK library implementation version - compiled in the library.
                ctx->data = (void*)libSemver;
                LOG_TRACE("library version: %s", ctx->data);

                result = STATUS_SUCCESS;
                break;

                // Add more OPs here

            default:
                result = ENOTSUP;
                break;
            }
        }
        return result;
    }
}

// Copyright (c) Microsoft. All rights reserved.
#ifdef _WIN32 /* TODO: [MG] - implement HttpServer for Linux and Mac OS X and re-enable this test */
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif
#include "common/Common.hpp"
#include "common/HttpServer.hpp"

#include "api/LogManagerImpl.hpp"
#include "api/LogManagerFactory.hpp"

#include "bond/All.hpp"
#include "bond/generated/CsProtocol_types.hpp"
#include "bond/generated/CsProtocol_readers.hpp"

#include "sqlite3.h"

using namespace testing;
using namespace MAT;


class MultipleLogManagersTests : public ::testing::Test,
                                 public HttpServer::Callback
{
  protected:
    std::list<HttpServer::Request> receivedRequests;
    std::string                    serverAddress;
    ILogConfiguration              config1, config2;
    HttpServer                     server;

  public:

    virtual void SetUp() override
    {
        int port = server.addListeningPort(0);
        std::ostringstream os;
        os << "localhost:" << port;
        server.setServerName(os.str());
        serverAddress = "http://" + os.str();

        server.addHandler("/1/", *this);
        server.addHandler("/2/", *this);

        server.start();

#if 0
        sqlite3_initialize();
        config1["skipSqliteInitAndShutdown"] = "true";
        config2["skipSqliteInitAndShutdown"] = "true";
#endif

        // Config for instance #1
        config1["cacheFilePath"] = "lm1.db";
        ::remove(config1["cacheFilePath"]);
        config1[CFG_STR_COLLECTOR_URL] = serverAddress + "/1/";
        config1["name"] = "Instance1";
        config1["version"] = "1.0.0";
        config1["config"]["host"] = "Instance1";

        // Config for instance #2
        config2["cacheFilePath"] = "lm2.db";
        ::remove(config2["cacheFilePath"]);
        config2[CFG_STR_COLLECTOR_URL] = serverAddress + "/2/";
        config1["name"] = "Instance2";
        config1["version"] = "1.0.0";
        config1["config"]["host"] = "Instance2"; // host
    }

    virtual void TearDown() override
    {
        sqlite3_shutdown();
        server.stop();
        ::remove(config1["cacheFilePath"]);
        ::remove(config2["cacheFilePath"]);
    }

    virtual int onHttpRequest(HttpServer::Request const& request, HttpServer::Response& response) override
    {
        UNREFERENCED_PARAMETER(response);
        receivedRequests.push_back(request);
        return 200;
    }

    void waitForRequests(unsigned timeout, unsigned expectedCount = 1)
    {
        auto sz = receivedRequests.size();
        auto start = PAL::getUtcSystemTimeMs();
        while (receivedRequests.size() - sz < expectedCount) {
            if (PAL::getUtcSystemTimeMs() - start >= timeout) {
                GTEST_FATAL_FAILURE_("Didn't receive request within given timeout");
            }
            PAL::sleep(100);
        }
    }

/*    CsProtocol::ClientToCollectorRequest decodeRequest(HttpServer::Request const& request)
    {
        std::vector<uint8_t> input(request.content.data(), request.content.data() + request.content.size());
        bond_lite::CompactBinaryProtocolReader reader(input);

        CsProtocol::ClientToCollectorRequest result;
        EXPECT_THAT(bond_lite::Deserialize(reader, result), true);

        return result;
    }
    */
};


TEST_F(MultipleLogManagersTests, TwoInstancesCoexist)
{
    std::unique_ptr<ILogManager> lm1(LogManagerFactory::Create(config1));
    std::unique_ptr<ILogManager> lm2(LogManagerFactory::Create(config2));

    lm1->SetContext("test1", "abc");

    lm2->GetSemanticContext().SetAppId("123");

    ILogger* l1a = lm1->GetLogger("aaa");

    ILogger* l2a = lm2->GetLogger("aaa", "aaa-source");
    EventProperties l2a1p("l2a1");
    l2a1p.SetProperty("x", "y");
    l2a->LogEvent(l2a1p);

    EventProperties l1a1p("l1a1");
    l1a1p.SetProperty("X", "Y");
    l1a->LogEvent(l1a1p);

    ILogger* l1b = lm1->GetLogger("bbb");
    EventProperties l1b1p("l1b1");
    l1b1p.SetProperty("asdf", 1234);
    l1b->LogEvent(l1b1p);

    lm1->GetLogController()->UploadNow();
    lm2->GetLogController()->UploadNow();

    waitForRequests(5000, 2);

    // Add more tests

    lm1.reset();
    lm2.reset();
}
#endif

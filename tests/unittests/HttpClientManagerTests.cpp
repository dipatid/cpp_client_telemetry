// Copyright (c) Microsoft. All rights reserved .

#include "common/Common.hpp"
#include "common/MockIHttpClient.hpp"
#include "http/HttpClientManager.hpp"

using namespace testing;
using namespace ARIASDK_NS;


class HttpClientManager4Test : public HttpClientManager {
  public:
    HttpClientManager4Test(IHttpClient& httpClient)
      : HttpClientManager(httpClient)
    {
    }

    virtual void scheduleOnHttpResponse(HttpCallback* callback) override
    {
        onHttpResponse(callback);
    }
};

class HttpClientManagerTests : public StrictMock<Test> {
  protected:
    MockIHttpClient        httpClientMock;
    HttpClientManager4Test hcm;

    RouteSink<HttpClientManagerTests, EventsUploadContextPtr const&> requestDone{this, &HttpClientManagerTests::resultRequestDone};

  protected:
    HttpClientManagerTests()
      : hcm(httpClientMock)
    {
        hcm.requestDone >> requestDone;
    }

    MOCK_METHOD1(resultRequestDone, void(EventsUploadContextPtr const &));
};


TEST_F(HttpClientManagerTests, HandlesRequestFlow)
{
    std::unique_ptr<SimpleHttpRequest> req(new SimpleHttpRequest("HttpClientManagerTests"));

    auto ctx = EventsUploadContext::create();
    ctx->httpRequestId = req->GetId();
    ctx->httpRequest = std::move(req);
    ctx->recordIds.push_back("r1,r2");
    ctx->latency = EventLatency_Normal;
    ctx->packageIds["tenant1-token"] = 0;

    IHttpResponseCallback* callback = nullptr;
    EXPECT_CALL(httpClientMock, SendRequestAsync(ctx->httpRequest.get(), _))
        .WillOnce(SaveArg<1>(&callback));
    hcm.sendRequest(ctx);
    ASSERT_THAT(callback, NotNull());

    PAL::sleep(200);

    std::unique_ptr<SimpleHttpResponse> rsp(new SimpleHttpResponse("HttpClientManagerTests"));
    rsp->m_result = HttpResult_OK;
    rsp->m_statusCode = 200;

    EXPECT_CALL(*this, resultRequestDone(ctx))
        .WillOnce(Return());
    IHttpResponse const* rspRef = rsp.get();
    callback->OnHttpResponse(rsp.release());

    EXPECT_THAT(ctx->httpResponse.get(), rspRef);
    EXPECT_THAT(ctx->durationMs, Gt(199));
}

TEST_F(HttpClientManagerTests, CancelAbortsRequests)
{
    std::unique_ptr<SimpleHttpRequest> req(new SimpleHttpRequest("HttpClientManagerTests"));

    auto ctx = EventsUploadContext::create();
    ctx->httpRequestId = req->GetId();
    ctx->httpRequest = std::move(req);
    ctx->recordIds.push_back("r1,r2");
    ctx->latency = EventLatency_Normal;
    ctx->packageIds["tenant1-token"] = 0;

    IHttpResponseCallback* callback = nullptr;
    EXPECT_CALL(httpClientMock, SendRequestAsync(ctx->httpRequest.get(), _))
        .WillOnce(SaveArg<1>(&callback));
    hcm.sendRequest(ctx);
    ASSERT_THAT(callback, NotNull());

    EXPECT_CALL(httpClientMock, CancelRequestAsync(Eq("HttpClientManagerTests")))
        .WillOnce(Return());
    EXPECT_THAT(hcm.cancelAllRequestsAsync(), true);

    std::unique_ptr<SimpleHttpResponse> rsp(new SimpleHttpResponse("HttpClientManagerTests"));
    rsp->m_result = HttpResult_Aborted;

    EXPECT_CALL(*this, resultRequestDone(ctx))
        .WillOnce(Return());
    IHttpResponse const* rspRef = rsp.get();
    callback->OnHttpResponse(rsp.release());

    EXPECT_THAT(ctx->httpResponse.get(), rspRef);
}

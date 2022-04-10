#include <unittests.hpp>

#define private public

#include <Request.hpp>

Request *new_request = new Request;

// Good requests

static std::string  basic_get_request =
	"GET /?parameter1=waarde1&parameter2=waarde2&parameter3=waarde3 HTTP/1.1\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Host: localhost\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n\r\n";

static std::string  basic_post_request =
	"POST /index.html HTTP/1.1\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Host: localhost:8082\r\n"
	"Content-Type: text/xml; charset=utf-8\r\n"
	"Content-Length: 35\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n\r\n"
	"<html><h1>Goodbye World</h1></html>";

static std::string  basic_delete_request =
	"DELETE /index.html HTTP/1.1\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Host: localhost\r\n\r\n";


// Bad requests

static std::string  basic_get_request_wrong =
	"GET / HTTP/1.0\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Host: localhost:8080\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n\r\n";

static std::string  basic_post_request_wrong =
	"POST /index.html HTTP/1.1\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Content-Type: text/xml; charset=utf-8\r\n"
	"Content-Length: 35\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n\r\n"
	"<html><h1>Goodbye World</h1></html>";

static std::string  basic_delete_request_wrong =
	"DELETe /index.html HTTP/1.1\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Host: localhost\r\n\r\n";

// Advanced requests

static std::string  advanced_get_request =
	"GET /script.py/html/index.html HTTP/1.1\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Host: localhost\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n\r\n";

// Chunked requests
static std::string chunked_post_request = 
	"POST /test.html HTTP/1.1\r\n"
	"Host: localhost:8082\r\n"
	"Content-Encoding: chunked\r\n\r\n"
	"4\r\n"
	"Wiki\r\n"
	"6\r\n"
	"pedia \r\n"
	"E\r\n"
	"in \r\n\r\n"
	"chunks.\r\n"
	"0\r\n\r\n";

static std::string chunked_post_request2 = 
	"POST /test.html HTTP/1.1\r\n"
	"Host: localhost:8082\r\n"
	"Content-Encoding: chunked\r\n\r\n"
	"B\r\n"
	"Bring this \r\n"
	"5\r\n"
	"chunk\r\n"
	"3\r\n"
	"ed \r\n"
	"6\r\n"
	"to me \r\n"
	"8\r\n"
	"please a\r\n"
	"1\r\n"
	"n\r\n"
	"2\r\n"
	"d \r\n"
	"6\r\n"
	"thank \r\n"
	"3\r\n"
	"you\r\n"
	"0\r\n"
	"\r\n";

static std::string chunked_post_request3 = 
	"POST /test.html HTTP/1.1\r\n"
	"Host: localhost:8082\r\n"
	"Content-Encoding: chunked\r\n\r\n"
	"19\r\n"
	"this is a very long test \r\n"
	"8\r\n"
	"request \r\n"
	"26\r\n"
	"i don't know what to say to be honest.\r\n"
	"0\r\n\r\n";

static std::string empty_chunked_post_request = 
	"POST /test.html HTTP/1.1\r\n"
	"Host: localhost:8082\r\n"
	"Content-Encoding: chunked\r\n\r\n";

// Basic requests that would succeed

class TestBasicRequest : public ::testing::Test
{
public:
    Request req;

protected:
    void SetUp() override
    {
        req.m_request = basic_get_request;
        req.divideRequest();
        req.errorChecking();
    }
};

TEST_F(TestBasicRequest, StartLine)
{
    EXPECT_TRUE(new_request->getMethod() == "GET");
    EXPECT_TRUE(new_request->getTarget() == "/");
    EXPECT_TRUE(new_request->getQuery() == "parameter1=waarde1&parameter2=waarde2&parameter3=waarde3");
    EXPECT_TRUE(new_request->getVersion() == HTTP_VERSION);
    EXPECT_TRUE(new_request->getPort() == 80);
}

TEST_CASE("Basic get request")
{
   new_request->setRequest(basic_get_request);
   new_request->divideRequest();
   new_request->errorChecking();

	SECTION("Start line")
	{
        REQUIRE(new_request->getMethod() == "GET");
        REQUIRE(new_request->getTarget() == "/");
        REQUIRE(new_request->getQuery() == "parameter1=waarde1&parameter2=waarde2&parameter3=waarde3");
        REQUIRE(new_request->getVersion() == ALLOWED_VERSION);
		REQUIRE(new_request->getPort() == 80);
	}

	SECTION("Headers")
	{
       std::map<std::string, std::string> TestMap =
       {
			{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36"},
			{"Host", "localhost"},
			{"Accept-Language", "en-us"},
			{"Accept-Encoding", "gzip, deflate"}
       };
		// REQUIRE(new_request->getHeaders() == TestMap);
	}

	SECTION("Body")
	{
		REQUIRE(new_request->getBody().empty());
	}

	SECTION("Status")
	{
		REQUIRE(new_request->getStatus() == HTTP_STATUS_OK);
	}
}

TEST_CASE("Basic post request")
{
   new_request->setRequest(basic_post_request);
   new_request->divideRequest();
   new_request->errorChecking();

	SECTION("Start line")
	{
        REQUIRE(new_request->getMethod() == "POST");
        REQUIRE(new_request->getTarget() == "/");
		REQUIRE(new_request->getFilename() == "/index.html");
        REQUIRE(new_request->getQuery().empty());
        REQUIRE(new_request->getVersion() == ALLOWED_VERSION);
		REQUIRE(new_request->getPort() == 8082);
	}

	SECTION("Headers")
	{
       std::map<std::string, std::string> TestMap =
       {
			{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36"},
			{"Host", "localhost:8082"},
			{"Content-Type", "text/xml; charset=utf-8"},
			{"Content-Length", "35"},
			{"Accept-Language", "en-us"},
			{"Accept-Encoding", "gzip, deflate"}
       };
		REQUIRE(new_request->getHeaders() == TestMap);
	}

	SECTION("Body")
	{
		REQUIRE(new_request->getBody() == "<html><h1>Goodbye World</h1></html>");
	}

	SECTION("Status")
	{
		REQUIRE(new_request->getStatus() == HTTP_STATUS_OK);
	}
}

TEST_CASE("Basic delete request")
{
   new_request->setRequest(basic_delete_request);
   new_request->divideRequest();
   new_request->errorChecking();

	SECTION("Start line")
	{
        REQUIRE(new_request->getMethod() == "DELETE");
        REQUIRE(new_request->getTarget() == "/");
		REQUIRE(new_request->getFilename() == "/index.html");
        REQUIRE(new_request->getQuery().empty());
        REQUIRE(new_request->getVersion() == ALLOWED_VERSION);
		REQUIRE(new_request->getPort() == 80);
	}

	SECTION("Headers")
	{
       std::map<std::string, std::string> TestMap =
       {
			{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36"},
			{"Host", "localhost"}
       };
	   REQUIRE(new_request->getHeaders() == TestMap);
	}

	SECTION("Body")
	{
		REQUIRE(new_request->getBody().empty());
	}

	SECTION("Status")
	{
		REQUIRE(new_request->getStatus() == HTTP_STATUS_OK);
	}
}

// Basic requests that would fail

TEST_CASE("Wrong basic get request")
{
	new_request->setRequest(basic_get_request_wrong);
	new_request->divideRequest();
	new_request->errorChecking();

	SECTION("Status")
	{
		REQUIRE(new_request->getStatus() == 505);
	}
}

TEST_CASE("Wrong basic post request")
{
	new_request->setRequest(basic_post_request_wrong);
  	new_request->divideRequest();
	new_request->errorChecking();
	if (new_request->getHeaders().find("Host") != new_request->getHeaders().end())
		std::cout << "I FOUND A HEADER WITH HOST: " << new_request->getHeaders().at("Host") << std::endl;;

	SECTION("Status")
	{
		REQUIRE(new_request->getStatus() == 400);
	}
}

TEST_CASE("Wrong basic delete request")
{
	new_request->setRequest(basic_delete_request_wrong);
	new_request->divideRequest();
	new_request->errorChecking();

	SECTION("Status")
	{
		REQUIRE(new_request->getStatus() == 405);
	}
}

TEST_CASE("Advanced get request")
{
	new_request->setRequest(advanced_get_request);
	new_request->divideRequest();
	new_request->errorChecking();

	SECTION("Start line")
	{
        REQUIRE(new_request->getMethod() == "GET");
        REQUIRE(new_request->getTarget() == "/");
		REQUIRE(new_request->getFilename() == "/script.py");
        REQUIRE(new_request->getQuery().empty());
        REQUIRE(new_request->getVersion() == ALLOWED_VERSION);
		REQUIRE(new_request->getCGIPath() == "/html/index.html");
		REQUIRE(new_request->getPort() == 80);
	}
}

TEST_CASE("Chunked post request")
{
	new_request->setRequest(chunked_post_request);
	new_request->divideRequest();
	new_request->errorChecking();

	SECTION("Headers")
	{
        REQUIRE(new_request->getMethod() == "POST");
		REQUIRE(new_request->getPort() == 8082);
	}

	SECTION("Body")
	{
        REQUIRE(new_request->getBody() == "Wikipedia in \r\n\r\nchunks.");
	}
}

TEST_CASE("Chunked post request 2")
{
	new_request->setRequest(chunked_post_request2);
	new_request->divideRequest();
	new_request->errorChecking();

	SECTION("Headers")
	{
        REQUIRE(new_request->getMethod() == "POST");
		REQUIRE(new_request->getPort() == 8082);
	}

	SECTION("Body")
	{
        REQUIRE(new_request->getBody() == "Bring this chunked to me please and thank you");
	}
}

TEST_CASE("Chunked post request 3")
{
	new_request->setRequest(chunked_post_request3);
	new_request->divideRequest();
	new_request->errorChecking();

	SECTION("Headers")
	{
        REQUIRE(new_request->getMethod() == "POST");
		REQUIRE(new_request->getPort() == 8082);
	}

	SECTION("Body")
	{
        REQUIRE(new_request->getBody() == "this is a very long test request i don't know what to say to be honest.");
	}
}

TEST_CASE("Empty chunked post request")
{
	new_request->setRequest(empty_chunked_post_request);
	new_request->divideRequest();
	new_request->errorChecking();

	SECTION("Headers")
	{
        REQUIRE(new_request->getMethod() == "POST");
		REQUIRE(new_request->getPort() == 8082);
	}

	SECTION("Body")
	{
        REQUIRE(new_request->getBody().empty());
	}
}


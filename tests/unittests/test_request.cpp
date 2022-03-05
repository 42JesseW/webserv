#include <catch.hpp>
#include <Request.hpp>

Request new_request;

// Good requests

static std::string  basic_get_request =
	"GET /?parameter1=waarde1&parameter2=waarde2&parameter3=waarde3 HTTP/1.1\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Host: localhost\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate";

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
	"Accept-Encoding: gzip, deflate";

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

// Basic requests that would succeed

TEST_CASE("Basic get request")
{
   new_request.setRequest(basic_get_request);

	SECTION("Start line")
	{
        REQUIRE(new_request.getMethod() == "GET");
        REQUIRE(new_request.getTarget() == "/");
        REQUIRE(new_request.getQuery() == "parameter1=waarde1&parameter2=waarde2&parameter3=waarde3");
        REQUIRE(new_request.getVersion() == ALLOWED_VERSION);
		REQUIRE(new_request.getPort() == 80);
	}

	SECTION("Headers")
	{
       std::map<std::string, std::string> TestMap =
       {
			{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36"},
			{"Host", "localhost:8082"},
			{"Accept-Language", "en-us"},
			{"Accept-Encoding", "gzip, deflate"}
       };
		REQUIRE(new_request.getHeaders() == TestMap);
	}

	SECTION("Body")
	{
		REQUIRE(new_request.getBody().empty());
	}

	SECTION("Status")
	{
		REQUIRE(new_request.getStatus() == 200);
	}
}

TEST_CASE("Basic post request")
{
   new_request.setRequest(basic_get_request);

	SECTION("Start line")
	{
        REQUIRE(new_request.getMethod() == "POST");
        REQUIRE(new_request.getTarget() == "/index.html");
        REQUIRE(new_request.getQuery().empty());
        REQUIRE(new_request.getVersion() == ALLOWED_VERSION);
		REQUIRE(new_request.getPort() == 8082);
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
		REQUIRE(new_request.getHeaders() == TestMap);
	}

	SECTION("Body")
	{
		REQUIRE(new_request.getBody() == "<html><h1>Goodbye World</h1></html>");
	}

	SECTION("Status")
	{
		REQUIRE(new_request.getStatus() == 200);
	}
}

TEST_CASE("Basic delete request")
{
   new_request.setRequest(basic_get_request);

	SECTION("Start line")
	{
        REQUIRE(new_request.getMethod() == "DELETE");
        REQUIRE(new_request.getTarget() == "/index.html");
        REQUIRE(new_request.getQuery().empty());
        REQUIRE(new_request.getVersion() == ALLOWED_VERSION);
		REQUIRE(new_request.getPort() == 80);
	}

	SECTION("Headers")
	{
       std::map<std::string, std::string> TestMap =
       {
			{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36"},
			{"Host", "localhost:8082"}
       };
		REQUIRE(new_request.getHeaders() == TestMap);
	}

	SECTION("Body")
	{
		REQUIRE(new_request.getBody().empty());
	}

	SECTION("Status")
	{
		REQUIRE(new_request.getStatus() == 200);
	}
}

// Basic requests that would fail

TEST_CASE("Wrong basic get request")
{
	new_request.setRequest(basic_get_request);

	SECTION("Status")
	{
		REQUIRE(new_request.getStatus() == 400);
	}
}

TEST_CASE("Wrong basic post request")
{
	new_request.setRequest(basic_get_request);

	SECTION("Status")
	{
		REQUIRE(new_request.getStatus() == 400);
	}
}

TEST_CASE("Wrong basic delete request")
{
	new_request.setRequest(basic_get_request);

	SECTION("Status")
	{
		REQUIRE(new_request.getStatus() == 400);
	}
}
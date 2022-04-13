# include <catch.hpp>
# include <Client.hpp>
# include <Request.hpp>
# include <Route.hpp>
# include <Response.hpp>
# include <vector>
# include <map>
# include <string>
# include <ctime>
# include <time.h>

// Good requests

static std::string basic_get_request =
	"GET /?parameter1=waarde1&parameter2=waarde2&parameter3=waarde3 HTTP/1.1\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Host: localhost\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n\r\n";

static std::string basic_post_request =
	"POST /index.html HTTP/1.1\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Host: localhost:8082\r\n"
	"Content-Type: text/xml; charset=utf-8\r\n"
	"Content-Length: 35\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n\r\n"
	"<html><h1>Goodbye World</h1></html>";

static std::string basic_delete_request =
	"DELETE /index.html HTTP/1.1\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Host: localhost\r\n\r\n";

// Bad requests

static std::string basic_get_request_wrong =
	"GET / HTTP/1.0\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Host: localhost:8080\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n\r\n";

static std::string basic_post_request_wrong =
	"POST /index.html HTTP/1.1\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Content-Type: text/xml; charset=utf-8\r\n"
	"Content-Length: 35\r\n"
	"Accept-Language: en-us\r\n"
	"Accept-Encoding: gzip, deflate\r\n\r\n"
	"<html><h1>Goodbye World</h1></html>";

static std::string basic_delete_request_wrong =
	"DELETe /index.html HTTP/1.1\r\n"
	"User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36\r\n"
	"Host: localhost\r\n\r\n";

// HTML files

std::string index_html =  "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n    <meta charset=\"UTF-8\">\n\
    <meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n\
    <title>Document</title>\n</head>\n<body>\n    <h1>Hello, welcome to our website.</h1>\n</body>\n</html>";

// Basic requests that would succeed

TEST_CASE("Basic get request -- basic config file with no redirection")
{
	Client	new_client;
	Request	new_request;
	Route	new_route;

	std::string m_base_url = "/";
	std::string m_file_search_path = "/html";
	std::string m_upload_path = "/html";
	bool m_has_autoindex = true;

	std::vector<std::string> m_accepted_methods;
	m_accepted_methods.push_back("GET");
	m_accepted_methods.push_back("POST");
	m_accepted_methods.push_back("DELETE");

	std::vector<std::string> m_index_files;
	m_index_files.push_back("index.html");
	m_index_files.push_back("index.php");

	std::vector<std::string> m_cgi_file_extensions;
	m_cgi_file_extensions.push_back(".php");
	m_cgi_file_extensions.push_back(".py");

	new_route.setBaseUrl(m_base_url);
	new_route.setFileSearchPath(m_file_search_path);
	new_route.setUploadPath(m_upload_path);
	new_route.setAutoIndex(m_has_autoindex);
	new_route.setAcceptedMethods(m_accepted_methods);
	new_route.setIndexFiles(m_index_files);
	new_route.setCgiFileExtensions(m_cgi_file_extensions);

    new_client.m_request = new_request;
	new_client.m_route = new_route;
	new_client.m_request.setRequest(basic_get_request);
	new_client.m_request.divideRequest();
	new_client.m_request.errorChecking();
    new_client.checkRoute();

    std::map<int, std::pair<std::string, std::string>> m_error_files = ConfigUtil::getHandle().getStatusCodeMap();
	Response new_response(new_client.m_request, new_client.m_route);

	new_response.buildResponse(m_error_files);

	SECTION("Start line")
	{
		REQUIRE(new_response.getStartLine() == "HTTP/1.1 200 OK\r\n");
	}

	SECTION("Headers")
	{
		std::string test_date;
		std::string test_content_length;
		int			content_length;

		content_length = new_response.getBody().length();
		test_content_length = std::to_string(content_length);

		std::time_t		rawtime;
		struct std::tm	*ptm;
		char			buf[50];

		time(&rawtime);
		ptm = gmtime(&rawtime);
		strftime(buf, 500, "%a, %d %b %Y %H:%M:%S GMT", ptm);
		test_date = std::string(buf);

		std::map<std::string, std::string> TestMap =
		{
			{"Date", test_date},
			{"Server", "Websurf/1.0.0 (Unix)"},
			{"Connection", "close"},
			{"Content-Length", test_content_length},
			{"Content-Type", "text/html; charset=\"utf-8\""},
	   };
	   REQUIRE(new_response.getHeadersMap() == TestMap);
	}

	SECTION("Body")
	{
		REQUIRE(new_response.getBody() == index_html); 
	}
}

TEST_CASE("Basic get request -- basic config file with redirection")
{
	Client	new_client;
	Request	new_request;
	Route	new_route;

	std::string m_base_url = "/";
	std::string m_file_search_path = "/data/w3";
	std::string m_upload_path = "/data/upload";
	bool m_has_autoindex = true;

	std::vector<std::string> m_accepted_methods;
	m_accepted_methods.push_back("GET");
	m_accepted_methods.push_back("POST");
	m_accepted_methods.push_back("DELETE");

	std::vector<std::string> m_index_files;
	m_index_files.push_back("index.html");
	m_index_files.push_back("index.php");

	std::vector<std::string> m_cgi_file_extensions;
	m_cgi_file_extensions.push_back(".php");
	m_cgi_file_extensions.push_back(".py");

	new_route.setBaseUrl(m_base_url);
	new_route.setFileSearchPath(m_file_search_path);
	new_route.setUploadPath(m_upload_path);
	new_route.setAutoIndex(m_has_autoindex);
	new_route.setRedirect(301, "www.google.com");
	new_route.setAcceptedMethods(m_accepted_methods);
	new_route.setIndexFiles(m_index_files);
	new_route.setCgiFileExtensions(m_cgi_file_extensions);

    new_client.m_request = new_request;
	new_client.m_route = new_route;
	new_client.m_request.setRequest(basic_get_request);
	new_client.m_request.divideRequest();
	new_client.m_request.errorChecking();
    new_client.checkRoute();

    std::map<int, std::pair<std::string, std::string>> m_error_files = ConfigUtil::getHandle().getStatusCodeMap();
	Response new_response(new_client.m_request, new_client.m_route);

	new_response.buildResponse(m_error_files);

	SECTION("Start line")
	{
		REQUIRE(new_response.getStartLine() == "HTTP/1.1 301 Moved Permanently\r\n");
	}

	SECTION("Headers")
	{
		std::string test_date;
		std::string test_content_length;
		int			content_length;

		content_length = new_response.getBody().length();
		test_content_length = std::to_string(content_length);

		std::time_t		rawtime;
		struct std::tm	*ptm;
		char			buf[50];

		time(&rawtime);
		ptm = gmtime(&rawtime);
		strftime(buf, 500, "%a, %d %b %Y %H:%M:%S GMT", ptm);
		test_date = std::string(buf);

		std::map<std::string, std::string> TestMap =
		{
			{"Date", test_date},
			{"Location", "www.google.com"},
			{"Server", "Websurf/1.0.0 (Unix)"},
			{"Connection", "close"},
			{"Content-Length", test_content_length},
			{"Content-Type", "text/html; charset=\"utf-8\""},
	   };
	   std::map<std::string, std::string>::iterator iter;
	   std::string test_headers_str;

	   std::cout << "-------------" << std::endl;
	   std::cout << "TEST HEADERS : " << std::endl;
	   for (iter = TestMap.begin(); iter != TestMap.end(); iter++)
		test_headers_str += (iter->first + ": " + iter->second + "/n");
	   std::cout << test_headers_str << std::endl;
	   std::cout << "-------------" << std::endl;

	   REQUIRE(new_response.getHeadersMap() == TestMap);
	}

	SECTION("Body")
	{
		REQUIRE(new_response.getBody().empty());
	}
}

// TEST_CASE("Basic post request")
// {
//    new_request->setRequest(basic_post_request);
//    new_request->divideRequest();
//    new_request->errorChecking();

// 	SECTION("Start line")
// 	{
//         REQUIRE(new_request->getMethod() == "POST");
//         REQUIRE(new_request->getTarget() == "/");
// 		REQUIRE(new_request->getFilename() == "/index.html");
//         REQUIRE(new_request->getQuery().empty());
//         REQUIRE(new_request->getVersion() == ALLOWED_VERSION);
// 		REQUIRE(new_request->getPort() == 8082);
// 	}

// 	SECTION("Headers")
// 	{
//        std::map<std::string, std::string> TestMap =
//        {
// 			{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36"},
// 			{"Host", "localhost:8082"},
// 			{"Content-Type", "text/xml; charset=utf-8"},
// 			{"Content-Length", "35"},
// 			{"Accept-Language", "en-us"},
// 			{"Accept-Encoding", "gzip, deflate"}
//        };
// 		REQUIRE(new_request->getHeaders() == TestMap);
// 	}

// 	SECTION("Body")
// 	{
// 		REQUIRE(new_request->getBody() == "<html><h1>Goodbye World</h1></html>");
// 	}

// 	SECTION("Status")
// 	{
// 		REQUIRE(new_request->getStatus() == HTTP_STATUS_OK);
// 	}
// }

// TEST_CASE("Basic delete request")
// {
//    new_request->setRequest(basic_delete_request);
//    new_request->divideRequest();
//    new_request->errorChecking();

// 	SECTION("Start line")
// 	{
//         REQUIRE(new_request->getMethod() == "DELETE");
//         REQUIRE(new_request->getTarget() == "/");
// 		REQUIRE(new_request->getFilename() == "/index.html");
//         REQUIRE(new_request->getQuery().empty());
//         REQUIRE(new_request->getVersion() == ALLOWED_VERSION);
// 		REQUIRE(new_request->getPort() == 80);
// 	}

// 	SECTION("Headers")
// 	{
//        std::map<std::string, std::string> TestMap =
//        {
// 			{"User-Agent", "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36"},
// 			{"Host", "localhost"}
//        };
// 	   REQUIRE(new_request->getHeaders() == TestMap);
// 	}

// 	SECTION("Body")
// 	{
// 		REQUIRE(new_request->getBody().empty());
// 	}

// 	SECTION("Status")
// 	{
// 		REQUIRE(new_request->getStatus() == HTTP_STATUS_OK);
// 	}
// }

// // Basic requests that would fail

TEST_CASE("Wrong basic get request ERROR 505 -- basic config file with no redirection")
{
	Client	new_client;
	Request	new_request;
	Route	new_route;

	std::string m_base_url = "/";
	std::string m_file_search_path = "/html";
	std::string m_upload_path = "/html";
	bool m_has_autoindex = true;

	std::vector<std::string> m_accepted_methods;
	m_accepted_methods.push_back("GET");
	m_accepted_methods.push_back("POST");
	m_accepted_methods.push_back("DELETE");

	std::vector<std::string> m_index_files;
	m_index_files.push_back("index.html");
	m_index_files.push_back("index.php");

	std::vector<std::string> m_cgi_file_extensions;
	m_cgi_file_extensions.push_back(".php");
	m_cgi_file_extensions.push_back(".py");

	new_route.setBaseUrl(m_base_url);
	new_route.setFileSearchPath(m_file_search_path);
	new_route.setUploadPath(m_upload_path);
	new_route.setAutoIndex(m_has_autoindex);
	new_route.setRedirect(0, "null");
	new_route.setAcceptedMethods(m_accepted_methods);
	new_route.setIndexFiles(m_index_files);
	new_route.setCgiFileExtensions(m_cgi_file_extensions);

    new_client.m_request = new_request;
	new_client.m_route = new_route;
	new_client.m_request.setRequest(basic_get_request_wrong);
	new_client.m_request.divideRequest();
	new_client.m_request.errorChecking();
    new_client.checkRoute();

    std::map<int, std::pair<std::string, std::string>> m_error_files = ConfigUtil::getHandle().getStatusCodeMap();
	Response new_response(new_client.m_request, new_client.m_route);

	new_response.buildResponse(m_error_files);

	SECTION("Start line")
	{
		REQUIRE(new_response.getStartLine() == "HTTP/1.1 505 HTTP Version Not Supported\r\n");
	}

	SECTION("Headers")
	{
		std::string test_date;
		std::string test_content_length;
		int			content_length;

		content_length = new_response.getBody().length();
		test_content_length = std::to_string(content_length);

		std::time_t		rawtime;
		struct std::tm	*ptm;
		char			buf[50];

		time(&rawtime);
		ptm = gmtime(&rawtime);
		strftime(buf, 500, "%a, %d %b %Y %H:%M:%S GMT", ptm);
		test_date = std::string(buf);

		std::map<std::string, std::string> TestMap =
		{
			{"Date", test_date},
			{"Server", "Websurf/1.0.0 (Unix)"},
			{"Connection", "close"},
			{"Content-Length", test_content_length},
			{"Content-Type", "text/html; charset=\"utf-8\""},
	   };
	   REQUIRE(new_response.getHeadersMap() == TestMap);
	}

	SECTION("Body")
	{
		// CHANGE THAT
		REQUIRE(new_response.getBody().empty());
	}
}

// TEST_CASE("Wrong basic post request")
// {
// 	new_request->setRequest(basic_post_request_wrong);
//   	new_request->divideRequest();
// 	new_request->errorChecking();
// 	if (new_request->getHeaders().find("Host") != new_request->getHeaders().end())
// 		std::cout << "I FOUND A HEADER WITH HOST: " << new_request->getHeaders().at("Host") << std::endl;;

// 	SECTION("Status")
// 	{
// 		REQUIRE(new_request->getStatus() == 400);
// 	}
// }

// TEST_CASE("Wrong basic delete request")
// {
// 	new_request->setRequest(basic_delete_request_wrong);
// 	new_request->divideRequest();
// 	new_request->errorChecking();

// 	SECTION("Status")
// 	{
// 		REQUIRE(new_request->getStatus() == 405);
// 	}
// }
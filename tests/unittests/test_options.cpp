#include <catch.hpp>
#include <Config.hpp>

#include <fstream>

static std::string  basic_conf = "http {\n"
     "\tserver {\n"
     "\t\tlisten          *:8082\n"
     "\t\tserver_name     example.org www.example.org\n"
     "        client_max_body_size 10\n"
     "        error_page 400 /full/path/to/file.html\n"
     "        location /home {\n"
     "            allowed_methods GET POST DELETE\n"
     "            root            /data/w3\n"
     "            autoindex       on\n"
     "            index           index.html index.htm\n"
     "            cgi_extensions  .php .py\n"
     "            upload_path     /data/upload\n"
     "            return          301 www.google.com\n"
     "        }\n"
     "\t}\n"
     "}";

TEST_CASE("Config::Option::parse basic input all")
{
    Config                      *config;
    Server                      *server;
    Route                       *route;
    std::string                 file_name("DEFAULT");
    std::vector<std::string>    names = {"example.org", "www.example.org"};
    std::vector<std::string>    methods = {"GET", "POST", "DELETE"};
    std::vector<std::string>    index = {"index.html", "index.htm"};
    std::vector<std::string>    cgi = {".php", ".py"};

    std::remove(file_name.c_str());
    std::fstream    file(file_name, std::ios::in | std::ios::out | std::ios::app );

    file << basic_conf;
    file.close();
    config = &Config::getHandle();
    config->setFilePath(file_name);
    REQUIRE(config->getFilePath() == file_name);
    REQUIRE_NOTHROW(config->loadFile());
    REQUIRE(config->getServers().size() == 1);
    for (int idx = 0; idx < config->getServers().size(); ++idx)
    {
        server = &config->getServers().at(idx);
        CHECK(server->getSockFd() != SOCK_FD_EMPTY);
        CHECK(server->getNames() == names);
        // TODO REQUIRE(server->getErrorFiles())
        CHECK(server->getClientMaxBodySize() == 10);
        REQUIRE(server->getRoutes().size() == 1);
        for (int i = 0; i < server->getRoutes().size(); ++i)
        {
            route = &server->getRoutes().at(i);
            CHECK(route->getBaseUrl() == "/home");
            CHECK(route->getAcceptedMethods() == methods);
            CHECK(route->getRedir()->status_code == 301);
            CHECK(route->getRedir()->url == "www.google.com");
            CHECK(route->getFileSearchPath() == "/data/w3");
            CHECK(route->getUploadPath() == "/data/upload");
            CHECK(route->hasAutoIndex());
            CHECK(route->getIndexFiles() == index);
            CHECK(route->getCgiFileExtensions() == cgi);
        }
    }
}

/*
 * ( Check restraints )
 * - listen only ones
 * - overwriting previous directives
 *   - server_name
 *   - client_max_body_size
 *   - allowed_methods
 *   - root
 *   - autoindex
 *   - index
 *   - cgi_extensions
 *   - upload_path
 *   - return
 * - error_page overwrite pair.second in map
 */

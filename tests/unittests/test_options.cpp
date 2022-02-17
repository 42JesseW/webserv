#include <catch.hpp>
#include <Config.hpp>

#include <fstream>

static std::string  basic_conf = ""
     "http {\n"
     "\tserver {\n"
     "\t\tlisten          *:8082\n"
     "\t\tserver_name     example.org www.example.org\n"
     "        client_max_body_size 10\n"
     "        error_page 400 /full/path/to/file.html\n"
     "        location / {\n"
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
    Config          *config;
    Server          *server;
    Route           *route;
    std::string     file_name("DEFAULT");
    std::fstream    file(file_name, std::ios::in | std::ios::out | std::ios::app);

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
        REQUIRE(server->getNames().size() == 1);
        CHECK(server->getNames().at(0) == DFL_SERVER_NAME);
        // TODO REQUIRE(server->getErrorFiles())
        CHECK(server->getClientMaxBodySize() == DFL_MAX_BODY_SIZE);
        REQUIRE(server->getRoutes().size() == 1);
        for (int i = 0; i < server->getRoutes().size(); ++i)
        {
            route = &server->getRoutes().at(i);
            CHECK(route->getBaseUrl() == "/base");
            CHECK(route->getAcceptedMethods() == Config::getDefaultMethods());
            CHECK(route->getRedir() == NULL);
            CHECK(route->getFileSearchPath() == DFL_FILE_SEARCH_PATH);
            CHECK(route->hasAutoIndex() == false);
            CHECK(route->getIndexFiles().size() == 1);
            CHECK(route->getIndexFiles().at(0) == DFL_INDEX_FILE);
            CHECK(route->getCgiFileExtensions().empty());
        }
    }
    std::remove(file_name.c_str());
}

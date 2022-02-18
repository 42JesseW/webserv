#include <catch.hpp>
#include <Config.hpp>

#include <utility>
#include <fstream>
#include <sys/stat.h>

#define SYS_ERROR (-1)

static std::string  basic_conf = ""
             "# start the configuration file. Only one http block is allowed\n"
             "http {\n"
             "\n"
             "\t# define a server block\n"
             "\tserver {\n"
             "\n"
             "\t\t# address to listen on in the form <address>:<port>. Only one listen is allowed\n"
             "\t\tlisten          *:8082\n"
             "\n"
             "\t\t# server_name(s) used for matching server using \"Host\" Request header\n"
             "\t\tserver_name     example.org www.example.org\n"
             "\n"
             "        # describes the maximum body size of a request in MB\n"
             "        client_max_body_size 10\n"
             "\n"
             "        # error page in the form \"error_page <http_error_number> <full_path_to_file>.html\"\n"
             "        error_page 400 /full/path/to/file.html\n"
             "\n"
             "        # setup a location with a base_pathname to match incoming request\n"
             "        location / {\n"
             "\n"
             "            # HTTP methods allowed for this (only GET POST DELETE are allowed)\n"
             "            allowed_methods GET POST DELETE\n"
             "\n"
             "            # sets the file_path to start looking for requested files\n"
             "            root            /data/w3\n"
             "\n"
             "            # provide directory listing if requested file is not found (either \"on\" or \"off\")\n"
             "            autoindex       on\n"
             "\n"
             "            # default file to answer if the request is a directory\n"
             "            index           index.html index.htm\n"
             "\n"
             "            # possible file extensions for which to pass to CGI\n"
             "            cgi_extensions  .php .py\n"
             "\n"
             "            # file_path of where to upload files. When set will allow uploading files\n"
             "            upload_path     /data/upload\n"
             "\n"
             "            # return a redirect in the form \"return 3<xx> <redirect_url>\"\n"
             "            return          301 www.google.com\n"
             "        }\n"
             "\t}\n"
             "}";

TEST_CASE("Default handle")
{
    CHECK(Config::getHandle().getServers().empty());
}

TEST_CASE(".loadFile() with default m_file_path")
{
    CHECK_THROWS(Config::getHandle().loadFile());
}

TEST_CASE(".loadFile() with file that does not exists")
{
    Config::getHandle().setFilePath("FILE_THAT_DOES_NOT_EXIST");
    CHECK_THROWS(Config::getHandle().loadFile());
}

TEST_CASE(".loadFile() with file that has invalid permissions")
{
    std::string     file_name("INVALID_FILE");

    std::remove(file_name.c_str());
    std::ofstream   invalid_file(file_name);

    REQUIRE(chmod(file_name.c_str(), 0000) != SYS_ERROR);
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() with file that is valid [COMMENTS]")
{
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);

    brackets_file << basic_conf;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_NOTHROW(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() with file that is valid [CLEAN]")
{
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
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

    REQUIRE(brackets_file.good());
    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_NOTHROW(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

/*
 * ( Check invalid brackets )
 */
TEST_CASE(".loadFile() with file that has invalid brackets [1]")
{
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
        "http {\n"
        "    server {\n"
        "    \n"
        "}";

    REQUIRE(brackets_file.good());
    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() with file that has invalid brackets [2]")
{
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
        "http {\n"
        "    server {\n"
        "\n"
        "        }\n"
        "    }\n"
        "}";

    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() with file that has invalid brackets [3]")
{
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
        "http {\n"
        "    server {\n"
        "\n"
        "            }\n"
        "        }\n"
        "    {\n"
        "    }\n"
        "}";

    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

/*
 * ( Check with wrong directives )
 * - Non-existing directives
 * - Directives in the wrong level
 * - Directives with invalid amount of arguments
 * - Directives with invalid argument type
 */

TEST_CASE(".loadFile() with directives in the wrong place [BASE]")
{
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
        "listen  *:8083\n"
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

    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() with directives in the wrong place [HTTP]")
{
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
        "http {\n"
        "\tlisten  *:8083\n"
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

    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() with directives in the wrong place [SERVER]")
{
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
            "http {\n"
            "\tserver {\n"
            "\t\tlisten          *:8082\n"
            "\t\tserver_name     example.org www.example.org\n"
            "        client_max_body_size 10\n"
            "        root            on\n"
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

    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() with directives in the wrong place [LOCATION]")
{
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
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
        "            server_name     lol.com\n"
        "            cgi_extensions  .php .py\n"
        "            upload_path     /data/upload\n"
        "            return          301 www.google.com\n"
        "        }\n"
        "\t}\n"
        "}";

    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

/*
 * ( Check defaults )
 */
TEST_CASE(".loadFile() using only defaults")
{
    Config          *config;
    Server          *server;
    Route           *route;
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
        "http {\n"
        "\tserver {\n"
        "        location /base {\n"
        "        }\n"
        "\t}\n"
        "}";

    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    config = &Config::getHandle();
    config->setFilePath(file_name);
    REQUIRE(config->getFilePath() == file_name);
    CHECK_NOTHROW(config->loadFile());
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
            CHECK(route->getAcceptedMethods() == ConfigUtil::getHandle().getDefaultMethods());
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

/*
 * ( Check missing directives without defaults )
 * - http is required
 * - Only one http block is allowed
 * - http block needs at least 1 server
 * - server needs at least 1 route
 */
TEST_CASE(".loadFile() http is required")
{
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
        "server {\n"
        "    location / {\n"
        "    }\n"
        "}";

    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() only one http block is allowed")
{
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
        "http {\n"
        "\tserver {\n"
        "        location / {\n"
        "        }\n"
        "\t}\n"
        "}\n"
        "\n"
        "http {\n"
        "\tserver {\n"
        "        location / {\n"
        "        }\n"
        "\t}\n"
        "}";

    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() http block requires at least one server")
{
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
        "http {\n"
        "\t\n"
        "}";

    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

TEST_CASE(".loadFile() server block requires at least one location")
{
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     invalid_bracket_file_data = ""
        "http {\n"
        "\tserver {\n"
        "        \n"
        "\t}\n"
        "}";

    brackets_file << invalid_bracket_file_data;
    brackets_file.close();
    Config::getHandle().setFilePath(file_name);
    CHECK_THROWS(Config::getHandle().loadFile());
    std::remove(file_name.c_str());
}

/*
 * ( Check multiple route within server )
 */
TEST_CASE(".loadFile() using only defaults multiple routes")
{
    Config                      *config;
    Server                      *server;
    Route                       *route;
    std::string                 file_name("BRACKETS_FILE");
    std::vector<std::string>    postfix = {"one", "two", "three", "four"};

    std::remove(file_name.c_str());
    std::fstream                brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string                 file_data = ""
        "http {\n"
        "\tserver {\n"
        "        listen\t8085"
        "        location /one {\n"
        "        }\n"
        "        location /two {\n"
        "        }\n"
        "        location /three {\n"
        "        }\n"
        "        location /four {\n"
        "        }\n"
        "\t}\n"
        "}";

    brackets_file << file_data;
    brackets_file.close();
    config = &Config::getHandle();
    config->setFilePath(file_name);
    REQUIRE(config->getFilePath() == file_name);
    CHECK_NOTHROW(config->loadFile());
    REQUIRE(config->getServers().size() == 1);
    for (int idx = 0; idx < config->getServers().size(); ++idx)
    {
        server = &config->getServers().at(idx);
        CHECK(server->getSockFd() != SOCK_FD_EMPTY);
        REQUIRE(server->getNames().size() == 1);
        CHECK(server->getNames().at(0) == DFL_SERVER_NAME);
        // TODO REQUIRE(server->getErrorFiles())
        CHECK(server->getClientMaxBodySize() == DFL_MAX_BODY_SIZE);
        REQUIRE(server->getRoutes().size() == 4);
        for (int i = 0; i < server->getRoutes().size(); ++i)
        {
            route = &server->getRoutes().at(i);
            CHECK(route->getBaseUrl() == ("/" + postfix[i]));
            CHECK(route->getAcceptedMethods() == ConfigUtil::getHandle().getDefaultMethods());
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

/*
 * ( Check multiple server blocks )
 */
TEST_CASE(".loadFile() using only defaults multiple servers")
{
    Config          *config;
    Server          *server;
    Route           *route;
    std::string     file_name("BRACKETS_FILE");

    std::remove(file_name.c_str());
    std::fstream    brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string     file_data = ""
        "http {\n"
        "\tserver {\n"
        "\t    listen  8080\n"
        "        location /base {\n"
        "        }\n"
        "\t}\n"
        "    server {\n"
        "        listen  8081\n"
        "        location /base {\n"
        "        }\n"
        "    }\n"
        "    server {\n"
        "        listen  8082\n"
        "        location /base {\n"
        "        }\n"
        "    }\n"
        "}";

    brackets_file << file_data;
    brackets_file.close();
    config = &Config::getHandle();
    config->setFilePath(file_name);
    REQUIRE(config->getFilePath() == file_name);
    CHECK_NOTHROW(config->loadFile());
    REQUIRE(config->getServers().size() == 3);
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
            CHECK(route->getAcceptedMethods() == ConfigUtil::getHandle().getDefaultMethods());
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

TEST_CASE(".loadFile() using only defaults multiple servers and routes")
{
    Config                      *config;
    Server                      *server;
    Route                       *route;
    std::string                 file_name("BRACKETS_FILE");
    std::vector<std::string>    postfix = {"one", "two", "three", "four"};

    std::remove(file_name.c_str());
    std::fstream                brackets_file(file_name, std::ios::in | std::ios::out | std::ios::app);
    std::string                 file_data = ""
        "http {\n"
        "\tserver {\n"
        "\t    listen  8080\n"
        "        location /one {\n"
        "        }\n"
        "        location /two {\n"
        "        }\n"
        "        location /three {\n"
        "        }\n"
        "\t}\n"
        "    server {\n"
        "        listen  8081\n"
        "        location /one {\n"
        "        }\n"
        "        location /two {\n"
        "        }\n"
        "    }\n"
        "    server {\n"
        "        listen  8082\n"
        "        location /one {\n"
        "        }\n"
        "    }\n"
        "}";

    brackets_file << file_data;
    brackets_file.close();
    config = &Config::getHandle();
    config->setFilePath(file_name);
    REQUIRE(config->getFilePath() == file_name);
    CHECK_NOTHROW(config->loadFile());
    REQUIRE(config->getServers().size() == 3);
    for (int idx = 0; idx < config->getServers().size(); ++idx)
    {
        server = &config->getServers().at(idx);
        CHECK(server->getSockFd() != SOCK_FD_EMPTY);
        REQUIRE(server->getNames().size() == 1);
        CHECK(server->getNames().at(0) == DFL_SERVER_NAME);
        // TODO REQUIRE(server->getErrorFiles())
        CHECK(server->getClientMaxBodySize() == DFL_MAX_BODY_SIZE);
        REQUIRE(server->getRoutes().size() == (config->getServers().size() - idx));
        for (int i = 0; i < server->getRoutes().size(); ++i)
        {
            route = &server->getRoutes().at(i);
            CHECK(route->getBaseUrl() == ("/" + postfix[i]));
            CHECK(route->getAcceptedMethods() == ConfigUtil::getHandle().getDefaultMethods());
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

/*
 * ( Check conflicting blocks)
 * - server blocks with same ports
 * - server blocks with same server_names
 * - location blocks with same base_url
 */


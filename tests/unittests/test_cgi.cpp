#include <catch.hpp>
#include <sys/poll.h>
#ifdef __linux__
# include <sys/wait.h>
# include <limits.h>
#endif

#define private         public
#define POLL_NO_TIMEOUT 0
#define BUFF_TEST_SIZE  1024

#include <CGI.hpp>

const std::string basic_post = ""
       "POST /test?field1=10&field2=20 HTTP/1.1\r\n"
       "\r\n"
       "Host: foo.example\r\n"
       "Content-Type: application/x-www-form-urlencoded\r\n"
       "Content-Length: 15\r\n"
       "\r\n"
       "num1=10&num2=20";

TEST_CASE("parse POST")
{
    SimpleRequest   request;

    request.parse(basic_post.c_str());
}

TEST_CASE("simple CGI init")
{
    CGI             simple_cgi;
    SimpleRequest   request;

    request.parse(basic_post.c_str());
    simple_cgi.setProgramPath("../../tests/unittests/cgi-bin/sleep.py");
    simple_cgi.init(request);
    REQUIRE(simple_cgi.m_environ["CONTENT_TYPE"] == request.m_headers["Content-Type"]);
    REQUIRE(simple_cgi.m_environ["CONTENT_LENGTH"] == request.m_headers["Content-Length"]);
}

/*
 * ( Check failing CGI )
 * - Invalid program_path
 * - Invalid script
 */

TEST_CASE("simple CGI exec [sleep]")
{
    CGI             simple_cgi;
    SimpleRequest   request;
    struct pollfd   pollfds[1];

    char            buff[BUFF_TEST_SIZE];
    char            cwd[PATH_MAX];
    std::string     full_program_path;
    const std::string   out = ""
      "Content-type:text/html\r\n\r\n\n"
      "<h1>Feeling sleepy...</h1>\n";

    getcwd(cwd, PATH_MAX);
    full_program_path += cwd;
    full_program_path += "/../../tests/unittests/cgi-bin/sleep.py";
    request.parse(basic_post.c_str());
    simple_cgi.setProgramPath(full_program_path);
    simple_cgi.init(request);
    REQUIRE(simple_cgi.exec() != SYS_ERROR);

    pollfds->fd = simple_cgi.m_pipe_out[0];
    pollfds->events = POLL_IN;
    pollfds->revents = 0;
    for ( ;; )
    {
        REQUIRE(poll(pollfds, 1, POLL_NO_TIMEOUT) != SYS_ERROR);

        usleep(1000000);
        if (pollfds->revents > 0)
        {
            REQUIRE(waitpid(simple_cgi.m_fork_pid, NULL, 0) != SYS_ERROR);
            ssize_t ret = read(simple_cgi.m_pipe_out[0], buff, BUFF_TEST_SIZE);
            buff[ret] = '\0';
            REQUIRE(std::strcmp(buff, out.c_str()) == 0);
            break ;
        }
    }
}

TEST_CASE("Simple CGI exec [hello_world]")
{
    CGI             simple_cgi;
    SimpleRequest   request;
    struct pollfd   pollfds[1];

    char            buff[BUFF_TEST_SIZE];
    char            cwd[PATH_MAX];
    std::string     full_program_path;
    const std::string   out = ""
      "Content-type:text/html\r\n\r\n\n"
      "<html>\n"
      "<head>\n"
      "<title>Hello World - First CGI Program</title>\n"
      "</head>\n"
      "<body>\n"
      "<h2>Hello World! This is my first CGI program</h2>\n"
      "</body>\n"
      "</html>\n";

    getcwd(cwd, PATH_MAX);
    full_program_path += cwd;
    full_program_path += "/../../tests/unittests/cgi-bin/hello_world.py";
    request.parse(basic_post.c_str());
    simple_cgi.setProgramPath(full_program_path);
    simple_cgi.init(request);
    REQUIRE(simple_cgi.exec() != SYS_ERROR);

    pollfds->fd = simple_cgi.m_pipe_out[0];
    pollfds->events = POLL_IN;
    pollfds->revents = 0;
    for ( ;; )
    {
        REQUIRE(poll(pollfds, 1, POLL_NO_TIMEOUT) != SYS_ERROR);

        usleep(1000000);
        if (pollfds->revents > 0)
        {
            REQUIRE(waitpid(simple_cgi.m_fork_pid, NULL, 0) != SYS_ERROR);
            ssize_t ret = read(simple_cgi.m_pipe_out[0], buff, BUFF_TEST_SIZE);
            buff[ret] = '\0';
            REQUIRE(std::strcmp(buff, out.c_str()) == 0);
            break ;
        }
    }
}

TEST_CASE("Simple CGI exec [form_input]")
{
    CGI             simple_cgi;
    SimpleRequest   request;
    struct pollfd   pollfds[1];

    char            buff[BUFF_TEST_SIZE];
    char            cwd[PATH_MAX];
    std::string     full_program_path;
    const std::string   out = ""
      "Content-Type:text/html\r\n\r\n\n"
      "<h1>Addition Results</h1>\n"
      "<output>10 + 20 = 30</output>\n";

    getcwd(cwd, PATH_MAX);
    full_program_path += cwd;
    full_program_path += "/../../tests/unittests/cgi-bin/form_input.py";
    request.parse(basic_post.c_str());
    simple_cgi.setProgramPath(full_program_path);
    simple_cgi.init(request);
    REQUIRE(simple_cgi.exec() != SYS_ERROR);

    pollfds->fd = simple_cgi.m_pipe_out[0];
    pollfds->events = POLL_IN;
    pollfds->revents = 0;
    for ( ;; )
    {
        REQUIRE(poll(pollfds, 1, POLL_NO_TIMEOUT) != SYS_ERROR);

        usleep(1000000);
        if (pollfds->revents > 0)
        {
            REQUIRE(waitpid(simple_cgi.m_fork_pid, NULL, 0) != SYS_ERROR);
            ssize_t ret = read(simple_cgi.m_pipe_out[0], buff, BUFF_TEST_SIZE);
            buff[ret] = '\0';
            REQUIRE(std::strcmp(buff, out.c_str()) == 0);
            break ;
        }
    }
}

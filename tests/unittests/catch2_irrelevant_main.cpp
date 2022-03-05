#include <sys/poll.h>

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

int     main(int argc, char *argv[], char *envp[])
{
    CGI             simple_cgi;
    SimpleRequest   request;
    struct pollfd   pollfds[1];

    char            buff[BUFF_TEST_SIZE];
    char            cwd[PATH_MAX];
    std::string     full_program_path;

    getcwd(cwd, PATH_MAX);
    full_program_path += cwd;
    full_program_path += "/cgi-bin/form_input.py";
    request.parse(basic_post.c_str());
    simple_cgi.setProgramPath(full_program_path);
    simple_cgi.init(request);
    simple_cgi.exec();

    pollfds->fd = simple_cgi.m_pipe_out[0];
    pollfds->events = POLL_IN;
    pollfds->revents = 0;
    for ( ;; )
    {
        poll(pollfds, 1, POLL_NO_TIMEOUT);

        usleep(1000000);
        if (pollfds->revents > 0)
        {
            waitpid(simple_cgi.m_fork_pid, NULL, 0);
            std::cout << "gottem... ??" << '\n';
            ssize_t ret = read(simple_cgi.m_pipe_out[0], buff, BUFF_TEST_SIZE);
            buff[ret] = '\0';
            std::cout << buff << '\n';
            break ;
        }
        std::cout << "waiting..." << '\n';
    }
}
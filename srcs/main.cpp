#include <Webserv.hpp>
#include <Signals.hpp>
#include <FileParser.hpp>

/* Seems like "\e" is GNU only and "\033" is the standard sequence. */
#define B "\033[0;38;2;42;111;240m"
#define T "\033[0;38;2;48;213;200m"
#define R "\033[m"

static char ascii_wave[] = "\n"
   "██╗    ██╗███████╗██████╗ ███████╗██╗   ██╗██████╗ ███████╗" B "           _.====.._                    \n" R
   T "██║    ██║██╔════╝██╔══██╗██╔════╝██║   ██║██╔══██╗██╔════╝" B "         ,:._       ~-_                 \n" R
   T "██║ █╗ ██║█████╗  ██████╔╝███████╗██║   ██║██████╔╝█████╗  " B "             `\\        ~-_             \n" R
   T "██║███╗██║██╔══╝  ██╔══██╗╚════██║██║   ██║██╔══██╗██╔══╝  " B "               |          `.            \n" R
   T "╚███╔███╔╝███████╗██████╔╝███████║╚██████╔╝██║  ██║██║     " B "             ,/             ~-_         \n" R
   T " ╚══╝╚══╝ ╚══════╝╚═════╝ ╚══════╝ ╚═════╝ ╚═╝  ╚═╝╚═╝     " B "           -''                 ~~--..__.\n" R
   B ".-..____.-..____.-..____.-.._____.-..____.-..________.-..____.-.._..-'\n" R;

static void write_usage_stderr(void)
{
    fprintf(stderr, "usage: %s [config_file]\n", PROG_NAME);
    std::exit(EXIT_FAILURE);
}

int     main(int argc, char *argv[])
{
    FileParser::config_vec_t    config_ports;
    FileParser                  parser(argv[0]);
    PortConfig                  *port;
    pthread_t                   *thread_ids;
    int                         exit_code;

    if (argc > 2)
        write_usage_stderr();
    thread_ids = NULL;
    register_signals();
    exit_code = EXIT_SUCCESS;
    fprintf(stdout, "%s\n[INFO] Starting %s\n", ascii_wave, PROG_NAME);
    if (argc == 2)
        parser.setFilePath(argv[1]);
    try
    {
        parser.loadFile();
        parser.mapTokens(config_ports);
        thread_ids = new pthread_t[config_ports.size()];
        for (FileParser::config_vec_t::size_type idx = 0; idx < config_ports.size(); ++idx)
        {
            port = config_ports[idx];
            /* returns non-zero value on error */
            if (pthread_create(&thread_ids[idx], NULL, (THREAD_FUNC_PTR)PortConfig::pollPort, port))
            {
                /* error handling */
                break ;
            }
        }
        for (FileParser::config_vec_t::size_type idx = 0; idx < config_ports.size(); ++idx)
        {
            if (pthread_join(thread_ids[idx], NULL))
            {
                /* error handling */
                break ;
            }
        }
    }
    catch (const FileParser::InvalidFile& file_err)
    {
        fprintf(stderr, "[ERROR] Invalid file\n");
        exit_code = EXIT_FAILURE;
    }
    catch (const FileParser::MappingFailure& map_err)
    {
        fprintf(stderr, "[ERROR] Failed to parse file: %s\n", map_err.info().c_str());
        exit_code = EXIT_FAILURE;
    }
    catch (const std::bad_alloc& alloc_err)
    {
        /* some error handling */
    }
    delete [] thread_ids;
    return (exit_code);
}

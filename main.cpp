#include "includes/GetResponse.hpp"

int     main(void)
{
    Request     req_object;
    
    int fd = open("RequestFile.txt", O_RDONLY);
    req_object.copyRequest(fd);

    GetResponse res_object(req_object);

    res_object.buildResponse();

    return (EXIT_SUCCESS);
}

#include "TestServer.hpp"

#include <iostream>
#include <fstream>
#include <string>

std::string getFileContents (std::ifstream& File)
{
    std::string Lines = "";        //All lines
    
    if (File)                      //Check if everything is good
    {
	while (File.good ())
	{
	    std::string TempLine;                  //Temp line
	    std::getline (File , TempLine);        //Get temp line
	    TempLine += "\n";                      //Add newline character
	    
	    Lines += TempLine;                     //Add newline
	}
	return Lines;
    }
    else                           //Return error
    {
	return "ERROR File does not exist.";
    }
}

HDE::TestServer::TestServer() : SimpleServer(AF_INET, SOCK_STREAM, 0, 80, INADDR_ANY, 10)
{
	launch();
}

void HDE::TestServer::accepter()
{
	struct sockaddr_in address = get_socket()->get_address();
	int addrlen = sizeof(address);
	new_socket = accept(get_socket()->get_sock(), (struct sockaddr *)&address, (socklen_t *)&addrlen);
	read(new_socket, buffer, 30000);
}

void HDE::TestServer::handler()
{
	std::cout << buffer << std::endl;
}

void HDE::TestServer::responder()
{
	// char *hello = "Hello from server";

	// write(new_socket, txt, strlen(txt));
	// close(new_socket);

	std::ifstream Reader ("./Networking/Servers/file.txt");             //Open file

    std::string Art = getFileContents (Reader);       //Get file
	int n = Art.length();
	char char_array[n + 1];
	strcpy(char_array, Art.c_str());

	write(new_socket, char_array, n + 1);
    Reader.close ();                           //Close file
	close(new_socket);

    // std::cout << Art << std::endl;               //Print it to the screen
}

void HDE::TestServer::launch()
{
	while (true)
	{
		std::cout << "==== WAITING ==== " << std::endl;
		accepter();
		handler();
		responder();
		std::cout << " ==== DONE ==== " << std::endl;
	}
}
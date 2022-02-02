#ifndef CLIENT_HPP

# define CLIENT_HPP

class Client
{
private:
public:
	Client();
	Client(const Client& client);
	~Client();

	Client& operator = (const Client& client);	
};

#endif

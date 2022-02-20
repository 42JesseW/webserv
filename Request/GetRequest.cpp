#include "GetRequest.hpp"

GetRequest::GetRequest() {}

GetRequest::GetRequest(const GetRequest &copy) : Request(copy)
{
	m_extension = copy.m_extension;
}

GetRequest & GetRequest::operator=(const GetRequest &copy)
{
	if (this != &copy)
		*this = copy;
	return (*this);
}

GetRequest::~GetRequest() {}

void GetRequest::setExtension() 
{
	m_extension = "html";
}

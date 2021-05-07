#include "TCPServer.h"

TCPServer::TCPServer(int32_t port/*=34543*/)
	: _server_sockfd{0},
	_addr{0},
	_show_extra_info{true},
	_echo_mode{true}
{
	_server_sockfd = socket(AF_NET, SOCK_STREAM, 0);
	bind();
	_addr.sin_family = AF_NET;
	_addr.sin_port = htons(port);
	//_addr.sin_addr.s_addr = htonl(INADDR_ANY); // not needed while _addr{0}
}
//==================================================================================
int32_t TCPServer::socket(int32_t domain, int32_t sock_type, int32_t protocol)
{
	int32_t ret = ::socket(domain, type, protocol);
	if (ret == -1)
	{
		perror("[SERVER] socket() error");
		exit(EXIT_FAILURE);
	}
	return ret;
}
//==================================================================================
void TCPServer::bind()
{
	if (::bind(_server_sockfd, (struct sockaddr*)&_addr, sizeof(_addr)) == -1)
	{
		perror("[SERVER] bind() error");
		exit(EXIT_FAILURE);
	}
}
//==================================================================================
void TCPServer::listen(int32_t backlog/*=5*/)									
{
	if (_show_extra_info && backlog > 5)
	{
		std::cout << "[SERVER] warning: backlog is more than 5" << std::endl;
	}
	if (::listen(_server_sockfd, backlog) == -1)
	{
		perror("[SERVER] listen() error");
		exit(EXIT_FAILURE);
	}
}
//==================================================================================
int32_t TCPServer::accept()
{
	int32_t ret = ::accept(_server_sockfd, (struct sockaddr*)&_addr, sizeof(_addr));
	if (ret == -1)
	{
		perror("[SERVER] accept() error");
		exit(EXIT_FAILURE);
	}
	return ret;
}
//==================================================================================
ssize_t TCPServer::read(void* buf, size_t buf_size)
{
	ssize_t bytes_readed = ::read(_server_sockfd, buf, buf_size);
	if (bytes_readed == -1)
	{
		perror("[SERVER] read() error");
		exit(EXIT_FAILURE);
	}
	else if (_show_extra_info && bytes_readed == 0)
	{
		std::cout << "[SERVER] reading: end of file reached" << std::endl;
	}
	if (_echo_mode)
	{
		std::cout << "[ECHO SERVER] \"";
		::write(STDOUT_FILENO, buf, bytes_readed); // using outer write() due to avoid writing extra info
		std::cout << "\"" << std::endl;
	}
	return bytes_readed;
}
//==================================================================================
ssize_t TCPServer::write(int32_t fd, const void* buf, size_t buf_size)
{
	ssize_t bytes_wrote = ::write(fd, buf, buf_size);
	if (bytes_wrote == -1)
	{
		perror("[SERVER] write() error");
		exit(EXIT_FAILURE);
	}
	else if (_show_extra_info)
	{
		std::cout << "[SERVER] writing: wrote " << bytes_wrote << " bytes" << std::endl;
	}
	return bytes_wrote;
}
//==================================================================================
int32_t TCPServer::getSocket() const
{
	return _server_sockfd;
}
//==================================================================================
void TCPServer::setShowExtraInfo(bool flag)
{
	_show_extra_info = flag;
}
//==================================================================================
bool TCPServer::getShowExtraInfo() const
{
	return _show_extra_info;
}
//==================================================================================
void TCPServer::setEchoMode(bool flag)
{
	_echo_mode = flag;
}
//==================================================================================
bool TCPServer::setEchoMode() const
{
	return _echo_mode;
}

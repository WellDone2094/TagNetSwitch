#ifndef THESIS_SERVERCONNECTION_H
#define THESIS_SERVERCONNECTION_H


class ServerConnection {
private:
	int sockfd;
    uint16_t port;
public:
	ServerConnection(uint16_t);
	~ServerConnection();

	//error checking function
	int takeConn();
};


#endif //THESIS_SERVERCONNECTION_H

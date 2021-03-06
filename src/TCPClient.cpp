#include "TCPClient.h"
#include "Pollard.h"

/**
 * I am using the following sites as references to create this file: 
 * https://www.geeksforgeeks.org/socket-programming-cc/
 * https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/
 * https://www.geeksforgeeks.org/tcp-and-udp-server-using-select/
 * https://stackoverflow.com/questions/15673846/how-to-give-to-a-client-specific-ip-address-in-c
 * http://beej.us/guide/bgnet/html/  // This document is amazing. 
 * 
 * NOTE: I am also not going to break out the client code into helper methods as much as I did the server code 
 * because the code is much more simple and I don't think it would increase readability.  
 *
 * Author: Joshua H. White
**/

/**********************************************************************************************
 * TCPClient (constructor) - Creates a Stdin file descriptor to simplify handling of user input. 
 *
**********************************************************************************************/
TCPClient::TCPClient() : Client(){}

/**********************************************************************************************
 * TCPClient (destructor) - No cleanup right now
 *
**********************************************************************************************/
TCPClient::~TCPClient() {}

/**********************************************************************************************
 * connectTo - Opens a File Descriptor socket to the IP address and port given in the
 *             parameters using a TCP connection.
 *
 *    Throws: socket_error exception if failed. socket_error is a child class of runtime_error
**********************************************************************************************/
void TCPClient::connectTo(const char *ip_addr, unsigned short port) {
    // Create the socket for the client
    this->client_sock = socket(AF_INET, SOCK_STREAM, 0);
    if( this->client_sock == 0 ){
        perror("Failure in socket() in TCPClient.\n");
        exit(EXIT_FAILURE);
    }

    this->address.sin_family = AF_INET; 
    this->address.sin_port = htons( port ); // htons is shorthand for Host-to-Network short

    // Convert the IP address to binary form
    if(inet_pton(AF_INET, ip_addr, &address.sin_addr) <= 0){
        perror("Invalid address in TCPClient.\n");
        exit(EXIT_FAILURE);
    }

    // Now try to connect
    if(connect(this->client_sock, (struct sockaddr *)&this->address, sizeof(this->address)) < 0){
        perror("Failure in connect() in TCPClient.\n");
        exit(EXIT_FAILURE);
    }

}

/**********************************************************************************************
 * handleConnection - Performs a loop that checks if the connection is still open, then 
 *                    looks for user input and sends it if available. Finally, looks for data
 *                    on the socket and sends it.
 * 
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
**********************************************************************************************/
void TCPClient::handleConnection() {
    // Used in the main loop of the client connection
    std::string data; 

    // Read in the welcome message that the server sends upon connection
    //this->valread = read (client_sock, buf, 1024);
    //buf[this->valread] = '\0';
    //std::cout << buf << std::endl;

    // Main loop to handle the rest of the client session: 
    while(1){
        // I like to have this here to let the client know they can type now
        //std::cout << ">"; 
        // Get input from client
        //getline(std::cin, data);
        // Check to see if the client wants to close the connection
        //if(data == "exit"){
        //    send(this->client_sock, data.c_str(), strlen(data.c_str()), 0);
        //     std::cout << "Closing connection" << std::endl;
        //    close(this->client_sock); // Don't forget to close the socket
        //    break;
        //} else if (data == ""){
        //    data = "wrong";
        //}
        //send(this->client_sock, data.c_str(), strlen(data.c_str()), 0); 
        //std::cout << "message sent" << std::endl;
        //this->valread = read (this->client_sock, buf, 1024);
        //buf[this->valread] = '\0';
        //std::cout << "From server: " << buf << std::endl;
        
        // So instead of printing out what we get from the server here we will be 
        // 1. putting it into the correct data type (LARGEINT ?)
        // 2. calling calcPollardsRho(new_thing)
        // 3. take the output and put it into a string
        // 4. put string in buf and send it back
        this->valread = read (this->client_sock, buf, 1024);
        std::string number (buf);
        // <TODO> add exit
        
        LARGEINT find(number);
        Pollard solver = Pollard(find);
        find = solver.calcPollardsRho(find);
        std::string s = boost::lexical_cast<std::string>(find);
        send(this->client_sock, s.c_str(), strlen(s.c_str()), 0);

    }
}

/**********************************************************************************************
 * closeConnection - Your comments here
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
**********************************************************************************************/
void TCPClient::closeConn() {
    close(this->client_sock); 
}



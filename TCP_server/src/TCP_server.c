/*
 ============================================================================
 Name        : TCP_server.c
 Author      : Cristian Biallo
 Version     : 1.0.0
 Copyright   : Your copyright notice
 Description : TCP server implementation in C that handles password generation requests from a client.
               It listens for incoming connections, processes password requests, and sends responses.
 ============================================================================
 */

#if defined WIN32
#include <winsock.h>  /**< Include Winsock header for Windows */
#else
#include <unistd.h>  /**< Include UNIX standard header for close() */
#include <sys/socket.h>  /**< Include socket library for UNIX */
#include <arpa/inet.h>  /**< Include ARP and Internet address family libraries */
#include <sys/types.h>   /**< Include for socket types */
#include <netinet/in.h>  /**< Include for internet address family structures */
#include <netdb.h>  /**< Include for host and network databases */
#define closesocket close  /**< Define closesocket to close for UNIX systems */
#endif

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "libs/password/password.h"  /**< Include the header for password generation functions */
#include "libs/protocol/protocol.h"  /**< Include the protocol definitions for communication */
#include "libs/utils/utils.h"     /**< Include the utils.h library for utility functions */


/**
 * @brief Clean up the Winsock library (Windows only).
 * This function is called to clean up the Winsock library when the server exits on a Windows system.
 */
void clearwinsock() {
#if defined WIN32
	WSACleanup();  /**< Cleans up the Winsock DLL */
#endif
}


/**
 * @brief Prints an error message to the console.
 * This function formats and prints the error message in magenta color.
 * @param[in] errorMessage: the error message to be printed.
 */
void errorhandler(char *errorMessage) {
	print_with_color(errorMessage, MAGENTA);  /**< Print the error message in Magenta */
}

int main() {

#if defined WIN32
	// Initialize Winsock
	WSADATA wsa_data;  /**< Holds information about the Windows Sockets implementation */
	WORD version_requested = MAKEWORD(2,2);  /**< Request version 2.2 of Winsock */
	int result = WSAStartup(version_requested, &wsa_data);  /**< Initialize Winsock library */
	if (result != NO_ERROR) {
		print_with_color("Error at WSAStartup()\n", MAGENTA);
		print_with_color("A usable WinSock DLL cannot be found.\n", MAGENTA);
		Sleep(3000); /**< Wait for 3 seconds */
		return -1;
	}
#endif

	// Create a welcome socket for the server to listen for incoming client connections
	int my_socket;
	my_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  /**< Create the server socket */
	if (my_socket < 0) {
		closesocket(my_socket);  /**< Close the socket */
		clearwinsock();  /**< Clean up Winsock */
		#if defined WIN32
			Sleep(3000);  /**< Wait before exiting */
		#endif
		return -1;
	}

	// Set up the socket address structure for binding the socket
	struct sockaddr_in sad;  /**< Socket address structure for binding */
	memset(&sad, 0, sizeof(sad));  /**< Clear the structure */
	sad.sin_family = AF_INET;  /**< Set the address family to AF_INET (IPv4) */
	sad.sin_addr.s_addr = inet_addr(DEFAULT_IP);  /**< Set the server's IP address */
	sad.sin_port = htons(DEFAULT_PORT);  /**< Convert port number to network byte order */

	// Bind the socket to the IP address and port
	if (bind(my_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
		errorhandler("Bind failed.\n");
		closesocket(my_socket);  /**< Close the socket */
		clearwinsock();  /**< Clean up Winsock */
		#if defined WIN32
			Sleep(3000);  /**< Wait before exiting */
		#endif
		return -1;
	}

	// Listen for incoming connections on the socket with a queue length of QLEN
	if (listen(my_socket, QLEN) < 0) {
		errorhandler("Listen failed.\n");
		closesocket(my_socket);  /**< Close the socket */
		clearwinsock();  /**< Clean up Winsock */
		#if defined WIN32
			Sleep(3000);  /**< Wait before exiting */
		#endif
		return -1;
	}

	// Accept new client connections in an infinite loop
	struct sockaddr_in cad;  /**< Client address structure */
	int client_socket;  /**< Socket descriptor for the client */
	int client_len;  /**< Size of the client address structure */
	print_with_color("Waiting for a client to connect...\n\n", BLUE);

	while (1) {
		client_len = sizeof(cad); /**< Set the size of the client address structure */

		// Accept a client connection
		if ((client_socket = accept(my_socket, (struct sockaddr*) &cad, &client_len)) < 0) {
			errorhandler("Accept failed (Client connection).\n");
			closesocket(client_socket);  /**< Close the socket */
			clearwinsock();  /**< Clean up Winsock */
			#if defined WIN32
				Sleep(3000);  /**< Wait before exiting */
			#endif
			return 0;
		}

		// Print client's IP address and port number
		print_with_color("New connection from ", GREEN);
		print_with_color(inet_ntoa(cad.sin_addr),YELLOW);
		print_with_color(":", CYAN);
		printf("%d\n",ntohs(cad.sin_port));

		// Create the menu to send to the client
		MenuMessage menu_msg;
		snprintf(menu_msg.menu_text, sizeof(menu_msg.menu_text),
				"Insert the type of password and its length (between 6 and 32):\n"
				"  n: numeric password (only digits)\n"
				"  a: alphabetic password (only lowercase letters)\n"
				"  m: mixed password (lowercase letters and digits)\n"
				"  s: secure password (uppercase letters, lowercase letters, digits, and symbols)\n"
				"  q: to close the connection\n"
				"? ");

		// Send the menu to the client
		if (send(client_socket, &menu_msg, sizeof(menu_msg), 0) != sizeof(menu_msg)) {
			errorhandler("send() sent a different number of bytes than expected (Menu).\n");
			closesocket(client_socket);  /**< Close the socket */
			clearwinsock();  /**< Clean up Winsock */
			#if defined WIN32
				Sleep(3000);  /**< Wait before exiting */
			#endif
			return -1;
		}


		// Handle password generation in a loop
		PasswordRequest password_msg;
		PasswordResponse response_msg;
		do{
			// Receive password type and length from the client
			if (recv(client_socket, &password_msg, sizeof(password_msg), 0) <= 0) {
				errorhandler("recv() failed or connection closed prematurely (Password settings).\n");
				closesocket(client_socket);  /**< Close the socket */
				clearwinsock();  /**< Clean up Winsock */
				#if defined WIN32
					Sleep(3000);  /**< Wait before exiting */
				#endif
				return -1;
			}

			// Check if the server should continue generating passwords
			response_msg.keep_going = keep_generating(password_msg.type, 'q');
			if(response_msg.keep_going) {
				// Validate password type and length using control functions from password.h
				if(control_type("nams", password_msg.type)) {
					if(control_length(password_msg.length, MIN_PASSWORD_LENGTH, MAX_PASSWORD_LENGTH)) {
						int numerical_length = atoi(password_msg.length); // Convert the string containing the length without the initial space
						// Determine the password type
						PasswordType password_type;
						switch (tolower(password_msg.type)) {
							case 'n':
								password_type = NUMERIC;
								break;
							case 'a':
								password_type = ALPHA;
								break;
							case 'm':
								password_type = MIXED;
								break;
							case 's':
								password_type = SECURE;
								break;
						}
						// Generate password
						generate_password(response_msg.password, password_type, numerical_length);
						strcpy(response_msg.error_msg, ""); // Error message absent
						response_msg.request_error = false;	// No error found

					}
					else {
						// Handle invalid length error
						strcpy(response_msg.password, "");  // No password generated
						strcpy(response_msg.error_msg, "The length for the password is not valid.\n"); // Error message for the length
						response_msg.request_error = true;	// Error found for the password length
					}
				}
				else {
					// Handle invalid type error
					strcpy(response_msg.password, ""); // No password generated
					strcpy(response_msg.error_msg, "The type inserted is not valid.\n"); // Error message for the type
					response_msg.request_error = true;	// Error found for the type
				}
			}
			else  {
				// No error, close connection
				strcpy(response_msg.password, ""); // No password generated
				strcpy(response_msg.error_msg, ""); // No error message
				response_msg.request_error = false;	// No error found
			}

			// Send password generation response to the client
			if (send(client_socket, &response_msg, sizeof(response_msg), 0) != sizeof(response_msg)) {
				errorhandler("send() sent a different number of bytes than expected (Password response).\n");
				closesocket(client_socket);  /**< Close the socket */
				clearwinsock();  /**< Clean up Winsock */
				#if defined WIN32
					Sleep(3000);  /**< Wait before exiting */
				#endif
				return -1;
			}

		} while(response_msg.keep_going);

		// Closing the connection with the client
		closesocket(client_socket); /**< Close the socket */
		print_with_color("Connection with the client closed.\n\n", BLUE);
	}

	// Clean up Winsock before exit (for Windows only)
	clearwinsock(); /**< Clean up Winsock */
	#if defined WIN32
		Sleep(3000);  /**< Wait before exiting */
	#endif
	return 0;
}

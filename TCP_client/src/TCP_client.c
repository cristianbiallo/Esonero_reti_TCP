/*
 ============================================================================
 Name        : TCP_client.c
 Author      : Cristian Biallo.
 Version     : 1.0.0
 Description : A TCP client implementation in C that communicates with a password generation server.
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
#include <string.h>
#include <stdbool.h>
#include "libs/protocol/protocol.h"  /**< Include protocol header for message structures and communication formats */
#include "libs/utils/utils.h"	   /**< Include the utils.h library for utility functions */


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

	// Create the client socket for communication with the server
	int c_socket;
	c_socket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);  /**< Create the client socket */
	if (c_socket < 0) {
		errorhandler("Client socket creation failed.\n");
		closesocket(c_socket);  /**< Close socket if creation failed */
		clearwinsock();  /**< Clean up Winsock */
		#if defined WIN32
			Sleep(3000);  /**< Wait before exiting */
		#endif
		return -1;
	}

	// Set up the server connection settings
	struct sockaddr_in sad;  /**< Socket address structure for the server */
	memset(&sad, 0, sizeof(sad));  /**< Clear the structure */
	sad.sin_family = AF_INET;  /**< Set address family to AF_INET (IPv4) */
	sad.sin_addr.s_addr = inet_addr(DEFAULT_IP);  /**< Set server IP address */
	sad.sin_port = htons(DEFAULT_PORT);  /**< Set server port, converting to network byte order */

	// Establish the connection to the server
	if (connect(c_socket, (struct sockaddr*) &sad, sizeof(sad)) < 0) {
		errorhandler("Connection failed.\n");
		closesocket(c_socket);  /**< Close the socket */
		clearwinsock();  /**< Clean up Winsock */
		#if defined WIN32
			Sleep(3000);  /**< Wait before exiting */
		#endif
		return -1;
	}

	// Indicate successful connection
	print_with_color("Connection completed\n\n", BLUE);

	// Receive the menu message from the server
	MenuMessage menu_msg;  /**< Message structure for the menu */
	if (recv(c_socket, &menu_msg, sizeof(menu_msg), 0) <= 0) {
		errorhandler("recv() failed or connection closed prematurely (Menu).\n");
		closesocket(c_socket);  /**< Close the socket */
		clearwinsock();  /**< Clean up Winsock */
		#if defined WIN32
			Sleep(3000);  /**< Wait before exiting */
		#endif
		return -1;
	}

	// Start password generation process
	PasswordRequest password_msg;   /**< Structure to hold password request (type and length) */
	PasswordResponse response_msg;  /**< Structure to hold server's response */
	char input[BUFFER_SIZE];
	do {
		// Display the menu and prompt the user to input password type and length
		print_with_color(menu_msg.menu_text, YELLOW);	 /**< Print the server's menu */

	    // Read all input row
		fgets(input, sizeof(input), stdin); /**< Read user input for password type and length */
		input[BUFFER_SIZE-1] = '\0';	/**< Ensure null termination for the length string */

		int arguments = sscanf(input," %c %s %s",&password_msg.type, password_msg.length, input); /**< Read user input for password type and length */
		password_msg.length[BUFFER_SIZE-1] = '\0';	/**< Ensure null termination for the length string */

        // Check if only the type is entered (no length)
        if (arguments == 1) {
        	print_with_color("(The length is absent, a default value is used: 8)\n", CYAN);
            strcpy(password_msg.length, "8");  // Default value if the length is absent
        } else if (arguments != 2) {
            // If the input is not valid, report the issue
            print_with_color("Invalid input. Please enter a valid type and length.\n", RED);
            response_msg.keep_going = true; /**< Allow to continue the cycle */
            continue;  // Continue the cycle if the input is not legit
        }

		// Send the password request to the server
		if (send(c_socket, &password_msg, sizeof(password_msg), 0) != sizeof(password_msg)) {
			errorhandler("send() sent a different number of bytes than expected (Password settings).\n");
			closesocket(c_socket);  /**< Close the socket */
			clearwinsock();  /**< Clean up Winsock */
			#if defined WIN32
				Sleep(3000);  /**< Wait before exiting */
			#endif
			return -1;
		}

		// Receive the server's password generation response
		if (recv(c_socket, &response_msg, sizeof(response_msg), 0) <= 0) {
			errorhandler("recv() failed or connection closed prematurely (Password generation response).\n");
			closesocket(c_socket);  /**< Close the socket */
			clearwinsock();  /**< Clean up Winsock */
			#if defined WIN32
				Sleep(3000);  /**< Wait before exiting */
			#endif
			return -1;
		}

		if(response_msg.keep_going){
			// Check if there was an error with the request
			if(response_msg.request_error) {
				print_with_color("Bad request: ", RED);
				print_with_color(response_msg.error_msg, RED);
				puts("");
			}
			else {
				// Display the generated password
				print_with_color("Password generated: ", GREEN);
				print_with_color(response_msg.password, GREEN);
				printf("\n\n");
			}
		}

	} while(response_msg.keep_going); /**< Continue until the server indicates to stop */


	// Close the connection with the server and clean up
	closesocket(c_socket);  /**< Close the socket */
	clearwinsock();  /**< Clean up Winsock */
	return 0;
}

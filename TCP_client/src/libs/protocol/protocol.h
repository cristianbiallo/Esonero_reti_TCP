/*
 ============================================================================
 Name        : protocol.h
 Author      : Cristian Biallo.
 Version     : 1.0.0
 Description : Header file used to define constants, structs, and protocol-specific
               data structures to support the client communication in the `TCP_client.c` file.
 ============================================================================
 */

#ifndef PROTOCOL_H
#define PROTOCOL_H

/* - - - - - - - - - - - - - - - - - - - DEFINES - - - - - - - - - - - - - - - - */

/**
 * @brief Defines the maximum size of the buffer used for communication.
 * The buffer is used for storing data such as menu messages and passwords.
 * This limit ensures the buffer is large enough for most operations but is still manageable in size.
 */
#define BUFFER_SIZE 1024        /**< Max dimension of the buffer */

/**
 * @brief Maximum allowable password length.
 * This constant defines the maximum length that can be set for a generated password.
 * The server or client will use this value to enforce password length restrictions.
 */
#define MAX_PASSWORD_LENGTH 32  /**< Maximum password length */

/**
 * @brief Default IP address for server-client communication.
 * The default IP is set to `127.0.0.1`, which is the loopback address for local communication.
 */
#define DEFAULT_IP "127.0.0.1"  /**< Default IP address for communication */

/**
 * @brief Default port number for the server to communicate with the client.
 * The default port is `8080`, allowing the client to connect to the server on this port.
 */
#define DEFAULT_PORT 8080       /**< Default port number for communication */

/* - - - - - - - - - - - - - - - - - - - END DEFINES - - - - - - - - - - - - - - - - */

/* - - - - - - - - - - - - - - - - - - STRUCTS - - - - - - - - - - - - - - - - - - - - */

/**
 * @struct MenuMessage
 * @brief Struct used to represent the menu text that will be sent to the client.
 *
 * This struct contains a field `menu_text` that holds the text of the menu, which is
 * typically displayed to the client upon a successful connection. The maximum size
 * of the menu text is constrained by `BUFFER_SIZE`.
 */
typedef struct {
    char menu_text[BUFFER_SIZE];  /**< Menu text to send to the client */
} MenuMessage;

/**
 * @struct PasswordRequest
 * @brief Struct used to represent a password generation request from the client.
 *
 * This struct contains:
 * - `type`: Specifies the type of password the client requests.
 * - `length`: Specifies the desired length of the generated password.
 *
 * The `length` field is stored as a string, allowing for validation to be done during processing.
 */
typedef struct {
    char type;        				/**< Type of the password to generate */
    char length[BUFFER_SIZE];       /**< Length of the password requested */
} PasswordRequest;

/**
 * @struct PasswordResponse
 * @brief Struct used to represent the response sent back to the client after a password generation request.
 *
 * This struct includes:
 * - `keep_going`: A flag to determine if the password generation process should continue.
 * - `password`: The actual generated password returned to the client.
 * - `request_error`: A flag indicating if there was an error in the password request.
 * - `error_msg`: A string that contains an error message if `request_error` is `true`.
 */
typedef struct {
    bool keep_going;            /**< Flag indicating if password generation should continue */
    char password[MAX_PASSWORD_LENGTH + 1];  /**< The generated password */
    bool request_error;         /**< Flag indicating if there was an error with the request */
    char error_msg[50];         /**< Error message if `request_error` is triggered */
} PasswordResponse;

/* - - - - - - - - - - - - - - - - - - - END STRUCTS - - - - - - - - - - - - - - - - - - - - - */

#endif // PROTOCOL_H

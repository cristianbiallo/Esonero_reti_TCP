/*
 ============================================================================
 Name        : password.c
 Author      : Cristian Biallo
 Version     : 1.0.0
 Description : Generates passwords based on specified length and type.
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "password.h"


/* - - - - - - - - - - - - - - - - - PASSWORD GENERATION - - - - - - - - - - - - - - - - - */

/**
 * @brief Generates a numeric password.
 * @param[in/out] password: a pointer where the generated numeric password will be stored.
 * @param[in] length: the desired length of the password.
 * @pre `length` should be a positive integer.
 * @post `password` is populated with a numeric password of the specified length.
 */
void generate_numeric(char *password, int length) {
    for (int i = 0; i < length; i++) {
        password[i] = '0' + rand() % 10; // Generates a digit between 0 and 9
    }
    password[length] = '\0';
}


/**
 * @brief Generates a lowercase alphabetic password.
 * @param[in/out] password: a pointer where the generated alphabetic password will be stored.
 * @param[in] length: the desired length of the password.
 * @pre `length` should be a positive integer.
 * @post `password` is populated with a lowercase alphabetic password.
 */
void generate_alpha(char *password, int length) {
    for (int i = 0; i < length; i++) {
        password[i] = 'a' + rand() % 26; // Generates a lowercase letter between 'a' and 'z'
    }
    password[length] = '\0';
}


/**
 * @brief Generates an alphanumeric password (digits and lowercase letters).
 * @param[in/out] password: a pointer where the generated alphanumeric password will be stored.
 * @param[in] length: the desired length of the password.
 * @pre `length` should be a positive integer.
 * @post `password` is populated with an alphanumeric password.
 */
void generate_mixed(char *password, int length) {
    for (int i = 0; i < length; i++) {
        password[i] = (rand() % 2) ? 'a' + rand() % 26 : '0' + rand() % 10; // Generates either a digit or a lowercase letter
    }
    password[length] = '\0';
}


/**
 * @brief Generates a secure password with letters, symbols, and numbers.
 * @param[in/out] password: a pointer where the generated secure password will be stored.
 * @param[in] length: the desired length of the password.
 * @pre `length` should be a positive integer.
 * @post `password` is populated with a secure password.
 */
void generate_secure(char *password, int length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*()";
    for (int i = 0; i < length; i++) {
        password[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    password[length] = '\0';
}


/**
 * @brief Generates a password based on the specified length and type.
 * @param[in/out] password: a pointer to store the generated password.
 * @param[in] type: the type of password to generate (NUMERIC, ALPHA, MIXED, or SECURE).
 * @param[in] length: the length of the password to generate.
 * @pre `length` should be a positive integer, and `type` should be a valid PasswordType.
 * @post `password` is populated with the generated password.
 */
void generate_password(char *password, PasswordType type, int length) {
    switch(type) {
        case NUMERIC:
            generate_numeric(password, length);
            break;
        case ALPHA:
            generate_alpha(password, length);
            break;
        case MIXED:
            generate_mixed(password, length);
            break;
        case SECURE:
            generate_secure(password, length);
            break;
    }
}

/* - - - - - - - - - - - - - - - - END PASSWORD GENERATION - - - - - - - - - - - - - - - - */


/* - - - - - - - - - - - - - - - - - PASSWORD CONTROLS - - - - - - - - - - - - - - - - - */

/**
 * @brief Checks if the generation should continue based on the specified end type.
 * @param[in] type: the current password type.
 * @param[in] type_for_ending: the type used to indicate the end of generation.
 * @return `true` if `type` is not equal to `type_for_ending`.
 * @return `false` if `type` is equal to `type_for_ending`.
 * @pre `type` and `type_for_ending` should be valid characters.
 */
bool keep_generating(const char type, const char type_for_ending) {
    return tolower(type) != tolower(type_for_ending);
}


/**
 * @brief Validates if the specified type is allowed.
 * @param[in] allowed_type: a string of allowed types.
 * @param[in] type: the type to check.
 * @return `true` if `type` is found in `allowed_type`.
 * @return 'false' if  'type` is not found in `allowed_type`.
 * @pre `allowed_type` should contain valid characters.
 */
bool control_type(const char *allowed_type, const char type) {
    return strchr(allowed_type, type) != NULL;
}


/**
 * @brief Validates if the length is within the specified range and is a positive integer.
 * @param[in] length: a string representing the desired password length.
 * @param[in] min_length: minimum allowable length.
 * @param[in] max_length: maximum allowable length.
 * @return `true` if `length` is within the range and valid.
 * @return 'false' if 'length' is not within the range and valid.
 * @pre `min_length` and `max_length` should be positive and `min_length < max_length`.
 */
bool control_length(const char *length, const int min_length, const int max_length) {
    // Check if all characters in `length` are digits
    for (int i = 0; length[i] != '\0'; i++) {
        if (!isdigit(length[i])) {
            return false;
        }
    }

    // Convert `length` to an integer and validate the range
    int numerical_length = atoi(length);
    return numerical_length >= min_length && numerical_length <= max_length;
}

/* - - - - - - - - - - - - - - - END PASSWORD CONTROLS - - - - - - - - - - - - - - - - */

/*
 ============================================================================
 Name        : password.h
 Author      : Cristian Biallo.
 Version     : 1.0.0
 Description : Header file providing functions to generate and validate passwords
               based on specified criteria. Functions include type validation,
               length validation, and password generation in multiple formats.
 ============================================================================
 */

#ifndef PASSWORD_H_
#define PASSWORD_H_

#include <stdbool.h>


/* - - - - - - - - - - - - - - - - - - - PASSWORD TYPES - - - - - - - - - - - - - - - - - */

/**
 * @enum PasswordType
 * @brief Enumerates the types of passwords that can be generated.
 *
 * - `NUMERIC`: Generates a password consisting of only numeric digits (0-9).
 * - `ALPHA`: Generates a password using lowercase alphabetic characters (a-z).
 * - `MIXED`: Generates a password with a mix of lowercase alphabetic characters and digits.
 * - `SECURE`: Generates a password with lowercase and uppercase alphabetic characters, digits, and symbols.
 */
typedef enum {
    NUMERIC,   /**< Numeric password */
    ALPHA,     /**< Lowercase alphabetic password */
    MIXED,     /**< Lowercase alphanumeric password */
    SECURE     /**< Secure password with uppercase, lowercase, numbers, and symbols */
} PasswordType;

/* - - - - - - - - - - - - - - - - - - END PASSWORD TYPES - - - - - - - - - - - - - - - - */

/* - - - - - - - - - - - - - - - - - - PASSWORD GENERATION - - - - - - - - - - - - - - - - - - */

/**
 * @brief Generates a password based on the specified type and length.
 *
 * This function creates a password using the specified `type` (NUMERIC, ALPHA, MIXED, SECURE) and length.
 * The resulting password is stored in the `password` parameter, which should be allocated to hold
 * at least `length + 1` characters (to account for the null terminator).
 *
 * - `NUMERIC`: Only numeric digits (0-9).
 * - `ALPHA`: Only lowercase alphabetic characters (a-z).
 * - `MIXED`: A mix of lowercase alphabetic characters and digits.
 * - `SECURE`: A combination of lowercase and uppercase alphabetic characters, digits, and symbols.
 *
 * @param[out] password: a pre-allocated array to store the generated password.
 *                       should have space for at least `length + 1` characters.
 * @param[in] type: the type of password to generate, as specified in the `PasswordType` enum.
 * @param[in] length: the desired length of the generated password.
 */
void generate_password(char *password, PasswordType type, int length);

/* - - - - - - - - - - - - - - - - - END PASSWORD GENERATION - - - - - - - - - - - - - - - - - */


/* - - - - - - - - - - - - - - - - - - PASSWORD CONTROLS - - - - - - - - - - - - - - - - - - */

/**
 * @brief Checks if password generation should continue based on the specified type.
 *
 * This function compares the `type` parameter to the `type_for_ending` parameter. If the
 * types do not match, it indicates that generation should continue. Otherwise, generation
 * will end if `type` matches `type_for_ending`.
 *
 * @param[in] type: specifies the current type of password being generated.
 * @param[in] type_for_ending: specifies the type that signals the end of generation.
 * @return `true` if `type` does not match `type_for_ending`, indicating continuation of generation.
 * @return `false` if `type` matches `type_for_ending`, signaling the end of generation.
 */
bool keep_generating(const char type, const char type_for_ending);


/**
 * @brief Validates if the specified password type is among the allowed types.
 *
 * This function checks if the given `type` parameter is one of the characters listed
 * in the `allowed_type` string. It returns `true` if the type is valid, and `false` otherwise.
 *
 * @param[in] allowed_type: a string containing all allowed types (e.g., "nams") for passwords.
 * @param[in] type: the specific type of password requested, to be validated against `allowed_type`.
 * @return `true` if `type` is among the allowed types in `allowed_type`.
 * @return `false` if `type` is not present in `allowed_type`.
 */
bool control_type(const char *allowed_type, const char type);


/**
 * @brief Checks if the password length is within a specified range and is positive.
 *
 * This function ensures the requested password length is both a positive integer and falls within
 * the specified range `[min_length, max_length]`. It first verifies that `length` is a numeric string,
 * then converts it to an integer for range checking.
 *
 * @param[in] length: a string representation of the desired password length, to be checked.
 * @param[in] min_length: the minimum allowable password length.
 * @param[in] max_length: the maximum allowable password length.
 * @return `true` if `length` is a positive integer within `[min_length, max_length]`.
 * @return `false` if `length` is not positive or is outside the specified range.
 */
bool control_length(const char *length, const int min_length, const int max_length);

/* - - - - - - - - - - - - - - - - - END PASSWORD CONTROLS - - - - - - - - - - - - - - - - - */

#endif /* PASSWORD_H_ */

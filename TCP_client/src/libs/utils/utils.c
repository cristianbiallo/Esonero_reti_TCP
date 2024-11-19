/*
 ============================================================================
 Name        : utils.c
 Author      : Cristian Biallo
 Version     : 1.0.0
 Description : Implementation of utility functions.

 Utilities included:
     - COLORS:
          1. print_with_color: Prints the specified text in the specified color.
 ============================================================================
 */

#include <stdio.h>
#include "utils.h"

/* - - - - - - - - - - - - - - - - - COLORS - - - - - - - - - - - - - - - - - */
/**
 * @brief Returns the ANSI code for the specified color.
 * @param[in] color: contains the color to use.
 * @pre The parameter should be of type `textColor`.
 * @post Returns a pointer to the ANSI escape code string for the specified color.
 */
const char *generate_ansi_color_code(textColor color) {
    switch(color) {
		case BLACK:
			return "\033[30m";
		case RED:
			return "\033[31m";
		case GREEN:
			return "\033[32m";
		case YELLOW:
			return "\033[33m";
		case BLUE:
			return "\033[34m";
		case MAGENTA:
			return "\033[35m";
		case CYAN:
			return "\033[36m";
		case WHITE:
			return "\033[37m";
		case RESET:
			return "\033[0m";
		default:
			return "\033[0m";	// Returns RESET by default
	}
}


/**
 * @brief Prints the specified text in the specified color.
 * @param[in] text: a pointer to the text to print.
 * @param[in] color: the textColor to apply to the text.
 * @pre `text` should be a non-null pointer to a valid string.
 * @post Prints `text` with the specified color applied.
 */
void print_with_color(const char *text, textColor color) {
    printf("%s%s%s", generate_ansi_color_code(color), text, generate_ansi_color_code(RESET));
}

/* - - - - - - - - - - - - - - - - END COLORS - - - - - - - - - - - - - - - - */

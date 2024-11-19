/*
 ============================================================================
 Name        : utils.h
 Author      : Cristian Biallo
 Version     : 1.0.0
 Description : Provides utility functions.

 Utilities included:
     - COLORS:
          1. print_with_color: Prints the specified text in the specified color.
 ============================================================================
 */

#ifndef UTILS_H_
#define UTILS_H_

/* - - - - - - - - - - - - - - - - - COLORS - - - - - - - - - - - - - - - - - */

/**
 * @enum textColor
 * @brief Defines colors for text output.
 *
 * Enumerates ANSI color codes to specify text colors in terminal output.
 */
typedef enum {
    BLACK,      /**< Black text color */
    RED,        /**< Red text color */
    GREEN,      /**< Green text color */
    YELLOW,     /**< Yellow text color */
    BLUE,       /**< Blue text color */
    MAGENTA,    /**< Magenta text color */
    CYAN,       /**< Cyan text color */
    WHITE,      /**< White text color */
    RESET       /**< Resets to default text color */
} textColor;


/**
 * @brief Prints the specified text in the specified color.
 *
 * This function prints a string with the ANSI color specified by the color
 * parameter. After printing, the text color is reset to default.
 *
 * @param[in] text Pointer to the string to print.
 * @param[in] color The textColor to apply to the text.
 */
void print_with_color(const char *text, textColor color);

/* - - - - - - - - - - - - - - - - END COLORS - - - - - - - - - - - - - - - - */

#endif /* UTILS_H_ */

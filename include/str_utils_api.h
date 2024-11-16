// TODO: Export str_utils as a stand alone package.
#pragma once

/**
 * @file        str_utils_api.h
 * @author      Shahar Avidar   (ikobh7@gmail.com)
 * @brief       str_utils enhances string.h & enables a richer usefull API.
 * @version     0.1
 * @date        2024-07-18
 *
 * @copyright Copyright (c) 2024
 *
 */
#include <stdlib.h>

/**
 * @brief       The strcumlen() function calculates the overall length of 'cnt' strings
 *              specified by a variable input list, excluding the terminating null byte ('\0').
 * @note        A mismatch between 'cnt' & the actual number of strings, using a non char* type,
 *              Will result in an undefined behaviour.
 *
 * @param       cnt             Number of strings.
 * @param       ...             'cnt' strings.
 * @return      size_t
 */
size_t strcumlen(int cnt, ...);

/**
 * @brief       The str_embed() function embeds the 'tok' string into the 'src' string,
 *              at position 'pos', The outcome is saved into 'dest'.
 *              The function returns a pointer to 'dest'.
 * @note        It is the user responsibility to ensure 'dest'hase enough allocated memory for the final string,
 *              or else an undefined behaviour is expected.
 *
 * @param       src             Source string
 * @param       tok             Token to be embedded within 'src'
 * @param       pos             The position at which the embedding will start.
 * @param       dest            Destination pointer to hold the outcome string.
 * @return      char*
 */
char *str_embed(const char *src, const char *tok, int pos, char *dest);

/**
 * @brief       Text
 *
 * @param       src             My Param doc
 * @param       count           My Param doc
 * @return      char**
 */
char **strdup_array(char **src, size_t count);
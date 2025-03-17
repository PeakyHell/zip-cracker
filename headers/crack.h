#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

#ifndef _CRACK_H_
#define _CRACK_H_

/**
 * Convert a decimal number in base 10 to a value of base BASE (defined in the file).
 *
 * @param int num The decimal number
 * @param char* buffer The buffer where the result will be stored
 */
void base_10_to_other_base(int num, char *buffer);

/**
 * Escape special characters in the password to avoid errors in command-line.
 * 
 * @param char* password The password to escape
 * @param char* escaped A char buffer where the escaped password will be stored
 */
void escape_password(char *password, char *escaped);

/**
 * Build the 7zip command and execute it in the command-line.
 * 
 * @param char* password The password that will be tried on the archive
 * @param char* archive_path The path to the archive to brute-force
 * @param char* output_path The path to the folder where the content of the archive will be placed
 * @param char* buffer The buffer that will store the command
 */
int test_password(char *password, char *archive_path, char *output_path, char *buffer);

/**
 * Test all password of length 1 to a give length to brute-force an archive
 * 
 * @param int max_length The maximum password length to try
 * @param char* archive_path The path to the archive to brute-force
 * @param char* output_path The path to the folder where the content of the archive will be placed
 */
int test_all_passwords(int max_length, char *archive_path, char *output_path);

#endif /* _CRACK_H_ */
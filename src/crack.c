#include "../headers/crack.h"

#if defined(__unix__) || defined(__APPLE__)
#define OS "UNIX"
#elif defined(_WIN32) || defined(_WIN64)
#define OS "WINDOWS"
#endif

#define BASE 94
#define CHARSET "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

void base_10_to_other_base(int num, char* buffer) {
    // Convert to BASE
    int i = 0;
    int digit;
    do {
        digit = num % BASE;
        buffer[i++] = CHARSET[digit];
        num /= BASE;
    } while (num > 0);

    // Null-termination
    buffer[i] = '\0';

    // Reverse string
    char *ptr = buffer;
    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char tmp = ptr[j];
        ptr[j] = ptr[len - j - 1];
        ptr[len - j - 1] = tmp;
    }
}

void escape_password(char *password, char *escaped) {
    while (*password) {
        if (strchr(" \"#$&'()*;<>?[]`\\|", *password)) {
            *escaped++ = '\\';
        }
        *escaped++ = *password++;
    }
    *escaped= '\0';
}

int test_password(char *password, char *archive_path, char *output_path, char *buffer) {
    // Get 7zip command and null output for specific OS
    char *_7zip = strcmp(OS, "UNIX") == 0 ? "7z" : "path to 7zip"; // TODO
    char *no_output = strcmp(OS, "UNIX") == 0 ? "/dev/null" : "NUL";

    // Escape special characters in password
    char escaped_password[2 * strlen(password) + 1];
    escape_password(password, escaped_password);

    // Make a buffer for the command
    size_t size = snprintf(NULL, 0, "%s x -p%s \"%s\" -o\"%s\" -y > %s 2>&1", _7zip, escaped_password, archive_path, output_path, no_output) + 1;
    snprintf(buffer, size, "%s x -p%s \"%s\" -o\"%s\" -y > %s 2>&1", _7zip, escaped_password, archive_path, output_path, no_output);

    // Execute the command
    return system(buffer);
}

int test_all_passwords(int max_length, char *archive_path, char *output_path) {
    clock_t start_time = clock();

    // Create a buffer to store the 7zip command
    int buff_size = 256 + 5 + 2 + 256 + 5 + 256 + 7 + 9 + 5;
    char command_buffer[buff_size + max_length + 1];

    // Calculate the number of possibilities
    long limit = 1;
    for (int i = 0; i < max_length; i++){
        limit *= BASE;
    }

    char password[max_length * 2 + 1];
    for (long i = 0; i < limit; i++) {
        // Create the password from the base 10 number
        base_10_to_other_base(i, password);

        // If password is correct, end and print password + processor time
        if (test_password(password, archive_path, output_path, command_buffer) == 0) {
            printf("Password : %s\nFound in : %f seconds\n", password, (float)(clock() - start_time) / CLOCKS_PER_SEC);
            exit(EXIT_SUCCESS);
        }
    }
    return -1;
}


int main(int argc, char *argv[]) {

    char *input_path = argv[1];
    char *output_path;
    int max_length;
    int c;

    // Get arguments
    while ((c = getopt(argc -1, argv + 1, "o:l:")) != -1) {
        switch (c) {
            case 'o':
                output_path = optarg;
                break;
            case 'l':
                max_length = strtol(optarg, (char **)NULL, 10);
                break;
        }
    }

    // Check if archive exists
    struct stat buff;
    if (stat(input_path, &buff) == -1) {
        fprintf(stderr, "Cannot find the archive!");
        exit(EXIT_FAILURE);
    }
    else {
        printf("Archive found !\nStarting the crack ...\n");
    }

    // Starts the cracking
    if (test_all_passwords(max_length, input_path, output_path) == -1) {
        printf("The correct password couldn't be found.\nMaybe try again with a higher max password size.\n");
    };

    return 0;
}
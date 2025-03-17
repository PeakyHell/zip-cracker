#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#if defined(__unix__) || defined(__APPLE__)
#define OS "UNIX"
#elif defined(_WIN32) || defined(_WIN64)
#define OS "WINDOWS"
#endif

#define BASE 94
#define VALUES "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

char *base_10_to_other_base(int num) {
    // TODO : Fix to use dynamic buffer
    static char buff[50];
    int i = 0;

    // Convert to BASE
    int digit;
    do {
        digit = num % BASE;
        buff[i++] = VALUES[digit];
        num /= BASE;
    } while (num > 0);

    buff[i] = '\0';

    // Reverse string
    char *ptr = buff;
    int len = i;
    for (int j = 0; j < len / 2; j++) {
        char tmp = ptr[j];
        ptr[j] = ptr[len - j - 1];
        ptr[len - j - 1] = tmp;
    }
    
    return ptr;
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
    char *_7zip = strcmp(OS, "UNIX") == 0 ? "7z" : "path to 7zip"; // TODO
    char *no_output = strcmp(OS, "UNIX") == 0 ? "/dev/null" : "NUL";

    char escaped_password[2 * strlen(password) + 1];
    escape_password(password, escaped_password);

    size_t size = snprintf(NULL, 0, "%s x -p%s \"%s\" -o\"%s\" -y > %s 2>&1", _7zip, escaped_password, archive_path, output_path, no_output) + 1;
    snprintf(buffer, size, "%s x -p%s \"%s\" -o\"%s\" -y > %s 2>&1", _7zip, escaped_password, archive_path, output_path, no_output);

    return system(buffer);
}

int test_all_passwords(long max_length, char *archive_path, char *output_path) {
    char *password;
    int buff_size = 256 + 5 + 2 + 256 + 5 + 256 + 7 + 9 + 5;
    char buffer[buff_size + max_length + 1];

    long limit = 1;
    for (int i = 0; i < max_length; i++){
        limit *= BASE;
    }

    for (long i = 0; i < limit; i++) {
        password = base_10_to_other_base(i);
        if (test_password(password, archive_path, output_path, buffer) == 0) {
            printf("Password found : %s", password);
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

    struct stat buff;
    if (stat(input_path, &buff) == -1) {
        fprintf(stderr, "Cannot find the archive!");
        exit(EXIT_FAILURE);
    }

    if (test_all_passwords(max_length, input_path, output_path) == -1) {
        printf("The correct password couldn't be found.\nMaybe try again with a higher max password size.\n");
    };

    return 0;
}
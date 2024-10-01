#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"

void print_header(char *command, int fd) {
    int time = uptime();  // Get current time in ticks
//    char buf[100];
    int len = 0;
    char *header = "\n--- ";
    len += strlen(header);
    write(fd, header, len);
    
    // Convert time to string
    char time_str[20];
    int time_len = 0;
    int temp = time;
    do {
        time_str[time_len++] = (temp % 10) + '0';
        temp /= 10;
    } while (temp > 0);
    // Reverse the string
    for (int i = 0; i < time_len / 2; i++) {
        char t = time_str[i];
        time_str[i] = time_str[time_len - 1 - i];
        time_str[time_len - 1 - i] = t;
    }
    write(fd, time_str, time_len);
    
    char *ticks = " ticks: ";
    write(fd, ticks, strlen(ticks));
    write(fd, command, strlen(command));
    write(fd, " ---\n", 5);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: watch [-n seconds] [-h] [-o output_file] <command>\n");
        exit(1);
    }

    int interval = 2;  // Default interval
    int command_start = 1;
    int show_header = 0;
    char *output_file = 0;

    // Parse options
    while (command_start < argc && argv[command_start][0] == '-') {
        if (strcmp(argv[command_start], "-n") == 0 && command_start + 1 < argc) {
            interval = atoi(argv[command_start + 1]);
            if (interval <= 0) {
                printf("Invalid interval. Using default (2 seconds).\n");
                interval = 2;
            }
            command_start += 2;
        } else if (strcmp(argv[command_start], "-h") == 0) {
            show_header = 1;
            command_start++;
        } else if (strcmp(argv[command_start], "-o") == 0 && command_start + 1 < argc) {
            output_file = argv[command_start + 1];
            command_start += 2;
        } else {
            printf("Unknown option: %s\n", argv[command_start]);
            exit(1);
        }
    }

    if (command_start >= argc) {
        printf("No command specified.\n");
        exit(1);
    }

    int log_fd = 1;  // Default to stdout
    if (output_file) {
        log_fd = open(output_file, O_WRONLY | O_CREATE | O_TRUNC);
        if (log_fd < 0) {
            printf("Failed to open output file: %s\n", output_file);
            exit(1);
        }
    }

    int paused = 0;
    printf("Watch started. Press 'p' to pause/unpause, 'q' to quit.\n");

    while (1) {
        // Check for user input
        char input;
        if (read(0, &input, 1) > 0) {
            if (input == 'p' || input == 'P') {
                paused = !paused;
                printf(paused ? "Paused. Press 'p' to resume.\n" : "Resumed.\n");
                continue;
            } else if (input == 'q' || input == 'Q') {
                printf("Quitting...\n");
                break;
            }
        }

        if (!paused) {
            if (show_header) {
                print_header(argv[command_start], log_fd);
            }

            int pid = fork();
            if (pid < 0) {
                printf("Failed to fork\n");
                exit(1);
            }

            if (pid == 0) {
                // Child process
                if (output_file) {
                    // Redirect stdout and stderr to the log file
                    close(1);
                    close(2);
                    dup(log_fd);
                    dup(log_fd);
                }
                exec(argv[command_start], &argv[command_start]);
                printf("Failed to execute command: %s\n", argv[command_start]);
                exit(1);
            } else {
                // Parent process
                wait(0);
            }
        }
        sleep(interval * 10);  // sleep takes ticks, 1 second = 10 ticks
    }

    if (output_file) {
        close(log_fd);
    }
    exit(0);
}

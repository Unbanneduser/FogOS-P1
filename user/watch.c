#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"
#include "kernel/fs.h"

void print_header(char *command, int fd, uint64 start_time) {
    fprintf(fd,"\n---%d ms:%s ---\n",(uptime()-start_time)*100,command);
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
    uint64 start_time=uptime();

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

    // int paused = 0;
    printf("Watch started. Press 'Ctrl-c' to pause and quit.\n");
    while (1) {
        if (show_header) {
            print_header(argv[command_start], log_fd, start_time);
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
        sleep(interval * 10);  // sleep takes ticks, 1 second = 10 ticks
    }

    if (output_file) {
        close(log_fd);
    }
    exit(0);
}

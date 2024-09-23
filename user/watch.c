#include "kernel/types.h"
#include "kernel/stat.h" 
#include "user/user.h" 



int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: watch <command>\n");
        exit(1);
    }
    // Run the provided command in an infinite loop
    printf("%d\n",argc);
    printf("%s\n",argv);
    while (1) {
        
        int command = fork();
        if (command < 0) {
            // Fork failed
            printf("Failed to fork\n");
        }

        if (command == 0) {
            // Child process
            exec(argv[1], &argv[1]);
            // If exec fails, print an error and exit the child
            printf("Failed to execute command: %s\n", argv[1]);
            exit(1);
        } else {
            // Parent process waits for the child to finish
            int status;
            wait(&status);

            // Sleep for 2 seconds (sleep takes argument in ticks, 1 tick = 100ms, so 20 ticks = 2 seconds)
            sleep(10);
        }

    exit(0);  // Exit with success status
    }

}

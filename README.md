# FogOS

![FogOS](docs/fogos.gif)

# Watch Command Documentation

@author zguo18
@author yhatnagar 

## Overview

The custom `watch` command implemented in OS4 allows you to execute a specified command repeatedly at set intervals. The command comes with optional features for logging the output, displaying a header, and controlling the execution through pause and quit options.

## Features

1. Customizable Interval (`-n`): Specify the interval in seconds between command executions. The default interval is 2 seconds.

2. Display Header (`-h`): Option to display the current timestamp in ticks before each command execution.

3. Log Output (`-o`): Write the output of the command to a specified file instead of displaying it in the console.

Pause and Quit (Ctrl+C): Press Ctrl+C to both pause and quit the command. When Ctrl+C is pressed, the system interrupts and stops the execution entirely. 

## Command Options implemented

-n <interval>: Set interval in seconds between command executions.
-h: Display a timestamped header before each execution.
-o <file>: Write output to a file instead of the console.


## Command Examples

Here are some examples of how to use the custom `watch` command:

- watch ls 
  
This runs the `ls` command every 2 seconds and prints the output to the console.

- watch -n 5 ls

This runs the `ls` command every 5 seconds instead of the default 2 seconds.


- watch -h -n 3 echo "Hello"

 This runs the `echo "Hello"` command every 3 seconds and displays a timestamp header before each execution.


- watch -o log.txt ls 
 
This runs the `ls` command every 2 seconds and writes the output to `log.txt` instead of printing it to the console.

- watch -n 5 -h -o date_log.txt date

This runs the `date` command every 5 seconds, displays a timestamped header, and logs the output to `date_log.txt`.


The watch command was implemented in C in an OS environment. It uses system calls such as fork(), exec(), and uptime() to execute the commands at set intervals, manage processes, and retrieve system time in ticks. 

The command also supports logging output to files by redirecting standard output and error streams to the specified file. For the pause and quit functionality, user input handling is employed to capture keypresses, though some functionality may depend on the specific OS shell configuration.

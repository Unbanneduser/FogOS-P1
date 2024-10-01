# FogOS

![FogOS](docs/fogos.gif)

Watch Command Documentation

Overview:
The custom watch command in OS4 executes a specified command repeatedly at set intervals with optional features for logging and displaying a header.

Features:
Customizable Interval (-n): Specify interval in seconds between executions (default is 2 seconds).
Display Header (-h): Show current timestamp before each execution.
Log Output (-o): Write command output to a file instead of the console.
Pause and Quit: Using ctrl + c to pause and quit execution.

Command Options:
-n <interval>: Set interval in seconds between command executions.
-h: Display a timesztamped header before each execution.
-o <file>: Write output to a file instead of the console.

Examples:
watch ls
watch -n 5 ls
watch -h -n 3 echo "Hello"
watch -o log.txt ls
watch -n 5 -h -o date_log.txt date



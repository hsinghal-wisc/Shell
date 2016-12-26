# Shell

The shell should operate in this basic way: when you type in a command (in response to its prompt), the shell creates a child process that executes the command you entered and then prompts for more user input when it has finished.

You will need to be able to handle running multiple commands simultaneously.
Your shell can be run in two modes: interactive and batch.
Jobs may be executed in either the foreground or the background. 
Each job that is executed by your shell should be given its own unique job id.

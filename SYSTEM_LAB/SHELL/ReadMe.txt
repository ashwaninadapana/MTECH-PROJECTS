HOW TO RUN:
	1)execute './CS558Assign1.o'

COMMANDS:
	1) mkdir
	2) help
	3) >
	4) >>
	5) cat
	6) exit
	7) cd

PROGRAM FLOW:
	1) CS558Assign1.cpp is the main program where a parent thread is created.
	2) loads the configuration "loadConfiguration()"
	3) starts the shell "startShell()" and waits for the user to input some command.
	4) If the input command is valid, it creates a child where it further executes the command using execvp()
	5) execvp executed object file "exec" of child_executor.cpp file.
	6) The child process checks whether the command has output redirection operator or not, and take appropriate actions

FUNCTION DESCRIPTIONS:
~~~~~~~~~~~~~~~~~~~~~~~~~~
CS558Assign1.cpp  
	----loadConfiguration():
		commandsArray is the array of strings where each string is of the format "<command>: <text file of the help related to the command>"
		This function splits the above string and saves them in a map "help" and vector "validCommands".
		retrieves the user information and hostname information and currentpath information
	----startShell():
		uses the user, hostname and currentpath information and creates a message and prints on the shell
		waits for the user to enter the command 
		After the command is entered, it parses the command using "parseCommand()", validates whether the command is valid or not by checking in the 				"validCommands" vector. Also checks if the command arguments size is not greater than 10 other than command
		If the command is valid, execute it.
	----parseCommand():
		Parses the entered command using spaces and populates "commandArgs" vector. So spaces are must when you enter the command.
	----clearCommandCache():
		Clears the commandArgs vector
	----executeCommand()
		Compares the command against all valid commands present and calls appropriate functions

child_executor.cpp
	----calculateRedirectionIndicesIfPresent():
		 Checks if the output redirection operator is present or not  and stores the value in "redirectionPresent" global variable.
		 stores the index where the first redirection operator is present and also stores the last redirection operator's index.
	----generateTop():
		The directory names present in the /proc directory are the process ids{pid}
		 Lists the files in the /proc directory and checks if the directory name is a number or not.
		 If the directory name is a number, print the contents of /proc/{pid}/status file.
	----redirectConsoleOutput():
		The command which has one of the redirection operators > or >> enters this function
		For the commands like cat and top, dup2 is used. For any other command, popen is used.
		dup2 redirects the console output to the file descriptor passed as first argument
		file descriptor is nothing but the filename passed after the last redirection index.
		popen executes any other system command
	----changeDirectory():
		changes the current working directory to the path passed as argument if it is valid.
		if the argumenta are more than 2, it gives error 
		If the argument is not given, it goes to "HOME" directory
	----mkdirCall():
		Checks if the directory is present or not.
		Otherwise calls createDirectory
	----createDirectory():
		Splits the directory name at '/' from the start 
		Check if the directory is present or not. If not present, creates the directory. Otherwise split the name at the next '/' and repeat this step
	----isCatCommandValid():
		checks if the cat command is valid
		if "cat" is given, waits for the user to enter the input and when the user enters the new line, the last statement is printed again.
		It accepts the input until CNTRL+D is pressed.
		if "cat > filename or cat >> filename" is given as the command, it reads the input from the user and writes or appends into the filename respectively
		if "cat filename" is given, it prints the content of the file on the console.
		Multiple filenames can also be given. If any of the file is missing, it gives the error corresponding to those files in the console.
		if filenames with multiple redirection operators and filenames given, it redirects the output to the last file.

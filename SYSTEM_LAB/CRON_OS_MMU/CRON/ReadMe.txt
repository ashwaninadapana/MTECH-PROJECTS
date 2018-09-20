How to run:
	./cron.out 

Input file:
	/myhome/crontab
	
SUPPORTED: -,/* JAN-DEC SUN-SAT


FUNCTION DESCRIPTION:
	fileChangeListener() : added watch over the configuration file and read call blocks until the file is modified.
				If the file is modified, it kills all the started threads and re-process the file again
	parseCronFile()      : parses the configuration file and adds the valid commands to conf_list vector
	getDayOfWeek()       : returns the assigned value of day of week in integer format(SUN-0, MON-1, TUE-2, WED-3.... SAT-6)
	getMonth() 	     : returns the assigned value of Month in integer format(JAN-1,FEB-2,.....DEC-12)
	isWildCardCharacter(): Checks if the field contains *
	isComment()          : checks if the line is a comment.i.e starts with #
	isSlashPresent()     : checks if there is a slash character
	isHiphenPresent()    : checks if there is a hiphen
	getActualValue()     : returns the integer value of month or day of week or integer itself. If it is invalid, it returns -1
	validateFields(), validateToken(), checkAllPossibilitiesForToken(): checks if the configuration fields are valid so that it can be added to conf_list
	trim()               : trims the space present at the ends
	processTheLine()     : process the line in the file and add to the conf_list if valid
	parseCronFile()	     : reads the file
	checkIfValuesMatches() : checks if the current time matches the time when the command has to be executed
	checkIfValuesexceeds(): checks if the execution time is already over
	checkIfSchedulable(): Checks if the command can be schedulable
	runCurrentCommands(): creates a thread for each task
	getDelayForTheStartOfVeryNextMin(): returns the delay for the first time execution so that the commands run at <min>:00
	makeItbackgroundCommand(): adds & at the end and run it as background process

	

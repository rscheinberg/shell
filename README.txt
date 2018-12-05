Reed_Shell - Reed Scheinberg

Features:
	- Run any bash program, including cd and exit
	- Use as many semicolons as wanted (;) for consecutive commands
	- Redirection with > and <

Attempted:
	- Pipes do not work :(
	- Only one redirection can be used in a command at a time (i.e. a > b would work but not a < b > c)
	- Redirection with < and > must be its own argument (i.e. a > b would work but not a>b)
	
Bugs:
	- The program ocassionally does not like commands with no additional arguments (i.e. ls on its own might give you a hard time every once in a while, it seems to work as of December 3)
	- Shell will either crash or do wacky stuff if the guidelines for input in "Attempted" are not followed (in simpler terms, my program does not check to make sure the user's input meets the standards I have made)
	
Headers:
/*======== main ==========
Inputs: N/A
Returns: N/A, but it runs the Reed_Shell
====================*/

/*======== cd ==========
Inputs: @path to the directory the process is being changed to
Returns: N/A, but it changes the directory and prints an error if needed
====================*/

/*======== remove_spaces ==========
Inputs: @line being modified
Returns: The modified line with the spaces at either end of the string removed
====================*/

/*======== parse_args ==========
Inputs: @line being parsed, and the number of @argc spaces in that line
Returns: The properly parsed line in char ** form
====================*/

/*======== leave ==========
Inputs: N/A
Returns: N/A, but prints a friendly goodbye message
====================*/

/*======== charfind ==========
Inputs: the char @c being searched in string (char array) @s
Returns: The number of times @c appears in @s
====================*/

/*======== semicolons ==========
Inputs: The @input being separated by semicolons
Returns: An array of arrays of strings, separated by semicolons, with each element being in a form similar to args from the command line
====================*/

/*======== find_redir ==========
Inputs: The parsed @commands (separated by semicolon), and @i, the element in commands that is currently being used by the program
Returns: A redir struct containing information about the location and type of the redirection symbols (< and >)
====================*/

/*======== redir ==========
Inputs: The @args of the current command, the redirect @type, and the @file that is being accessed with the redirection
Returns: N/A, but it does exec the program for you depending on the type of redirection used
====================*/

/*======== execreedvp ==========
Inputs: The @args of the current command being used, and a redir struct, @redir_info , containing redirect information about that command
Returns: N/A, but it does run the redirection using redir()
====================*/
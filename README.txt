# Socket Programming

	## Instructions to Run the code
		- In Server Directory :
			-> gcc server.c -o server
			-> ./server 9898
		- In Client Directory :
			-> gcc client.c -o client
			-> ./client 127.0.0.1 9898

	## Error Handling
		- if port number is not given :
			-> server prints "Port number not provided--Program terminated"
			-> client prints "usage ./client hostname port"
		- if it could not open socket :
			-> both server and client prints "Error opeing socket"
		- if it couldn't read or write the file :
			-> server prints "Error on reading"
			-> client prints "Error on writing"
		- if file doesn't exist :
			-> server prints "Sorry client but the file does not exist"
			-> client prints "File not accessible or File not present"
		- if invalid command is given :
			-> both print "get command not used:try once again using get command"



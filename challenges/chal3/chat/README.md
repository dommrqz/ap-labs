# Challenge 3
## Jorge Dominic Márquez Muñoz, A00569034

### IRC chat.
### Compiling instructions:
	1. go run server.go -host <host> -port <port number> (in one terminal)
    2. go run client.go -user <name> -server <server> (in another terminal)
        2.1 IMPORTANT: press enter in the current terminal once before making anything else.
    3. go run client.go -user <another name> -server <server> (in another terminal)
        3.1 IMPORTANT: press enter in the current terminal once before making anything else.

### Testing code:
    1. Type any message from the client side to see its broadcast.
    2. /users (list of users will appear)
    3. /time (time will be printed)
    4. /user <username> (info about the user will be put)
    5. /msg <username> <message> (sends a message only to that user)
# Lab 3.2
## Jorge Dominic Márquez Muñoz, A00569034

### Base64 algorithm encoder/decoder with progress notifier through SIGINT and SIGPWR signal.
### Compilation instructions:

#### This lab includes a Makefile, so can be compiled and cleaned by
	1. make
	2. make clean
#### For manual compiling
	1. gcc -c logger.c -o logger.o
	2. gcc -c base64.c -o base.o
	3. gcc logger.o base.o -o base64.o
### Testing code:
#### (only manual testing is provided, since it will be easier to visualize the result).
    1. ./base64.o --encode [vigilante.txt | sick-kid.txt | aesop11.txt]
    2. (in another terminal) ps aux | grep base64 (take the pid of the first process that appears)
    3. kill -s [SIGINT | SIGPWR] (progress will appear in the terminal where base64 is running)
    4. ./base64.o --decode encoded.txt
    5. (in another terminal) ps aux | grep base64 (take the pid of the first process that appears)
    6. kill -s [SIGINT | SIGPWR] (progress will appear in the terminal where base64 is running)
    7. cat decoded.txt (to see it is identical as the file that was encoded in step 1)
#### References:
    1. Base64 algorithm taken from: http://fm4dd.com/programming/base64/base64_stringencode_c.htm

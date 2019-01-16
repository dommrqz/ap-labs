string = raw_input("Enter string: ")
count = 0
used_letters = list()
longest_substring = ""
temp_string = ""

temp_string += string[0]

for i in range(1, len(string)):

    if string[i] in temp_string:
        temp_string = ""

    temp_string += string[i]

    if (len(temp_string) > len(longest_substring)):
        longest_substring = temp_string
    

print ("Length is: " + str(len(longest_substring)))
print ("Substring is: " + longest_substring)
    
#RSHELL Project

##Summary

**Darren Houn**
**Shawn Wu**
*dhoun001, swu038*

Throughout the project, we have implemented the process of taking in a
command line and processing the command. Furthermore, the program will
execute the command line action through execvp. We use fork and wait
to handle the child process so that the process can return to the parent
process when execvp finishes. Additionally, we handle connectors and
their logic to keep calling additional functions. Also, test cases are
used to test the program functionality.

###Known Bugs

1. Taking in a exit after || does not work
  * need special logic
2. Cannot tokenize a string and manipulate it within the same vector
  * will BREAK the tokenizer
  * will also ruin changing string to cstring
3. The || connector cannot be tested for in the child process
4. Having the || connector before many cases is tough to handle
5. Cannot pass a pointer to char pointer to execvp, must const cast
6. Exit functions must be implemented before calling exec
  * exec returns error
7. Quotes and backslashes are not accounted for
8. Having multiple || together requires tougher logic
9. Compiler throws errors for comparing unsigned ints to ints
10. tokenizer cannot handle connector logic because of double appearance
  * use own separator for connectors and comments
11. Need to us WEXITSTATUS to see the return value of wait, could not 
  * (con) implement || connector logic without this.
12. Exit works on random occasions, generally works with && but sometimes
  * (con) with || and ; depending on combination
13. Pressing the enter key as a command will cause the program to abort. 
14. The test logic works in most cases, but wont work with [] as a single function.
15. The [] as a single function creates a segmentation fault.
16. Also the parenthesis logic may not work with the test logic.
17. This is a flaw with our logic implementation as we were too late on realizing it.
18. Additionally, the implementation of the test logic may coincidentally harm the other logic.
19. This is cuased my a miscommunication between the members.

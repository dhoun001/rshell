#!/bin/sh
#tests exit and commands with exit 

#testing just exit: 
exit 

#testing with one command ; exit:
ls ; exit 

#testing with one incorrect command ; exit:
asd ; exit 

#testing with one incorrect command ; exit:
asd ; exit

#testing with one command && exit:
echo && exit 

#testing with one incorrect command && exit:
dsfa && exit

#testing with one incorrect command || exit:
esad || exit

#testing with one command || exit:
ls || exit       *failed 

#testing with one parameter ; exit: 
ls -a ; exit 

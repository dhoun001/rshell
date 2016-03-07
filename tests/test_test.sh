#!/bin/sh
#tests for the test command 

#test to see an existing file if it exists and is a regular file
test -f src 

#test to see an existing file if it exists 
test -e src

#test to see an existing file if it exists and if it is a directory
test -d src 

#testing with a non-existing file to see if it exists 
test -e asdfdsfsadfa

#testing with a non-existing file to see if it exists and is a regular file
test -f sadfdaa

#testing with a non-existing file to see if it exists and is a directory
test -d dafdafdfa

#testing multiple commands with two existing files.
test -f src ; test -d src
test -e src && test -d src
test -f src || test -e src

#testing multiple commands with one existing file and one non-existing file
test -f src ; test -d adsfdafa
test -f src ; test -e asdfdsafd
test -e src || test -d adskfds
test -f src && test -f sadfdsaf

#testing multiple commands both with non-existing files
test -f sasdfrc ; test -d adsfdafa
test -f sdafrc ; test -e asdfdsafd
test -e ssdfdfsdrc || test -d adskfds
test -f swerrwc && test -f sadfdsaf

#testing with three types of files
test -e asdfdafa || test -f asdfdasf && test -d ewrewrewr
test -f src && test -f 33433 || test -d weadsfdfa
test -f src || test -e src && test -d src
test -d adsfdas || test -e src || test -f 24222
test -e src && test -e src && test -e src

#testing with brackets
[ -e src ] 
[ -d src ] 
[ -f src ]
[ -e asdf ]
[ -d asdf ]
[ -f asdfad ]

#testing with brackets with multiple commands
[ -e src ] && echo "yo" 
[ -f src ] || ls 
[ -d src ] ; ls -a 
[ -e asdf ] && ls 
[ -f sadf ] || echo "hi"
[ -d asdf ] ; [ -f src ] 

  



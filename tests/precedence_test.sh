#!/bin/sh
#tests for precedence operators

#testing with a single parenthesis 
(echo "A" && ls) 
ls && (echo "Hi" || echo "YO") 
echo "hi" || (ls && ls -a) 
ls ; (ls ; ls) 
ls || (ls ; ls) || ls 
echo "y" && (ls || mkdir this) ; cd this 
ls ; ( asd || ls -a) && echo "yo"
ls -a && (ls && echo "hi") ; ls || mkdir this 

#testing with incorrect commands plus single parenthesis
(esdfcho "A" && ls) 
sadfls && (echo "Hi" || ecsadfho "YO") 
echo "hi" || (ls && ls -a) 
ls ; (ladsfs ; ls) 
ls || (ls ; lasdfs) || ls 
ls || (asdf ; adsf) || adsf 

#testing with multple parentheses 
(ls && ls -a) || (mkdir this && cd this) 
(echo "hi" ; echo "yo") && (ls ; ls -a) 
(ls || ls) ; (ls && adsf) 
(asdf || ls) ; (mkdir this || test -f src) 

#testing with parentheses within parentheses 
((ls && mkdir test) || (echo "run" ; echo "hi")) ; ls -a && (ls && echo "yo")
(ls || asdf ) ; (( echo "hi" ; ls) || test -f src) 
(ls ; ls) && (((echo "hi" && ls) || (test -f src) ; mkdir this) && ls) 

#testing with incorrect number parentheses 
((ls && ls) 
(((((ls || echo))) || echo ( ls ; ls ) 
(ls ; ls ))) 


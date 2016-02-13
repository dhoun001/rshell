#!/bin/sh
#test commands with ;, &&, or || 

#testing ; with correct commands: 
    ls ; echo 

#testing ; with incorrect commands: 
    lasd ; adsf

#testing ; with one correct and one incorrect command:
    asdf ; ls 
    echo ; asdf

#testing || with correct commands: 
    echo || ls 

#testing || with incorrect commands: 
    add || sdklf

#testing || with one correct and one incorrect command: 
    ls || asdf
    asdf || ls 

#testing && with correct commands: 
    ls && echo 

#testing && with incorrect commands: 
    asdf && dfa

#testing && with one correct and one incorrect: 
    ads && ls
    ls && asdf

#testing with || and ; 
    ls ; echo || mkdir star 
    asdf ; echo || ls 
    ls ; asdf || echo 
    asdf ; sdf || adf
    ls || echo ; mkdir star     
    234 || echo ; adsf
    echo || asdf ; lsi
    asd || asdf ; asdf 

#testing with || and &&
    ls && echo || mkdir star 
    asdf && echo || ls 
    ls && asdf || echo 
    asdf && sdf || adf
    ls || echo && mkdir star     
    234 || echo && adsf
    echo || asdf && ls
    asdf || asdf && asdf
    
#testing with ; and &&
    ls ; echo && mkdir star 
    asdf ; echo && ls 
    ls ; asdf && echo 
    asdf ; sdf && adf
    ls && echo ; mkdir star     
    234 && echo ; adsf
    echo && asdf ; lsi
    asd && asdf ; asdf 

#testing with || and || 
    ls || echo || mkdir star 
    asdf || echo || ls 
    ls || asdf || echo 
    asdf || sdf || adf
    ls || echo || mkdir star     
    234 || echo || adsf
    echo || asdf || lsi
    asd || asdf || asdf 

#testing with ; and ;
    ls ; echo ; mkdir star 
    asdf ; echo ; ls 
    ls ; asdf ; echo 
    asdf ; sdf ; adf
    ls ; echo ; mkdir star     
    234 ; echo ; adsf
    echo ; asdf ; lsi
    asd ; asdf ; asdf 

#testing wiht & and &
    ls && echo && mkdir star 
    asdf && echo && ls 
    ls && asdf && echo 
    asdf && sdf && adf
    ls && echo && mkdir star     
    234 && echo && adsf
    echo && asdf && lsi
    asd && asdf && asdf 

#testing with long chains: 
    ls ; echo ; ls && mkdir this adsf || ls -a ; vim main.cpp 
    echo && ls -a ; ls -a -l ; ls -a -l -h && ads || echo || ls




  
 

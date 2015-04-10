#!/bin/bash
echo "Removing the GNU Readline Directory"
rm -rf readline/
echo "Removing Object files, Lex Generated Code, and Yacc Generated Code"
rm -rf lex.yy.c y.tab.c y.tab.h main shell *.o *.output *.gch

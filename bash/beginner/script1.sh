#! /bin/bash
# script end in .sh to ensure no conflicts with existing commands.
# still, use `which -a`, `whereis`, `locate` to double check.

# `which env` + bash
echo "The script starts now."

echo "Hi, $USER!"
echo

echo "I will now fetch you a list of connected users:"
echo
set -x
# debugging mode
# set -f == set -o noglob
# set -v == set -o verbose
# set -x == set -o xtrace
w
set +x
echo

echo "This is an non-interactive shell, see \$-"
echo $$ $0 $-
echo

echo "I'm setting two variables now."
COLOUR="black"
VALUE="9"
echo "This is a string: $COLOUR"
echo "This is a number: $VALUE"
echo

echo "Exercises:"

echo "1. Write a script using your favorite editor. The script should display the path to your homedirectory and the terminal type that you are using. Additionally it shows all the services started up in runlevel 3 on your system. (hint: use HOME, TERM and ls /etc/rc3.d/S*)"
echo "$HOME"
echo "$TERM"
set +f
echo "`ls /etc/rc3.d/S*`"
echo


echo "I'm giving you back your prompt now."
echo

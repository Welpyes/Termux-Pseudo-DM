#!/bin/bash

# grab the binary from github and shove it in PATH
echo "Downloading the display manager binary..."
curl -L https://github.com/Welpyes/Termux-Pseudo-DM/releases/download/release/dm -o /data/data/com.termux/files/usr/bin/dm
chmod +x /data/data/com.termux/files/usr/bin/dm # make it runnable
echo "Binary’s in \$PATH now at /data/data/com.termux/files/usr/bin/dm"

# set up the .dmrc template
DMRC="$HOME/.dmrc"
echo "Making a fresh .dmrc in $HOME..."
cat > "$DMRC" << EOL
username = 
pwd = 
cmd = am start --user 0 -n com.termux.x11/com.termux.x11.MainActivity > /dev/null 2>&1
EOL

# ask for username and password
clear
echo "Let’s set up your login stuff"
read -p "What’s your username? " user
read -s -p "What’s your password? " pass # -s hides the input
echo # just a newline after hidden input

# slap them into .dmrc
sed -i "s/username = /username = $user/" "$DMRC"
sed -i "s/pwd = /pwd = $pass/" "$DMRC"

echo "All set! Your .dmrc is ready at $DMRC"
echo "Run 'dm' to start the display manager"

main() {
	# Use colors, but only if connected to a terminal, and that terminal
	# supports them.
	if which tput >/dev/null 2>&1; then
			ncolors=$(tput colors)
	fi
	if [ -t 1 ] && [ -n "$ncolors" ] && [ "$ncolors" -ge 8 ]; then
		RED="$(tput setaf 1)"
		GREEN="$(tput setaf 2)"
		YELLOW="$(tput setaf 3)"
		BLUE="$(tput setaf 4)"
		BOLD="$(tput bold)"
		NORMAL="$(tput sgr0)"
	else
		RED=""
		GREEN=""
		YELLOW=""
		BLUE=""
		BOLD=""
		NORMAL=""
	fi

	# Only enable exit-on-error after the non-critical colorization stuff,
	# which may fail on systems lacking tput or terminfo
	set -e

	if [ ! -n "$INITDEV" ]; then
		# Check if initdev directory is within homepath
		INITDEV=~/.initdev
	fi

	if [ -d "$INITDEV" ]; then
		printf "${YELLOW}You already have initdev installed.${NORMAL}\n"
		printf "You'll need to remove $INITDEV if you want to re-install.\n"
		exit
	fi

	hash g++ >/dev/null 2>&0 || {
		apt-get install g++
		echo "Trying to install g++ via apt package manager"
		exit 1
	}
	

	# Check if g++ is installed
	hash g++ >/dev/null 2>&1 || {
		echo "Error: G++ (C++ compiler) is not installed"
		echo "Trying to install via apt package manager"
		exit 1
	}
	
	# Prevent the cloned repository from having insecure permissions. Failing to do
	# so causes compinit() calls to fail with "command not found: compdef" errors
	# for users with insecure umasks (e.g., "002", allowing group writability). Note
	# that this will be ignored under Cygwin by default, as Windows ACLs take
	# precedence over umasks except for filesystems mounted with option "noacl".
	umask g-w,o-w

	printf "${BLUE}Cloning Init Dev...${NORMAL}\n"
	hash git >/dev/null 2>&1 || {
		echo "Error: git is not installed"
		exit 1
	}
	# The Windows (MSYS) Git is not compatible with normal use on cygwin
	if [ "$OSTYPE" = cygwin ]; then
		if git --version | grep msysgit > /dev/null; then
			echo "Error: Windows/MSYS Git is not supported on Cygwin"
			echo "Error: Make sure the Cygwin git package is installed and is first on the path"
			exit 1
		fi
	fi
	env git clone --depth=1 https://github.com/bsoumeya/initdev_cc.git $INITDEV || {
		printf "Error: git clone of init dev repo failed\n"
		exit 1
	}
	if [ ! -f "$INITDEV/bin/initdev" ]; then
		g++ $INITDEV/src/main.cpp -o $INITDEV/bin/initdev
	else
		g++ $INITDEV/src/main.cpp -o $INITDEV/bin/initdev1
	fi
	# CHECK_ZSH_INSTALLED=$(grep /zsh$ /etc/shells | wc -l)
	# if [ ! $CHECK_ZSH_INSTALLED -ge 1 ]; then
	# 	echo "export PATH='$INITDEV/bin:$PATH'" >> ~/.zshrc
	# else
	# 	echo "export PATH='$INITDEV/bin:$PATH'" >> ~/.bashrc
	# fi
	# unset CHECK_ZSH_INSTALLED

	if [ -n "$ZSH_VERSION" ]; then
		# export path only if not already there
		echo "export PATH='$INITDEV/bin:$PATH'" >> ~/.zshrc
	elif [ -n "$BASH_VERSION" ]; then
		echo "export PATH='$INITDEV/bin:$PATH'" >> ~/.bashrc
	fi
	printf "${GREEN}"
echo	"	(         (   ( /(    )\ )   (    )            "
echo	"	)\   (    )\  )\())  (()/(  ))\  /((           "
echo	"	((_)  )\ )((_)(_))/    ((_))/((_)(_))\         "
echo	"	(_) _(_/( (_)| |_     _| |(_))  _)((_)         "
echo	"	| || ' \))| ||  _|  / _\` |/ -_) \ V /         "
echo	"	|_||_||_| |_| \__|  \__,_|\___|  \_/           ....is now installed!"
																								
	printf "${NORMAL}"
}

main
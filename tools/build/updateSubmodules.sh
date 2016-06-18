#!/bin/bash
################################################################################
#            ___________      .__.__  .__  .__               __                #
#            \__    ___/______|__|  | |  | |__|____    _____/  |_              #
#              |    |  \_  __ \  |  | |  | |  \__  \  /    \   __\             #
#              |    |   |  | \/  |  |_|  |_|  |/ __ \|   |  \  |               #
#              |____|   |__|  |__|____/____/__(____  /___|  /__|               #
#                                                  \/     \/                   #
#                                         Networks Inc.                        #
# File:    updateSubmodules.sh                                                 #
#                                                                              #
# This script is used to update git submodules for this repository.            #
#                                                                              #
# Author:  Jérémie Faucher-Goulet                                              #
#                                                                              #
#   Copyright © Trilliant Inc. 2016                                            #
#   www.trilliantinc.com                                                       #
#                                                                              #
################################################################################

# Figure out directory where this script is located
SOURCE="${BASH_SOURCE[0]}";
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
  DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
  SOURCE="$(readlink "$SOURCE")";
  [[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE"; # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";


################################################################################
# Configuration constants
################################################################################
# Git repository
BUILD_GIT_REPO=PUT_GIT_HOST_HERE
# User to be used for GIT access
BUILD_GIT_USER=PUT_GIT_USER_HERE
# Password to be used with BUILD_GIT_USER
BUILD_GIT_PASSWORD=PUT_GIT_PASSWORD_HERE

#################################################################################
# Functions
#################################################################################

main () {
	# Look for Git installation    
	gitSetup;

	# Update submodules
	updateSubmodules;

	echo ""
	echo "Execution successful!";
}

updateSubmodules () {
	echo ""
	echo "[Updating submodules]";

	cd "$DIR";

	git submodule foreach --recursive git reset --hard;
	git submodule foreach --recursive git clean -fdx;
	git submodule init;
	git submodule sync;

	gitCredentialsSetup;
	git submodule update --init --recursive;
	if [ $? -eq 0 ]; then
		gitCredentialsTeardown;
	else
		gitCredentialsTeardown;
		error "Failed to update submodules";
	fi
}

gitCredentialsSetup () {
	if [ -n "$JENKINS_HOME" ]; then
		# File where the credentials will be stored and read by Git
		GIT_CRED_FILE=$(echo "$WORKSPACE/$JOB_NAME.credentials");

		# Create the proper path based on OS... Git only support path defined unix-style
		if [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
			UNIX_GIT_CREDS=$GIT_CRED_FILE;
		elif [ "$(expr substr $(uname -s) 1 5)" == "MINGW" ]; then
			# Convert path to the credential file from Windows-style to Unix-style, needed for the git configuration.
			UNIX_GIT_CREDS=$(echo "/$GIT_CRED_FILE" | sed -e 's/\\/\//g' -e 's/://');
		else
			error "Failed to identify OS";
		fi
		
		# Configure git to use a credential file.
		git config --global credential.helper 'store --file='\""$UNIX_GIT_CREDS"\";
		
		# Fill in the credentials using stored global passwords in Jenkins
		echo https://$BUILD_GIT_USER:$BUILD_GIT_PASSWORD@$BUILD_GIT_REPO > "$GIT_CRED_FILE";
	fi
}

gitCredentialsTeardown () {
	if [ -n "$JENKINS_HOME" ]; then
		# Destroy stored credentials
		git config --global --remove-section credential;
		rm -f "$GIT_CRED_FILE";
	fi
}

gitSetup () {
	echo "";
	echo "[Git configuration]";

# Looking for a Git installation by trying the following:
# 1. Using the windows command "where".
# 2. Using the Linux command "which".
# 3. Looking into the default install path on windows.

    $(cmd //c 'where /q git' 2> /dev/null);
	if [ $? -eq 0 ]; then
		echo "Found Git through 'where'";
		# Get Windows "where" result
		local where=$(cmd //c 'where /f git');
		# Remove double quotes from first occurrence
		where=$(echo $where | sed 's/"\([^"]*\)"/\1/');
		# Remove other occurrences with double quotes
		where=$(echo $where | sed 's/"\([^"]*\)"//');
		# Remove trailing white spaces
		GIT_EXEC=$(echo $where | sed 's/\s$//g');
	elif [ -n "$(which git 2> /dev/null)" ]; then
		echo "Found Git through 'which'";
		GIT_EXEC=$(which git);
	elif [ -e "C:\Program Files\Git\bin\git.exe" ]; then
		echo "Found Git through windows default installation path";
		GIT_EXEC="C:\Program Files\Git\bin\git.exe";
	elif [ -e "C:\Program Files (x86)\Git\bin\git.exe" ]; then
		echo "Found Git through windows default installation path";
		GIT_EXEC="C:\Program Files (x86)\Git\bin\git.exe";
	fi

	# Checking if Git was found. Else, quit the script.
	if [ -n "$GIT_EXEC" ]; then
		echo "Found Git here: "$GIT_EXEC;
		export GIT_HOME=$(dirname "$(dirname "$GIT_EXEC")");
		PATH="$GIT_HOME/bin:$PATH";
		git --version;
	else
		error "Git Not Found";
	fi
}

################################################################################
# Helper functions
################################################################################

error () {
	echo "ERROR: $1";
	exit 1;
}

warning () {
	echo "WARNING: $1";
}

################################################################################
# Internal constants
################################################################################

SCRIPT_NAME="$(basename $BASH_SOURCE)";

################################################################################
# Execution starting point
################################################################################

# Welcome message
echo "[Running $SCRIPT_NAME]";

# Asking for help?
if [[ ( $1 == "--help") ||  $1 == "-h" ]]; then
	echo "$SCRIPT_NAME updates the git submodules.";
	echo "Usage: $SCRIPT_NAME";
	echo "";
	exit 0;
fi

# Run this script
main;

# Exit message
echo "[Exiting $SCRIPT_NAME]";
exit 0;

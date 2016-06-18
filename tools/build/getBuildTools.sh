#!/bin/bash
#################################################################################
#            ___________      .__.__  .__  .__               __                 #
#            \__    ___/______|__|  | |  | |__|____    _____/  |_               #
#              |    |  \_  __ \  |  | |  | |  \__  \  /    \   __\              #
#              |    |   |  | \/  |  |_|  |_|  |/ __ \|   |  \  |                #
#              |____|   |__|  |__|____/____/__(____  /___|  /__|                #
#                                                  \/     \/                    #
#                                         Networks Inc.                         #
# File:    getBuildTools.sh                                                     #
#                                                                               #
# Script to fetch build tools from Git                                          #
#                                                                               #
# Author:  Jérémie Faucher-Goulet                                               #
#                                                                               #
#   www.trilliantinc.com                                                        #
#                                                                               #
# The MIT License (MIT)                                                         #
#                                                                               #
# Copyright (c) 2015 Trilliant                                                  #
#                                                                               #
# Permission is hereby granted, free of charge, to any person obtaining a copy  #
# of this software and associated documentation files (the "Software"), to deal #
# in the Software without restriction, including without limitation the rights  #
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     #
# copies of the Software, and to permit persons to whom the Software is         #
# furnished to do so, subject to the following conditions:                      #
#                                                                               #
# The above copyright notice and this permission notice shall be included in    #
# all copies or substantial portions of the Software.                           #
#                                                                               #
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    #
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      #
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   #
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        #
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, #
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN     #
# THE SOFTWARE.                                                                 #
#################################################################################

# Figure out directory where this script is located
SOURCE="${BASH_SOURCE[0]}";
while [ -h "$SOURCE" ]; do # resolve $SOURCE until the file is no longer a symlink
	DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";
	SOURCE="$(readlink "$SOURCE")";
	[[ $SOURCE != /* ]] && SOURCE="$DIR/$SOURCE" # if $SOURCE was a relative symlink, we need to resolve it relative to the path where the symlink file was located
done
DIR="$( cd -P "$( dirname "$SOURCE" )" && pwd )";

# Go up two level to reach the root of the repository
ROOT="$(dirname $(dirname $DIR))";

#################################################################################
# Configuration constants
#################################################################################

# Name of the repository containing build dependencies
BUILD_REPO_NAME=PUT_GIT_REPO_HERE
# Where the Git repository is located
BUILD_GIT_REPO=PUT_GIT_HOST_HERE
# URL of the repository containing build dependencies
BUILD_REPO_URL=PUT_GIT_URL_HERE
# Specify the commit reference of smv6-arm-build required by this project. If the variable is commented, the smv6-arm-build won't be updated.
BUILD_REPO_COMMIT=PUT_GIT_SHA1_HERE
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

	# Setup 6LR dependency repository (6lr-arm-build)
	setupBuildRepo;

	echo ""
	echo "Execution successful!";
}

setupBuildRepo () {
	echo ""
	echo "[Setting up $BUILD_REPO_NAME]"

	if [ -n "$ARM_BUILD_HOME" ]; then
		echo "ARM_BUILD_HOME defined";

		# Correct path-style if on Windows
		if [ "$(expr substr $(uname -s) 1 5)" == "MINGW" ]; then
			ARM_BUILD_HOME="$(echo /$ARM_BUILD_HOME | sed -e 's/\\/\//g' -e 's/://')";
		fi
	else
		ARM_BUILD_HOME="$(dirname "$ROOT")"/"$BUILD_REPO_NAME";
	fi
	
	# Now that we've determined where the repo should be located, clone or validate.
	if [ -d "$ARM_BUILD_HOME" ]; then
		validateRepo;
	else
		cloneRepo;
	fi
}

validateRepo () {
	echo "Making sure '$ARM_BUILD_HOME' is valid and up to date";

	# Checking if it's a Git repository
	cd "$ARM_BUILD_HOME";
	git rev-parse --is-inside-work-tree &> /dev/null;
	if [ $? -eq 0 ]; then
		# Checking if the repository's remote has the proper URL
		local CUR_REPO_URL=$(git config --get remote.origin.url);
		if [ "$CUR_REPO_URL" == "$BUILD_REPO_URL" ]; then
			if [ -n "$GIT_REF" ]; then
				checkoutFromCommit "$GIT_REF"
			else
				warning "The variable GIT_REF is not set and the repository $BUILD_REPO_NAME won't be updated";
			fi
		else
			error "Repository wasn't cloned from '"$BUILD_REPO_URL"'";
		fi
	else
		error "Not a valid Git repository";
	fi
}

checkoutFromCommit () {
	local CUR_LOCAL=$(git rev-parse @);
	local WANTED=$(git rev-parse $1);
	if [ "$CUR_LOCAL" == "$WANTED" ]; then
		echo "Cleaning repository..."
		git clean -fdx;
		if [ $? -ne 0 ] ; then
			error "Failed to clean repository "$BUILD_REPO_NAME;
		fi
		git reset --hard;
		if [ $? -ne 0 ] ; then
			error "Failed to reset repository "$BUILD_REPO_NAME;
		fi
		echo "Repository up-to-date!";
	else
		# Fetching from remote
		gitCredentialsSetup;
		git fetch --all;
		if [ $? -eq 0 ]; then
			git checkout $1;
			if [ $? -eq 0 ]; then
				gitCredentialsTeardown;
				echo "Repository updated with success";
				validateRepo;
				return;
			else
				gitCredentialsTeardown;
				error "Checkout of $1 failed";
			fi
		else
			gitCredentialsTeardown;
			error "Fetching from remote failed";
		fi
	fi 
}

cloneRepo () {
	# Create directory before cloning into
	mkdir -p "$ARM_BUILD_HOME";
	if [ $? -ne 0 ] ; then
		error "Failed to create directory for "$BUILD_REPO_NAME;
	fi
	
	# Checkout repository
	gitCredentialsSetup;
	git clone -n "$BUILD_REPO_URL" "$ARM_BUILD_HOME";
	
	if [ $? -eq 0 ]; then
		gitCredentialsTeardown;
		# Validate respository
		validateRepo;
	else
		gitCredentialsTeardown;
		error "Failed to clone "$BUILD_REPO_URL;
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
		# Convert (if needed) the Windows-style path to Unix-style before turning this into a _HOME constant
		GIT_HOME=$(dirname "$(dirname "$(echo "/$GIT_EXEC" | sed 's/\\/\//g' | sed 's/://')")");
		export GIT_HOME=$GIT_HOME;
		PATH="$GIT_HOME/bin:$PATH";
		git --version;
	else
		error "Git Not Found";
	fi
}

#################################################################################
# Helper functions
#################################################################################

error () {
	echo "ERROR: $1";
	exit 1
}

warning () {
	echo "WARNING: $1";
}

#################################################################################
# Internal constants
#################################################################################

SCRIPT_NAME="$(basename $BASH_SOURCE)";
GET_HELP="Type './$SCRIPT_NAME --help' for usage information.";
PARAM_REF="gitReference";

#################################################################################
# Execution starting point
#################################################################################

# Welcome message
echo "";
echo "[Running $SCRIPT_NAME]";

# Asking for help?
if [[ ( $1 == "--help") ||  $1 == "-h" ]]; then
	echo "$SCRIPT_NAME runs through the process of updating the smv6-arm-build repository containing build tools.";
	echo "Usage: $SCRIPT_NAME [$PARAM_REF]";
	echo "";
	echo "      -${PARAM_REF}: (Optional) Git reference to be checked out. Overrides the hardcoded value within this script.";
	exit 0;
fi

# Make sure we have the expected parameters and backup them
if [ "$#" -eq 1 ]; then
	GIT_REF="$1";
elif [ "$#" -gt 1 ]; then
	error "Expected 0 or 1 parameter. $GET_HELP";
else
	if [ -n "$BUILD_REPO_COMMIT" ]; then
		GIT_REF=$BUILD_REPO_COMMIT;
	fi
fi

# Run this script
main;

# Exit message
echo "";
echo "[Exiting $SCRIPT_NAME]";
exit 0;

#!/bin/bash
#################################################################################
#            ___________      .__.__  .__  .__               __                 #
#            \__    ___/______|__|  | |  | |__|____    _____/  |_               #
#              |    |  \_  __ \  |  | |  | |  \__  \  /    \   __\              #
#              |    |   |  | \/  |  |_|  |_|  |/ __ \|   |  \  |                #
#              |____|   |__|  |__|____/____/__(____  /___|  /__|                #
#                                                  \/     \/                    #
#                                         Networks Inc.                         #
# File:    versionProperty.sh                                                   #
#                                                                               #
# Script to generate version number                                             #
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

#################################################################################
# This scripts generates a java properties file supported by the EnvInject      #
# plugin in Jenkins https://wiki.jenkins-ci.org/display/JENKINS/EnvInject+Plugin#
#                                                                               #
# Once generated, a file named "version.properties" will be created in the      #
# current directory. It will contain a single property:                         #
#          PROJECT_VERSION = {VERSION}                                          #
#                                                                               #
# NOTE: When supplied with the "console" parameter, this script will output the  #
# version to the console instead.                                               #
#################################################################################

# Call 'git describe' and change the output from the format '-a-gSHA' to '+a.SHA', matching the "Semantic Version 2.0.0" format.
# https://confluence.trilliant.local/x/3AKCAQ
VERSION="$(git describe --long | sed -ne 's/^\(.*\)-\([0-9]\{1,4\}\)-g\([a-f0-9]\{1,40\}\)$/\1+\2.\3/p')";

if [ "$1" = "console" ]; then
	echo $VERSION
else
	# Create the file and inject the property
	echo -n "PROJECT_VERSION = " > version.properties
	echo $VERSION >> version.properties
fi

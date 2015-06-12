#!/bin/bash

################################################################################################
## This scripts generates a java properties file supported by the EnvInject plugin in Jenkins ##
## https://wiki.jenkins-ci.org/display/JENKINS/EnvInject+Plugin                               ##
##                                                                                            ##
## Once generated, a file named "version.properties" will be created in the current directory.##
## It will contain a single property:                                                         ##
##          PROJECT_VERSION = {VERSION}                                                       ##
##                                                                                            ##
## This is used by Jenkins to generate the version during build time and obtain an environment##
## variable called "PROJECT_VERSION", used in other steps of the build.                       ##
################################################################################################

# Create the file and inject the property
echo -n "PROJECT_VERSION = " > version.properties

# Call 'git describe' and change the output from the format '-a-gSHA' to '+a.SHA', matching the "Semantic Version 2.0.0" format.
# https://confluence.trilliant.local/x/3AKCAQ
git describe --long | sed -ne 's/^\(.*\)-\([0-9]\{1,4\}\)-g\([a-f0-9]\{1,40\}\)$/\1+\2.\3/p' >> version.properties

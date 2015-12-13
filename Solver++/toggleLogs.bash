#!/bin/bash

# Public function
# Return directory name from given path
# Example:
#	getDir '/usr/foo.bar'
#	returns '/usr'
#
function getParentDir() {
	echo ${1%/*/*}
}

# Public function
# Expand special character '~'/'.', if occurred, to home/current directory.
# Only first '~'/'.' will be expanded.
# Add '/' as a last character if needed
#
function expandPath() {
	# Expand of ~ character
	local var="${1/#\~/$HOME}"

	# Expand of . character
	local var="${var/#\./$PWD}"

	# Add '/' as a last character if needed
	if ! [[ "$var" =~ '/'$ ]]; then 
		var=$var'/'

	fi

	echo "$var"
}

function move() {
	echo "Move '$1' > '$2'."
	mv "$1" "$2"
}

function makeDir() {
	echo "Create directory '$1'."
	mkdir -p "$1"
}

# Script control flag
# 0 - uncomments every log-like line in project and moves back all necessary files,
# 1 - comments every log-like line in project and moves all logging related files out.
toggleLogs=0

# Regexp of lines with logging macro (in 1 line).
INCLUDE_LOGGER='#include <(log4cxx\/.*)>'

INCLUDE_LOG_BUNDLE='#include ".*?\/log\/bundle\/.*"'
INCLUDE_LOG_UTILS='#include ".*?\/log\/utils\/.*"'

LOG_POINTER='(const |const static |static const )?log4cxx::LoggerPtr \w*?\(\s*log4cxx::Logger::getLogger\(\s*".*?"\)\);'
CONFIG_LOG='LogUtils::configureLog\(\s?".+?[.].+?"\);'
	
LOG_LINES='((TRACE|DEBUG|INFO|WARN|ERROR|FATAL)\(\s*.+?,(.|\s)*?\));'
COMMENT_START='\/\*'
COMMENT_END='\*\/'

MAKEFILE_LOG='^#?-include src\/src\/log*?'
MAKEFILE_SOURCES='^#?src\/src\/log*?'
MAKE_COMMENT='#'

MAKE_OBJECTS='LIBS :=( -l\w+)*'
LOG4CXX_LIB='log4cxx'

projectPath=$(pwd)/
includePath='src/include/'
srcPath='src/src/'

makefilePath="$projectPath"'Debug/'

# Path to folder where all logging-related files will be stored when they are moved from project
extractLogForder="$projectPath"'extracted_log/'

# All files and directories to be moved from project in order to delete logging funcionality
logIncludePath="$includePath"'log/'
logSrcPath="$srcPath"'log/'
logIncludeProjectPath="$projectPath""$logIncludePath"
logSrcProjectPath="$projectPath""$logSrcPath"

logIncludeExtractPath="$extractLogForder""$logIncludePath"
logSrcExtractPath="$extractLogForder""$logSrcPath"


function getToggleState() {
	
	# If no extract folder exists create one and assume to comment all logs
	if ! [ -d "$1" ]; then
		toggleLogs=1
		makeDir "$logIncludeExtractPath"
		makeDir "$logSrcExtractPath"
	fi

}

function uncommentLogs() {
	echo "Add logs back to project..."
	echo "Move logging-related sources into project..."
	move "$logIncludeExtractPath" "$logIncludeProjectPath"

	local srcFiles=$(find "$srcPath" -name '*.cpp' -or -name '*.c')

	echo -e "Uncomment logging-related code in source files...\n"
	for f in $srcFiles;
	do
		echo -e "\tParsing file: $f"
		perl -0777 -p -i -e "s/$COMMENT_START\s*($INCLUDE_LOGGER|$LOG_LINES|$INCLUDE_LOG_BUNDLE|$INCLUDE_LOG_UTILS|$LOG_POINTER|$CONFIG_LOG)\s*$COMMENT_END/\$1/g" "$f"
	done;
	echo -e "\nAll logging-related code in source files has been uncommended."

	echo "Move logging-related headers into project..."
	move "$logSrcExtractPath" "$logSrcProjectPath"

	local includeFiles=$(find "$includePath" -name '*.hpp' -or -name '*.h')

	echo -e "Uncomment logging-related code in header files...\n"
	for f in $includeFiles;
	do
		echo -e "\tParsing file: $f"
		perl -0777 -p -i -e "s/$COMMENT_START\s*($INCLUDE_LOGGER|$LOG_LINES|$INCLUDE_LOG_BUNDLE|$INCLUDE_LOG_UTILS|$LOG_POINTER|$CONFIG_LOG)\s*$COMMENT_END/\$1/g" "$f"
	done;
	echo -e "\nAll logging-related code in headers files has been uncommended."

	echo "Changing makefile and related files..."

	cd "$makefilePath"
	make clean

	echo "Changing makefile..."
	sed -E -i "/$MAKEFILE_LOG/s/$MAKE_COMMENT//" "$makefilePath"'makefile'

	echo "Changing sources.mk..."
	sed -E -i "/$MAKEFILE_SOURCES/s/$MAKE_COMMENT//" "$makefilePath"'sources.mk'

	echo "Changing objects.mk..."
	sed -E -i "s/$MAKE_OBJECTS/& -l$LOG4CXX_LIB/" "$makefilePath"'objects.mk'

	cd "$projectPath"
	rm -rf "$extractLogForder"
	echo "Done."

}

function commentLogs() {
	echo "Removing logs from project..."
	echo "Move logging-related sources out from project..."
	move "$logSrcProjectPath" $(getParentDir "$logSrcExtractPath")
	local srcFiles=$(find "$srcPath" -name '*.cpp' -or -name '*.c')

	echo -e "Comment logging-related code in source files...\n"
	for f in $srcFiles;
	do
		echo -e "\tParsing file: $f"
		perl -0777 -p -i -e "s/($INCLUDE_LOGGER|$LOG_LINES|$INCLUDE_LOG_BUNDLE|$INCLUDE_LOG_UTILS|$LOG_POINTER|$CONFIG_LOG)/$COMMENT_START\t\$1\t$COMMENT_END/g" "$f"
	done;
	echo -e "\nAll logging-related code in source files has been commended."

	echo "Move logging-related headers out from project..."
	move "$logIncludeProjectPath" $(getParentDir "$logIncludeExtractPath")

	local includeFiles=$(find "$includePath" -name '*.hpp' -or -name '*.h')

	echo -e "Comment logging-related code in header files...\n"
	for f in $includeFiles;
	do
		echo -e "\tParsing file: $f"
		perl -0777 -p -i -e "s/($INCLUDE_LOGGER|$LOG_LINES|$INCLUDE_LOG_BUNDLE|$INCLUDE_LOG_UTILS|$LOG_POINTER|$CONFIG_LOG)/$COMMENT_START\t\$1\t$COMMENT_END/g" "$f"
	done;
	echo -e "\nAll logging-related code in headers files has been commended."

	echo "Changing makefile and related files..."

	cd "$makefilePath"
	make clean

	echo "Changing makefile..."
	sed -E -i "s/$MAKEFILE_LOG/$MAKE_COMMENT\0/" "$makefilePath"'makefile'

	echo "Changing sources.mk..."
	sed -E -i "s/$MAKEFILE_SOURCES/$MAKE_COMMENT\0/" "$makefilePath"'sources.mk'

	echo "Changing objects.mk..."
	sed -E -i "/$MAKE_OBJECTS/s/ -l$LOG4CXX_LIB//" "$makefilePath"'objects.mk'

	cd "$projectPath"
	echo "Done."
	
}

getToggleState $extractLogForder

echo '--------------------------------------------------------------'
if [ $toggleLogs -eq 0 ]; then
	uncommentLogs
else
	commentLogs
fi
echo '--------------------------------------------------------------'

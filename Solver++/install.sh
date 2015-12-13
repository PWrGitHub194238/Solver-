#!/bin/bash

#https://projects.coin-or.org/Osi/
#Błąd weryfikacji certyfikatu serwera dla 'https://projects.coin-or.org:443'
 #- Certyfikat nie jest wydany przez zaufanego dostawcę. Skorzystaj
  # z odcisku palca by zweryfikować go samodzielnie!
#Informacje o certyfikacie:
# - Nazwa maszyny: *.coin-or.org
# - Okres ważności: od Apr 11 20:44:41 2014 GMT do Apr 11 20:44:41 2017 GMT
# - Wydawca: http://certs.godaddy.com/repository/, GoDaddy.com, Inc., Scottsdale, Arizona, US
# - Odcisk palca: E7:5B:E2:82:44:7F:EF:52:8B:E2:07:B0:D9:B4:27:0C:0E:64:11:56
#Odrzucić (r), zaakceptować chwilowo (t) czy zaakceptować na stałe (p)? p

coinOsiSVNBaseURL='https://projects.coin-or.org/svn/Osi/stable'

coinOsiSharedLibConfigDir='/etc/ld.so.conf.d/'
coinOsiSharedLibConfigFile='coin-Osi.conf'

log4cxxWgetURL='http://ftp.ps.pl/pub/apache/logging/log4cxx/0.10.0/apache-log4cxx-0.10.0.tar.gz'

antWgetURL='http://ftp.piotrkosoft.net/pub/mirrors/ftp.apache.org//ant/source/apache-ant-1.9.6-src.tar.gz'

antContribWgetURL='http://sourceforge.net/projects/ant-contrib/files/latest/download?source=files'

# Public function
# Parse command line arguments
#
#	-d --defaults			set script not to prompt about any value which has a default one specified
#	-f --force-yes			force every prompt to be skiped with 'y' value - use with caution
#	--osi-path <arg>		path where coin-Osi project will be downloaded
#	--osi-version <arg>		which coin-Osi project version to be downloaded
#	--osi-include-path <arg>	path where coin-Osi include files will be moved
#	--osi-lib-path <arg>		path where coin-Osi libraries will be moved
#	--log4cxx-path <arg>		path where log4cxx project will be downloaded
#
function pasreBash() {
	while [[ $# > 0 ]]
	do
		case $1 in
			-d|--defaults)
				DEFAULT='y'
			;;
			-f|--force-yes)
				FORCE_YES='y'
			;;
			--osi-version)
				bash_osi_ver="$2"
				shift # past argument
				;;
			--osi-path)
				bash_osi_path=$(expandPath "$2")
				shift # past argument
				;;
			--osi-include-path)
				bash_osi_include_path=$(expandPath "$2")
				shift # past argument
				;;
			--osi-lib-path)
				bash_osi_lib_path=$(expandPath "$2")
				shift # past argument
				;;
			--log4cxx-path)
				bash_log4cxx_path=$(expandPath "$2")
				shift # past argument
				;;
			*)
				printHelp
				exit;
			;;
		esac
		shift # past argument or value
	done
}

# Public function
#
# Print help for script
#
function printHelp() {
	echo "Usage: [sudo] bash preReq.sh [-d|--defaults] [-f|--force-yes] [--osi-path <arg>] [--osi-version <arg>] [--osi-include-path <arg>] [--osi-lib-path <arg>] [--log4cxx-path <arg>]"
	echo "	-d --defaults			set script not to prompt about any value which has a default one specified"
	echo "	-f --force-yes			force every prompt to be skiped with 'y' value - use with caution"
	echo "	--osi-path <arg>		path where coin-Osi project will be downloaded"
	echo "	--osi-version <arg>		which coin-Osi project version to be downloaded"
	echo "	--osi-include-path <arg>	path where coin-Osi include files will be moved"
	echo "	--osi-lib-path <arg>		path where coin-Osi libraries will be moved"
	echo "	--log4cxx-path <arg>		path where log4cxx project will be downloaded"
#
}


# Public function
# Return file extension from given path
# Example:
#	getFileExt '/usr/foo.bar'
#	returns 'bar'
#
function getFileExt() {
	echo ${1##*.}
}

# Public function
# Return file name with its extension from given path
# Example:
#	getFileName '/usr/foo.bar'
#	returns 'foo.bar'
#
function getFileName() {
	echo ${1##*/}
}

# Public function
# Return directory name from given path
# Example:
#	getDir '/usr/foo.bar'
#	returns '/usr'
#
function getDir() {
	echo ${1%/*}
}

# Public function
# Return base directory name from given path
# Example:
#	getDir '/usr/foo/foo.bar'
#	returns '/usr'
#
function getBaseDir() {
	echo ${1%%/*}
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

# Public function
# Read user input and return inputed value or default one (if user left field blank)
#
# Parameters:
#	$1 - text message to show to user before asking for value
#	$2 - default value
# Example:
#	var=$(readData 'Text#1' 'Text#2')
#	returns 'Text#2' or user no-empty input
#
function readData() {
	read -rep "$1 (\"$2\" if left blank): "

	if [ -z "$REPLY" ]; then
		echo "$2"
	else
		echo "$REPLY"
	fi
}

# Private function
# Function supports bash command line arguments.
#
# Parameters:
#	$1 - text message to show to user before asking for value
#	$2 - default value
#	$3 - bash command line argument
# Example:
#	result=$(bash_readData 'Text#1' 'Text#2' "$var")
#	returns
#		- Default value ('Text#2') if $DEFAULT is set to 'y'
#		- Value given by the command line if specified
#		- readData() returned value
#
function bash_readData() {
	if [ "$DEFAULT" == 'y' ]; then
		echo "$2"
	elif ! [ -z "$3" ]; then
		echo "$3"
	else
		echo $(readData "$1" "$2")
	fi
}

# Public function
# Read user input and return inputed value or default one (if user left field blank).
# In both cases special character '~', if occurred, will be expanded to home directory.
# Only first '~' will be expanded.
#
# Parameters:
#	$1 - text message to show to user before asking for value
#	$2 - default value
# Example:
#	var=$(readDir 'Text#1' $(pwd)'/Text#2/')
#	returns absolute path to folder 'Text#2' in current directory or user no-empty input
#
function readDir() {
	local var=$(readData "$1" "$2")

	echo $(expandPath "$var")
}

# Private function
# Function supports bash command line arguments.
#
# Parameters:
#	$1 - text message to show to user before asking for value
#	$2 - default value
#	$3 - bash command line argument
# Example:
#	result=$(bash_readDir 'Text#1' $(pwd)'/Text#2/' "$var")
#	returns bash_readData() expanded value
#
function bash_readDir() {
	local var=$(bash_readData "$1" "$2" "$3")

	echo $(expandPath "$var")
}


# Public function
# Read user input and return inputed value or default one (if user left field blank).
# In both cases special character '~', if occurred, will be expanded to home directory.
# Only first '~' will be expanded.
# If given directory exists then function will ask to delete it first.
#
# Parameters:
#	$1 - text message to show to user before asking for value
#	$2 - default value
# Example:
#	var=$(remakeDir 'Text#1' $(pwd)'/Text#2/')
#	returns absolute path to folder 'Text#2' in current directory or user no-empty input
#
function remakeDir() {
	local var=$(readDir "$1" "$2")
	
	removeIfExists "\"$var\" already exists. Remove this directory?" "$var"
	makeDirIfNotExists "$var"

	echo "$var"
}

# Private function
# Function supports bash command line arguments.
#
# Parameters:
#	$1 - text message to show to user before asking for value
#	$2 - default value
#	$3 - bash command line argument
# Example:
#	var=$(bash_remakeDir 'Text#1' $(pwd)'/Text#2/' "$var")
#	returns bash_readDir() value
#
function bash_remakeDir() {
	local var=$(bash_readDir "$1" "$2" "$3")

	bash_removeIfExists "\"$var\" already exists. Remove this directory?" "$var"
	bash_makeDirIfNotExists "$var"

	echo "$var"
}

# Public function
# Remove file/directory if it exists.
#
# Parameters:
#	$1 - text message to show to user before asking for 'y' or 'n' value
#	$2 - file name/path to directory
#
function removeIfExists() {
	if [ -d "$2" ]; then
		if [ $(confirm "$1") == "y" ]; then
			sudo rm -fr "$2"
		fi;
	fi
}

# Private function
# Function supports bash command line arguments.
#
# Parameters:
#	$1 - text message to show to user before asking for 'y' or 'n' value
#	$2 - file name/path to directory
#
function bash_removeIfExists() {
	if [ -d "$2" ]; then
		if [ $(bash_confirm "$1") == "y" ]; then
			sudo rm -fr "$2"
		fi;
	fi
}

# Public function
# Create given directory if it  exists.
#
# Parameters:
#	$1 - path to directory 
#
function makeDirIfNotExists() {
	if ! [ -d "$1" ]; then
		sudo mkdir -p "$1"
	fi
}

# Private function
# Function supports bash command line arguments.
#
# Parameters:
#	$1 - path to directory 
#
function bash_makeDirIfNotExists() {
	makeDirIfNotExists "$1"
}

# Public function
# Show prompt and force user to confirm or cancel.
#
# Parameters:
#	$1 - text message to show to user before asking for 'y' or 'n' value
#
function confirm() {
	read -e -p "$1 (y/n): "
	echo "$REPLY"
}

# Private function
# Function supports bash command line arguments.
#
# Parameters:
#	$1 - text message to show to user before asking for 'y' or 'n' value
#
function bash_confirm() {
	if [ "$FORCE_YES" == 'y' ]; then
		echo 'y'
	else
		echo $(confirm "$1")
	fi
}

# Public function
# Install additional package and dependencies
#
# Parameters:
#	$1 - package name
#
function installPackage() {
	if [ $(dpkg-query -W -f='${Status}' "$1" 2>/dev/null | grep -c "ok installed") -eq 0 ];
	then
		if [ $(confirm "Confirm installation of package \"$1\"?") == "y" ]; then
			sudo apt-get -yf install "$1"
			sudo apt-get -yf install
		fi;
	else
		echo "Package \"$1\" already has been installed."
	fi;
}

# Public function
# Install additional package and dependencies
#
# Parameters:
#	$1 - package name
#
function bash_installPackage() {
	if [ $(dpkg-query -W -f='${Status}' "$1" 2>/dev/null | grep -c "ok installed") -eq 0 ];
	then
		if [ $(bash_confirm "Confirm installation of package \"$1"\"?) == "y" ]; then
			sudo apt-get -yf install "$1"
			sudo apt-get -yf install
		fi;
	fi
}

####################################################################################################################

# Private function
# Configure coin-Osi project to support GLPK LP/MIP Solver 
#
# Parameters:
#	$1 - path to coin-Osi checkout directory
#
function configureOsiGlpk() {
	echo "Configuring ThirdParty GLPK"
	cd "$1/ThirdParty/Glpk/"

	glpk_ver=$(grep -Po '^glpk_ver=\K.*$' get.Glpk)
	echo "Use glpk version: $glpk_ver"

	bash get.Glpk
	cd ../../
}

# Private function
# Build coin-Osi project with given configuration
#
# Parameters:
#	$1 - path to coin-Osi checkout directory
#
function makeOsi() {
	
	configureOsiGlpk "$1"

	./configure -C
	make
	make test
	make install

}

# Private function
# Checkout coin-Osi project into given path and call makeOsi()
#
# Parameters:
#	$1 - version of coin-Osi stable build
#	$2 - path where coin-Osi project will be checked out.
# Example:
#	getOsi '0.107' '/home/xyz/coin-Osi'
#	will download to '/home/xyz/coin-Osi' and compile coin-Osi project (version '0.107')
#
function getOsi() {
	
	echo "\"subversion\" is needed to checkout coin-Osi project's sources."
	bash_installPackage "subversion"

	echo 'Checking out the coin-Osi project...'
	sudo svn co "$coinOsiSVNBaseURL/$1" "$2"

	echo "Coin-Osi project has been saved into \"$2\"."
	cd "$2"
	makeOsi "$2"
	cd ../

}

# Public function
# Checkout given coin-Osi project stable version into given path and move both include files and libraries to given directories.
#
function executeOsiCreation() {
	local osi_ver=$(bash_readData 'Choose coin-Osi project version to chechout' '0.107' "$bash_osi_ver")
	echo "Coin-Osi version to checkout is set to: \"$osi_ver\""

	echo 'WARNING. Osi project only supports old glpk API which was removed in versions higher than 4.52'
	local osi_path=$(bash_remakeDir 'Enter where to checkout Osi project' $(pwd)'/coin-Osi/' "$bash_osi_path")
	echo "Osi checkout path is set to: \"$osi_path\""

	local osi_include_path=$(bash_remakeDir 'Enter where to move Osi header files' '/usr/local/include/coin-Osi/' "$bash_osi_include_path")
	echo "Osi header files path is set to: \"$osi_include_path\""

	local osi_lib_path=$(bash_remakeDir 'Enter where to move Osi libraries files' '/usr/local/lib/coin-Osi/'"$bash_osi_lib_path")
	echo "Osi libraries files path is set to: \"$osi_lib_path\""

	getOsi "$osi_ver" "$osi_path"

	echo "Moving headers to \"$osi_include_path\"..."
	sudo mv "$osi_path"'include/coin/'* "$osi_include_path"

	echo "Moving libraries to \"$osi_lib_path\"..."
	sudo mv "$osi_path"'lib/'* "$osi_lib_path"

	echo 'Configuring shared libraries properties...'
	echo "Create \"$coinOsiSharedLibConfigFile\" file in directory \"$coinOsiSharedLibConfigDir\"..."

	mkdir -p "$coinOsiSharedLibConfigDir"

	sudo sh -c "echo \"$osi_lib_path\" > \"$coinOsiSharedLibConfigDir/$coinOsiSharedLibConfigFile\""

	echo 'Reload system configuration...'
	sudo ldconfig

	echo 'Done.'
}

function removeOsiCreation() {
	local osi_path=$(bash_readDir 'Enter Osi project path' $(pwd)'/coin-Osi/' "$bash_osi_path")
	echo "\"$osi_path\" directory will be DELETED."
	
	local osi_include_path=$(bash_readDir 'Enter Osi headers path' '/usr/local/include/coin-Osi/' "$bash_osi_include_path")
	echo "\"$osi_include_path\" directory will be DELETED."

	local osi_lib_path=$(bash_readDir 'Enter Osi libraries path' '/usr/local/lib/coin-Osi/' "$bash_osi_lib_path")
	echo "\"$osi_lib_path\" directory will be DELETED."

	echo "\"$coinOsiSharedLibConfigDir$coinOsiSharedLibConfigFile\" file will be DELETED"

	if [ $(bash_confirm 'Are you sure to delete this directories?') == "y" ]; then
		sudo rm -fr "$osi_path"
		sudo rm -fr "$osi_include_path"
		sudo rm -fr "$osi_lib_path"
		sudo rm -f "$coinOsiSharedLibConfigDir$coinOsiSharedLibConfigFile"
		echo 'Reload system configuration...'
		sudo ldconfig
		echo 'Done.'
	fi;
}

####################################################################################################################

function getLog4cxx() {
	echo "Log4cxx download path is set to: \"$1\""
	echo "Attepmt to download Log4cxx library for Solver++ project..."
	cd "$1"
	wget $log4cxxWgetURL
	if [ $? -eq 0 ] 
	then
		makeLog4cxx $(getFileName "$log4cxxWgetURL")
	else
		echo "Fail to download Log4cxx library from given source!"
	fi
}

function makeLog4cxx() {
	tar -zxf "$1"
	local folderName=$(getBaseDir $(tar -tf "$1" | head -1))
	cd $folderName
	
	echo "\"libapr1-dev\" is needed to build Log4cxx library from it's sources."
	bash_installPackage "libapr1-dev"
	echo "\"libaprutil1-dev\" is needed to build Log4cxx library from it's sources."
	bash_installPackage "libaprutil1-dev"
	echo "\"zip\" is needed to build Log4cxx library from it's sources."
	bash_installPackage "zip"

	./configure
	make
	make check
	sudo make install
	cd ../
}

function executeLog4cxxCreation() {
	bash_installPackage "liblog4cxx10v5"
	bash_installPackage "liblog4cxx10-dev"
	echo "Done"
}

function executeValgrindCreation() {
	bash_installPackage "valgrind"
	echo "Done"
}


####################################################################################################################


set -e

pasreBash $@

removeOsiCreation

executeOsiCreation

executeLog4cxxCreation

executeValgrindCreation

bash_installPackage "octave"

bash_installPackage "inkscape"

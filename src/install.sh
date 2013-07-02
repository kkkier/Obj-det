#!/bin/tcsh 

# Create a directory tree for the image libraries
mkdir -p ~/code
mkdir -p ~/code/bin
mkdir -p ~/code/bin/i686
mkdir -p ~/code/include
mkdir -p ~/code/lib
mkdir -p ~/code/lib/i686
mkdir -p ~/code/obj
mkdir -p ~/code/obj/i686
mkdir -p ~/code/src
mkdir -p ~/code/src/libraries

cp *.tgz ~/code/src/
cp *.sh ~/code/src/
cp *.txt ~/code/src/
pushd ~/code/src
foreach h (`ls -1 *.tgz`)
  tar -xvzf $h
end

#
# Set the library path
#

if( $?LD_LIBRARY_PATH )
  setenv LD_LIBRARY_PATH ${LD_LIBRARY_PATH}:~/code/lib/i686
else
  setenv LD_LIBRARY_PATH ~/code/lib/i686
endif

if( $?QTDIR )
else
  setenv QTDIR /usr/share/qt3
endif

#
# Compile the directories individually
#

./compile.sh


# clean up

mkdir -p tarfiles
mv *.tgz tarfiles
cp *.sh tarfiles
cp *.txt tarfiles

popd

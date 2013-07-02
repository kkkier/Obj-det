#!/bin/sh 

MACHTYPE=i686
CODE=$HOME/code

D_BIN=$CODE/bin/$MACHTYPE
D_INC=$CODE/include
D_LIB=$CODE/lib/$MACHTYPE
D_OBJ=$CODE/obj/$MACHTYPE

# install script for first stable relase of the image package

# target directories for installation
LIB_INSTALL=$D_LIB
INC_INSTALL=$D_INC
BIN_INSTALL=$D_BIN

# installation 
cp -f ./libppmio.so.1.0.1 $LIB_INSTALL
ln -fs $LIB_INSTALL/libppmio.so.1.0.1 $LIB_INSTALL/libppmio.so
ln -fs $LIB_INSTALL/libppmio.so.1.0.1 $LIB_INSTALL/libppmio.so.1

cp -f ./librgbdisp.so.1.0.1 $LIB_INSTALL
ln -fs $LIB_INSTALL/librgbdisp.so.1.0.1 $LIB_INSTALL/librgbdisp.so
ln -fs $LIB_INSTALL/librgbdisp.so.1.0.1 $LIB_INSTALL/librgbdisp.so.1

cp -f ./imgtest $BIN_INSTALL/

cp -f ./image.h $INC_INSTALL/
cp -f ./rgbimage.h $INC_INSTALL/
cp -f ./rgbdisplay.h $INC_INSTALL/
cp -f ./ppmio.h $INC_INSTALL/
cp -f ./rgbwidget.h $INC_INSTALL/

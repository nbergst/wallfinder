#  Build the doxygen documentation for the project and load the docset into Xcode.

#  Use the following to adjust the value of the $DOXYGEN_PATH User-Defined Setting:
#    Binary install location: /Applications/Doxygen.app/Contents/Resources/doxygen
#    Source build install location: /usr/local/bin/doxygen

#  If the config file doesn't exist, run 'doxygen -g $SOURCE_ROOT/doxygen.config' to
#   a get default file.
export DOXYGEN_PATH="/usr/local/bin/doxygen"

echo "DOXYGEN_PATH = ${DOXYGEN_PATH}"
echo "TEMP_DIR = ${TEMP_DIR}"


# if ! [ -f $SOURCE_ROOT/doxygen.config ]
# then
# echo doxygen config file does not exist
# $DOXYGEN_PATH -g $SOURCE_ROOT/doxygen.config
# fi

#  Append the proper input/output directories and docset info to the config file.
#  This works even though values are assigned higher up in the file. Easier than sed.

cp $SOURCE_ROOT/doc/doxygen.config $TEMP_DIR/doxygen.config

echo "OUTPUT_DIRECTORY = $SOURCE_ROOT/doc" >> $TEMP_DIR/doxygen.config
echo "GENERATE_DOCSET        = YES" >> $TEMP_DIR/doxygen.config
echo "DOCSET_BUNDLE_ID       = M&N" >> $TEMP_DIR/doxygen.config

#  Run doxygen on the updated config file.
#  Note: doxygen creates a Makefile that does most of the heavy lifting.

$DOXYGEN_PATH $TEMP_DIR/doxygen.config

#  make will invoke docsetutil. Take a look at the Makefile to see how this is done.

make -C $SOURCE_ROOT/doc/DoxygenDocs.docset/html install

#  Construct a temporary applescript file to tell Xcode to load a docset.

rm -f $TEMP_DIR/loadDocSet.scpt

echo "tell application \"Xcode\"" >> $TEMP_DIR/loadDocSet.scpt
echo "load documentation set with path \"/Users/$USER/Library/Developer/Shared/Documentation/DocSets/\""
>> $TEMP_DIR/loadDocSet.scpt
echo "end tell" >> $TEMP_DIR/loadDocSet.scpt

#  Run the load-docset applescript command.

osascript $TEMP_DIR/loadDocSet.scpt

exit 0

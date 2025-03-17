PASSWORD=123
LENGTH=3
ARCHIVE_PATH="archive.zip"
SOURCE_PATH="source/"
OUTPUT_PATH="output/"

# Compile the cracker
build:
	clang -o cracker crack.c

# Create a zip archive at ARCHIVE_PATH with the content of SOURCE_PATH and secures it with PASSWORD
archive:
	7z a ${ARCHIVE_PATH} ${SOURCE_PATH}* -p${PASSWORD}

# Run the cracker with password of size 1 to LENGTH on the ARCHIVE_PATH and outputs its content in OUTPUT_PATH
run: build archive
	./cracker ${ARCHIVE_PATH} -o ${OUTPUT_PATH} -l ${LENGTH}

# Run lldb to find memory leaks
lldb: build
	lldb -- ./cracker ${ARCHIVE_PATH} -o ${OUTPUT_PATH} -l ${LENGTH}

# Delete archive, output and cracker executable
clean:
	rm -f ${ARCHIVE_PATH}
	rm -rf ${OUTPUT_PATH}
	rm -f cracker
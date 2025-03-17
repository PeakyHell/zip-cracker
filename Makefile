PASSWORD=123
LENGTH=3
ARCHIVE_PATH="archive.zip"
SOURCE_PATH="source/"
OUTPUT_PATH="output/"

build:
	clang -o cracker crack.c

run: build archive
	./cracker ${ARCHIVE_PATH} -o ${OUTPUT_PATH} -l ${LENGTH}

archive:
	7z a ${ARCHIVE_PATH} ${SOURCe}* -p${PASSWORD}

lldb: build
	lldb -- ./cracker ${ARCHIVE_PATH} -o ${OUTPUT_PATH} -l ${LENGTH}

clean:
	rm -f ${ARCHIVE_PATH}
	rm -rf ${OUTPUT_PATH}
	rm -f cracker
PASSWORD=123
LENGTH=3
ARCHIVE_PATH="archive.zip"
OUTPUT_PATH="output/"

build:
	clang -o cracker crack.c

run: build
	./cracker ${ARCHIVE_PATH} -o ${OUTPUT_PATH} -l ${LENGTH}

archive:
	7z a archive.zip arch/* -p${PASSWORD}

lldb: build
	lldb -- ./cracker ${ARCHIVE_PATH} -o ${OUTPUT_PATH} -l ${LENGTH}

clean:
	rm -f archive.zip
	rm -rf output/
	rm -f cracker
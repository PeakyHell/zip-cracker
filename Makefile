PASSWORD=123
LENGTH=3
ARCHIVE_PATH="archive.zip"
OUTPUT_PATH="output/"

main:
	7z a archive.zip arch/* -p${PASSWORD}
	clang -o cracker crack.c

run:
	./cracker ${ARCHIVE_PATH} -o ${OUTPUT_PATH} -l ${LENGTH}

lldb:
	lldb -- ./cracker -- ${ARCHIVE_PATH} -o ${OUTPUT_PATH} -l ${LENGTH}

clean:
	rm -f archive.zip
	rm -r output/
	rm -f cracker
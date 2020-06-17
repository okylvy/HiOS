GCC=gcc
TXT2C=txt2c

gcc:
	$(GCC) -o $(TXT2C) $(TXT2C).c
	./$(TXT2C)
	mv fonts.c ../src/

clean:
	rm -rf fonts.c
	rm -rf $(TXT2C)

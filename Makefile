SRC=src
TOOLS=tools

.PHONY: clean

img:
	make gcc -C $(TOOLS)
	make img -C $(SRC)

run: img
	make run -C $(SRC)

clean:
	make clean -C $(SRC)
	make clean -C $(TOOLS)

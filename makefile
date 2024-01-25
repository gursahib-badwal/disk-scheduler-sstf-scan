
all: disk_scheduler.c 
	gcc -o DSSimul disk_scheduler.c

clean:
	rm -f DSSimul
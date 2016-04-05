SIMDIR=/home/phys/villaa/umn_work/
SIMEVENTDIR=$(SIMDIR)getSimEvent/

all: supersima2root

supersima2root: supersima2root.cpp 
	g++ -g -o supersima2root supersima2root.cpp -I $(SIMEVENTDIR) -L $(SIMEVENTDIR) -lgetSimEvent `root-config --cflags --glibs` 

install: supersima2root
	@mkdir -p ~/bin
	@cp supersima2root ~/bin/

clean:
	rm -f *.o
	rm -f supersima2root 
	rm -f ~/bin/supersima2root

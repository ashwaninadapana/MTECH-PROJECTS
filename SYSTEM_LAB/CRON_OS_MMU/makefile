all: PageTableSimulation exec cron

CXX = g++

PageTableSimulation : Que2/OS.cpp Que2/MMU.cpp
	$(CXX) Que2/OS.cpp -o Que2/os.out
	
exec : Que2/MMU.cpp
	$(CXX) Que2/MMU.cpp -o Que2/mmu.out

cron : Que1/cron.cpp
	$(CXX) Que1/cron.cpp -lpthread -o Que1/cron.out

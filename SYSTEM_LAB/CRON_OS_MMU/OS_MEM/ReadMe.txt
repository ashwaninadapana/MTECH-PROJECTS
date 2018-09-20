HOW TO RUN:
./os.out <no_pages> <no_frames>
./mmu.out <no_pages> <tracefile> <page_bits> <offset_bits> <os_pid>

OS:
	createSharedMemory(): creates shared page table
	initialiseFreeFrames(): initialises fifo_list with all frames as free
	registerSignals(): registers signals like SIGUSR1,SIGUSR2,SIGINT as OS process has to respond to those signals
		SIGUSR1(handler: sigusr1_handler) : handles page fault
		SIGUSR2: increases lfu counter for page hit
		SIGINT:  terminates and destroys the page table shared memory if Cntrl+C is entered.
	createLFUList(): maintains the counter for each frame that tells how many times the page in the frame is referenced.
	getVictimFrame(): gets the victim frame based on replacement policies(either FIFO or LFU)
		FIFO: call fifo_victim()
		LFU: call lfu_victim()
	getFreeFrame(): returns the frame no if there is any free frame available

MMU:
	attachSharedMemory() : attaches to the shared page table memory created by the OS
	processMemTraceFile(): reads the memory trace file and process each entry by checking if there is any page fault or page hit
		Changes the dirty bit to 1 if it is write operation
		if pagefault, mmu signals OS to handle it 
	detachSharedMemory(): destroy the shared memory and signal the OS process for the last time
	printStatistics():  Prints the page table information and page hit and page fault count
	convertHexToBinaryAddress(): converts the virtual address in memory trace file into binary format
	getPageNumber(): converts hexadecimal address and  get the page number in the decimal format



STATISTICS:
	file: memtrace.txt
	FIFO: 	
		pagehits:13
		pagefaults:27
		disk accesses:39 
	LFU:
		pagehits:10
		pagefaults:30
		disk accesses:44 
		

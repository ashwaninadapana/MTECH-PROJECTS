all: CS558Assign1 exec

CXX = g++

CS558Assign1 : CS558Assign1.cpp exec child_executor.cpp
	$(CXX) CS558Assign1.cpp -lreadline -o CS558Assign1
	
exec : child_executor.cpp
	$(CXX) child_executor.cpp -o exec

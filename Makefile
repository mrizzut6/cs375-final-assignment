all: submission

submission: submission.cpp
	g++ -g submission.cpp -o submission

clean:
	rm -f submission
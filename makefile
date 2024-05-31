all: FSreport

FSreport: FSreport.c
	gcc -o FSreport -lm FSreport.c

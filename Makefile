OFILES = main.o ipc.o proc.o mecanicien.o chef_atelier.o

main : main.o proc.o ipc.o mecanicien chef_atelier
		gcc -o main.out main.o ipc.o proc.o

main.o : main.c
		gcc -c main.c

ipc.o : ipc.c
		gcc -c ipc.c

proc.o : proc.c
		gcc -c proc.c

mecanicien : mecanicien.o ipc.o
		gcc -o mecanicien ipc.o mecanicien.o

mecanicien.o : mecanicien.c
		gcc -c mecanicien.c

chef_atelier : chef_atelier.o ipc.o
		gcc -o chef_atelier ipc.o chef_atelier.o

chef_atelier.o : chef_atelier.c
		gcc -c chef_atelier.c

clean :
	-rm -f ${OFILES}

.PHONY : clean
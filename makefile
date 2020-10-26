all: compile echo

compile:
	@gcc -o horse_race_chess main.c

echo:
	@echo 'Compiled successfully, type "./horse_race_chess" to start playing.'

all: choose-language

choose-language:
	@echo ""
	@echo "Please choose a language:"
	@echo ""
	@echo "deutsch                   make german"
	@echo "english                   make english"
	@echo ""

clean:
	@echo ""
	@echo "Please choose your operating system:"
	@echo ""
	@echo "Linux                     make linux-clean"
	@echo ""

german:
	@echo ""
	@echo "Bitte waehle Dein Betriebssystem aus:"
	@echo ""
	@echo "Linux                     make linux-german"
	@echo ""

english:
	@echo ""
	@echo "Please choose your operating system:"
	@echo ""
	@echo "Linux                     make linux-english"
	@echo ""

linux-english:
	-rm -f os.h
	ln -s linux.h os.h
	make OS=LINUX RM=rm RMOPT=-f LN=ln LNOPT=-sf copy-english exec-setup-unix

linux-german:
	-rm -f os.h
	ln -s linux.h os.h
	make OS=LINUX RM=rm RMOPT=-f LN=ln LNOPT=-sf copy-german exec-setup-unix

linux-clean:
	-rm -f os.h
	-rm -f lang.h
	-rm -f setup

copy-german:
	-$(RM) $(RMOPT) lang.h
	-$(RM) $(RMOPT) setup
	$(LN) $(LNOPT) german.h lang.h

copy-english:
	-$(RM) $(RMOPT) lang.h
	-$(RM) $(RMOPT) setup
	$(LN) $(LNOPT) english.h lang.h

setup: setup.c
	$(CC) -o setup setup.c

exec-setup-unix: setup
	./setup

exec-setup-dos: setup
	setup


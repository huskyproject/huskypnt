
all: choose-language

choose-language:
	@echo ""
	@echo "Please choose a language:"
	@echo ""
	@echo "language                  command"
	@echo "--------------------------------------------------------------"
	@echo "english                   make c-english"
	@echo "deutsch                   make c-german"
	@echo ""

clean: linux-clean
distclean: linux-distclean

c-english:
	@echo ""
	@echo "Please choose your operating system:"
	@echo ""
	@echo "operating system          command"
	@echo "--------------------------------------------------------------"
	@echo "Linux                     make linux-english"
	@echo ""

c-german:
	@echo ""
	@echo "Bitte waehle Dein Betriebssystem aus:"
	@echo ""
	@echo "Betriebssystem            command"
	@echo "--------------------------------------------------------------"
	@echo "Linux                     make linux-german"
	@echo ""

linux-english:
	make -C linux english

linux-german:
	make -C linux german

linux-clean:
	-make -C linux clean

linux-distclean:
	-make -C linux distclean



all: choose-language

choose-language:
	@echo ""
	@echo "Please choose a language:"
	@echo ""
	@echo "language                  command"
	@echo "--------------------------------------------------------------"
	@echo "english                   make english"
	@echo "deutsch                   make german"
	@echo ""

clean:
	@echo ""
	@echo "Please choose your operating system:"
	@echo ""
	@echo "operating system          command"
	@echo "--------------------------------------------------------------"
	@echo "Linux                     make linux-clean"
	@echo ""

english:
	@echo ""
	@echo "Please choose your operating system:"
	@echo ""
	@echo "operating system          command"
	@echo "--------------------------------------------------------------"
	@echo "Linux                     make linux-english"
	@echo ""

german:
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
	make -C linux clean


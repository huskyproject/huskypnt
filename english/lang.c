/* english.c - english questions/texts */

#include "common.h"
#include "os.h"
#include "lang.h"

char *langDir = "english";

char *questions[numIdx+1] =
{
  "",
  "Run the fido-software as which group (default: %s)? ",
  "Run the fido-software as which user (default: %s)? ",
  "Which users shall be able to use the fido-software\n"
  "(e.g. \"sascha,tobi\")? ",
  "Put the libraries into (default: %s): ",
  "Put the programs into (default: %s): ",
  "Put the man-pages into (default: %s): ",
  "Put the configs into (default: %s): ",
  "Put the log-files into (default: %s): ",
  "Put the includes into (default: %s): ",
  "Install as debug-versions (0=no, 1=yes, default: %s)? ",
  "Does this system base on libc5 or on glibc2 (aka libc6) (libc5/glibc2)? ",
  "Where do you live? ",
  "What is your name (first- and surname)? ",
  "Working directory (default: %s)? ",
  "Inbound (default: %s)? ",
  "Protected Inbound (default: %s)? ",
  "LocalInbound (default: %s)? ",
  "TempInbound (default: %s)? ",
  "TempOutbound (default: %s)? ",
  "Outbound (default: %s)? ",
  "MsgBaseDir (default: %s)? ",
  "NodelistDir (default: %s)? ",
  "NetmailDir (default: %s)? ",
  "Put the scripts into (default: %s)? ",
  "ISDN-Port if available (suggested: /dev/ttyI2)? ",
  "Modem-Port if available (COM1: /dev/ttyS0, COM2: /dev/ttyS1, ...)? ",
  "If your COM-port has a FIFO (that's a buffer, found on UARTs of type\n"
  "16550 and 16650), you may set the baudrate as high as your modem\n"
  "supports (usually 57600 or 115200). But if it does not have a FIFO, it\n"
  "will loose data on high data rates. In that case you should set the\n"
  "baudrate to 19200 or 38400.\n"
  "You may change that setting later in ifcico.cfg if needed (e.g. \n"
  "CRC-errors while receiving or transmitting files).\n"
  "The faster your modem is, the higher the baudrate has to be to transport\n"
  "the data between computer and modem fast enough.\n"
  "Because of compression the data rates between computer and modem may be\n"
  "higher than the speed of the modem. I recommend the following baudrates:\n" 
  "14k4: 19200 Baud, 28k8: 38400 Baud, 33k6: 57600 Baud, 56k: 115200 Baud\n"
  "If you were able to use your modem with a high baudrate (>38400) under\n"
  "Windows it is very probable that you own a COM-port with FIFO, so you\n"
  "may safely use a high baudrate under Linux, too (exceptions confirm the\n"
  "rule :) ).\n"
  "Baudrate for Modem-Port? ",
  "International part of phonenumber (e.g. 49 for germany)? ",
  "Local part of phonenumber (e.g. 641 for Giessen)? ",
  "Last part of (Voice-)phonenumber (e.g. 3012958)? ",
  "Last part of (Data-)phonenumber (e.g. 3012959)? ",
  "Dialout prefix if needed (e.g. 0W)? ",
  "local prefix (usually 0)? ",
  "international prefix (usually 00)? ",
  "Which packer would you like to use (default: %s)? ",
  "Which point-number did you get (e.g. 5)? ",
  "What is the address of your uplink (e.g. 2:2435/424)? ",
  "What is the name of your uplink (first- and surname)? ",
  "Which password did your uplink give you? ",
  "Home directory of the new user (default: %s)? ",
  "Put the documentation on info-format into (default: %s): ",
  "Put the documentation on html-format into (default: %s): "
};

char *chooseBossText =
  "Now you should choose a so-called \"Boss-Node\" (also called uplink).\n"
  "That is the system where you get your mails. I recommend to choose a node\n"
  "from the list below, because they are able to help you if you run\n"
  "into any trouble (at least concerning fido :) ). It is mainly a question\n"
  "of cost, but because of the massive changes in prices for phone calls (at\n"
  "least in germany) there is not much difference anymore. Sometimes\n"
  "(especially for short calls) a remote system may be cheaper to reach\n"
  "than a local system (thanks to german telecom :( ). Also the possibility\n"
  "to transfer the fido mails over the Internet (so-called Fido over IP)\n"
  "makes the cost of any system in the world (that offers Fido over IP)\n"
  "equal.\n"
  "The following nodes accept points:\n"
  "\n%s\n"
  "Choose an uplink (does not need to be in that list, that are only\n"
  "suggestions) and discuss with him the settings (password etc.)\n"
  "while this program installs the software. When it has finished compiling\n"
  "and installing the fido system it will ask you what you and your uplink\n"
  "decided to use. \n"
  "If this text is too long you may scroll with Shift-PageUp/PageDn.\n";
 
char *pressReturnText = "<Return>\n";
char *allrightText = "Everything allright (%c/%c)? ";
char *notFoundText = "%s not found.\n";
char *pleaseInstallProgramsText =
  "Please install all needed programs first.\n";
char *pleaseCopyFilesText =
  "Please copy all needed files into the current directory first.\n";
char *groupExistsText = "\aGroup '%s' already exists, please add the users (%s)  to it on your own.\n";
char *userExistsText = "\aUser '%s' already exists.\n";
char *createdGroupText = "Created group %s (ID: %d) with the users %s in it.\n";
char *createdUserText = "Created user %s (ID: %d) in group #%d.\n"
  "Please choose a secure, but easy to remember password:\n";
char *creatingUserAndGroupText = "Creating user and group.\n";
char *creatingDirsText = "Creating directories.\n";
char *settingRightsText = "Setting rights.\n";
char *createdDirsText = "Created directories.\n";
char *unzipSourcesText = "Unpacking sources.\n";
char *creatingMakefileCfgsText = "Creating Makefile-Configs.\n";
char *createdMakefileCfgsText = "Created Makefile-Configs.\n";
char *compilingText = "Compiling %s...\n";
char *compiledText = "Compiled %s.\n";
char *installingText = "Installing %s...\n";
char *installedText = "Installed %s.\n";
char *pleaseExecuteAsRootText = "Please run this script as root.\n";
char *creatingCfgText = "Writing configuration.\n";
char *createdCfgText = "Wrote configuration.\n";
char *configLdsoText = "Setting up ld.so (dynamic libraries).\n";
char *LdsoEntryMadeText = "Made entry into /etc/ld.so.conf.\n";
char *configLdsoDoneText = "ld.so set up.\n";
char *creatingUserCfgText = "Creating configs for user '%s'.\n";
char *createdUserCfgText = "Created configs for user '%s'.\n";
char *configProfileText = "Setting up login-profiles (/etc/profiles).\n";
char *configProfileDoneText = "Login-profiles set up.\n";
char *copyingNodelistsText = "Copying nodelists.\n";
char *copiedNodelistsText = "Copied nodelists.\n";
char *copyingScriptsText = "Copying scripts.\n";
char *copiedScriptsText = "Copied scripts.\n";
char *compilingNodelistsText = "Compiling nodelists.\n";
char *compiledNodelistsText = "Compiled nodelists.\n";
char *installDoneText = "Finished installation.\n"
  "Please log in as %s now and just try \"huskyui\". :)\n"
  "Have fun!\n";
char *chdirErrorText = "Could not change to directory '%s'!\n";

char yesKey = 'y';
char noKey = 'n';


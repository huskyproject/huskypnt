/* english.c - english questions/texts */

#include "auto.h"
#include "common.h"
#include "os.h"
#include "lang.h"

char *langDir = "english";

char *configMenus =
  "configuration - main menu\n"
  "\n"
  " 1) system config\n"
  " 2) personal config\n"
  " 3) fido directory config\n"
  " 4) communications setup\n"
  "\n"
  " x) continue\n"
  "\n";

char *menuInvalidChoice = "Invalid choice! Try again.\n";

char *systemConfigTitle = "configuration - system config\n\n";
tMenuEntry systemConfigEntries[numSystemConfigEntries] =
{
  { '1', " 1) fido group: %s\n", groupNameIdx },
  { '2', " 2) fido-admin user: %s\n", fidoNameIdx },
  { '3', " 3) fido-admin group: %s\n", admGroupNameIdx },
  { '4', " 4) allowed users: %s\n", usersIdx },
  { '5', " 5) libc version: %s\n", libcVersionIdx },
  { '6', " 6) generate debug versions: %s\n", debugIdx },
  { '7', " 7) library directory: %s\n", libDirIdx },
  { '8', " 8) binary directory: %s\n", binDirIdx },
  { '9', " 9) man-page directory: %s\n", manDirIdx },
  { 'a', " a) config directory: %s\n", cfgDirIdx },
  { 'b', " b) logfile directory: %s\n", logDirIdx },
  { 'c', " c) include directory: %s\n", incDirIdx },
  { 'd', " d) info directory: %s\n", infoDirIdx },
  { 'e', " e) html directory: %s\n", htmlDirIdx },
  { 'f', " f) script directory: %s\n\n", scriptDirIdx },
};

char *personalConfigTitle = "configuration - personal config\n\n";
tMenuEntry personalConfigEntries[numPersonalConfigEntries] =
{
  { '1', " 1) location: %s\n", locationIdx },
  { '2', " 2) system operator: %s\n", sysOpNameIdx }
};

char *fidoDirConfigTitle = "configuration - fido directory config\n\n";
tMenuEntry fidoDirConfigEntries[numFidoDirConfigEntries] =
{
  { '1', " 1) working directory: %s\n", workDirIdx },
  { '2', " 2) insecure inbound: %s\n", inboundIdx },
  { '3', " 3) protected inbound: %s\n", protInboundIdx },
  { '4', " 4) local inbound: %s\n", localInboundIdx },
  { '5', " 5) outbound: %s\n", outboundIdx },
  { '6', " 6) temporary inbound: %s\n", tempInboundIdx },
  { '7', " 7) temporary outbound: %s\n", tempOutboundIdx },
  { '8', " 8) message base directory: %s\n", msgbaseDirIdx },
  { '9', " 9) nodelist directory: %s\n", nodelistDirIdx },
  { 'a', " a) netmail directory: %s\n", netmailDirIdx }
};

char *commConfigTitle = "configuration - communications setup\n\n";
tMenuEntry commConfigEntries[numCommConfigEntries] =
{
  { '1', " 1) isdn device: %s\n", isdnDevIdx },
  { '2', " 2) modem device: %s\n", modemDevIdx },
  { '3', " 3) computer<->modem baudrate: %s\n", modemBaudIdx },
  { '4', " 4) international prefix: %s\n", internatPrefixIdx },
  { '5', " 5) local prefix: %s\n", localPrefixIdx },
  { '6', " 6) voice number: %s\n", voiceNumIdx },
  { '7', " 7) data number: %s\n", dataNumIdx },
  { '8', " 8) dialout number: %s\n", amtNumIdx },
  { '9', " 9) local number: %s\n", localNumIdx },
  { 'a', " a) international number: %s\n", internatNumIdx }
};

char *uplinkConfigTitle = "uplink configuration\n\n";
tMenuEntry uplinkConfigEntries[numUplinkConfigEntries] =
{
  { '1', " 1) point number: %s\n", pointNrIdx },
  { '2', " 2) uplink address: %s\n", uplinkAddrIdx },
  { '3', " 3) uplink name: %s\n", uplinkNameIdx },
  { '4', " 4) uplink password: %s\n", uplinkPwdIdx },
  { '5', " 5) packer: %s\n", packerIdx }
};

char *homeDirQuestion = "Homeverzeichnis des neuen Users (default: %s)? ";

char *descTexts[numIdx] =
{
  "Run the fido-software as which group? \n\n",
  "Run the fido-software as which user? \n\n",
  "Which users shall be able to use the fido-software\n"
  "(e.g. \"sascha,tobi\")? \n\n",
  "Put the libraries into: \n\n",
  "Put the programs into: \n\n",
  "Put the man-pages into: \n\n",
  "Put the configs into: \n\n",
  "Put the log-files into: \n\n",
  "Put the includes into: \n\n",
  "Install as debug-versions (0=no, 1=yes, default: %s)? \n\n",
  "Does this system base on libc5 or on glibc2 (aka libc6) (libc5/glibc2)? \n\n",
  "Where do you live? \n\n",
  "What is your name (first- and surname)? \n\n",
  "Working directory? \n\n",
  "Inbound? \n\n",
  "Protected Inbound? \n\n",
  "LocalInbound? \n\n",
  "TempInbound? \n\n",
  "TempOutbound? \n\n",
  "Outbound? \n\n",
  "MsgBaseDir? \n\n",
  "NodelistDir? \n\n",
  "NetmailDir? \n\n",
  "Put the scripts into? \n\n",
  "ISDN-Port if available (suggested: /dev/ttyI2)? \n\n",
  "Modem-Port if available (COM1: /dev/ttyS0, COM2: /dev/ttyS1, ...)? \n\n",
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
  "Baudrate for Modem-Port? \n\n",
  "International part of phonenumber (e.g. 49 for germany)? \n\n",
  "Local part of phonenumber (e.g. 641 (not 0641!) for Giessen)? \n\n",
  "Last part of (Voice-)phonenumber (e.g. 3012958)? \n\n",
  "Last part of (Data-)phonenumber (e.g. 3012959)? \n\n",
  "Dialout prefix if needed (e.g. 0W)? \n\n",
  "local prefix (usually 0)? \n\n",
  "international prefix (usually 00)? \n\n",
  "Which packer would you like to use? \n\n",
  "Which point-number did you get (e.g. 5)? \n\n",
  "What is the address of your uplink (e.g. 2:2435/424)? \n\n",
  "What is the name of your uplink (first- and surname)? \n\n",
  "Which password did your uplink give you? \n\n",
  "Home directory of the new user? \n\n",
  "Put the documentation on info-format into: \n\n",
  "Put the documentation on html-format into: \n\n",
  "",
  "Use which group for accessing user files? \n\n",
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
  "The following english-speaking nodes accept points:\n"
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
char *groupExistsText = "\aGroup '%s' already exists, please add the users (%s) to it on your own.\n";
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


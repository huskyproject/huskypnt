/* german.h - german questions/texts */

#ifndef __GERMAN_H__
#define __GERMAN_H__

char *questions[numIdx+1] =
{
  "",
  "Unter welcher Gruppe soll das Fido-System laufen (default: %s)? ",
  "Unter welchem User soll das Fido-System laufen (default: %s)? ",
  "Welche User sollen das Fido-System benutzen koennen\n"
  "(z.B. \"sascha,tobi\")? ",
  "In welchem Verzeichnis sollen die Libs abgelegt werden (default: %s)? ",
  "In welchem Verzeichnis sollen die Programme abgelegt werden (default: %s)? ",
  "In welchem Verzeichnis sollen die Man-Pages abgelegt werden (default: %s)? ",
  "In welchem Verzeichnis sollen die Configs abgelegt werden (default: %s)? ",
  "In welchem Verzeichnis sollen die Logs abgelegt werden (default: %s)? ",
  "In welchem Verzeichnis sollen die Includes abgelegt werden (default: %s)? ",
  "Als Debug-Versionen compilieren (0 fuer nein, 1 fuer ja, default: %s)? ",
  "Ist dies ein libc5- oder glibc2-System (libc5/glibc2)? ",
  "Wo wohnst Du? ",
  "Wie heisst Du (Vor- und Nachname)? ",
  "Arbeitsverzeichnis (default: %s)? ",
  "Wo soll der Inbound liegen (default: %s)? ",
  "Wo soll der Protected Inbound liegen (default: %s)? ",
  "Wo soll der LocalInbound liegen (default: %s)? ",
  "Wo soll der TempInbound liegen (default: %s)? ",
  "Wo soll der TempOutbound liegen (default: %s)? ",
  "Wo soll der Outbound liegen (default: %s)? ",
  "Wo sollen die Areas angelegt werden (default: %s)? ",
  "Wo sollen die Nodelisten abgelegt werden (default: %s)? ",
  "Wo soll die NetmailArea angelegt werden (default: %s)? ",
  "In welchem Verzeichnis sollen die Scripts abgelegt werden (default: %s)? ",
  "ISDN-Port falls vorhanden (empfohlen: /dev/ttyI2)? ",
  "Modem-Port falls vorhanden (COM1: /dev/ttyS0)? ",
  "Wenn Du ueber einen COM-Port mit FIFO (UART 16550) verfuegst,\n"
  "kannst Du die Baudrate so hoch einstellen, wie Dein Modem\n"
  "vertraegt (57600 oder 115200). Wenn der Port keinen FIFO (Puffer)\n"
  "hat, gehen bei Baudraten ueber 38400 Daten verloren.\n"
  "Du kannst die Baudrate auch nachtraeglich noch in der ifcico.cfg\n"
  "aendern, falls Fehler (haeufig sind es CRC-Fehler) auftreten.\n"
  "Je schneller Dein Modem ist, desto hoeher muss auch die Baudrate\n"
  "der Schnittstelle sein, damit die Daten schnell genug vom Modem\n"
  "zum Rechner abtransportiert werden koennen. Folgende Werte sind\n"
  "mindestens zu empfehlen (durch Kompression sind hoehere Geschwindigkeiten\n"
  "als beim Modem angegeben moeglich):\n"
  "14k4: 19200 Baud, 28k8: 38400 Baud, 33k6: 57600 Baud, 56k: 115200 Baud\n"
  "Wenn Dein Modem unter DOS/Windows mit einer hohen Baudrate (ueber 38400)\n"
  "funktioniert hat, kannst Du davon ausgehen, dass Dein COM-Port\n"
  "einen FIFO hat.\n"
  "Baudrate fuer Modem-Port? ",
  "Internationale Vorwahl (z.B. 49 fuer Deutschland)? ",
  "Ortsvorwahl (z.B. 641 fuer Giessen)? ",
  "Eigene (Sprach-)Telefonnumer (z.B. 3012958)? ",
  "Eigene (Daten-)Telefonnumer (z.B. 3012959)? ",
  "Amtskennziffer fuer Telefonanlage (z.B. 0-)? ",
  "Ortskennziffer (normalerweise 0)? ",
  "Auslandskennziffer (normalerweise 00)? ",
  "Welchen Packer moechtest Du benutzen (default: %s)? ",
  "Welche Pointnummer hast Du (z.B. 5)? ",
  "Welche Adresse hat der Uplink (z.B. 2:2435/424)? ",
  "Wie heisst der Uplink (Vor- und Nachname)? ",
  "Welches Passwort wurde vereinbart? ",
  "Homeverzeichnis des neuen Users (default: %s)? ",
  "In welchem Verzeichnis soll die Doku im Info-Format abgelegt werden\n"
  "(default: %s)? ",
  "In welchem Verzeichnis soll die Doku im HTML-Format abgelegt werden\n"
  "(default: %s)? "
};

char *chooseBossText =
  "Nun solltest Du Dir schonmal einen sogenannten \"Boss-Node\" (auch Uplink\n"
  "genannt) aussuchen. Von diesem System bekommst Du dann Deine Mails.\n"
  "Ich empfehle, einen Node aus dem Netz 2435 zu waehlen, denn dort gibt es\n"
  "Leute, die Dir bei Problemen helfen.\n"
  "Hauptkriterium waren bisher die Telefonkosten. Inzwischen koennte es evtl.\n"
  "billiger sein, einen Node in Ferntarif zu waehlen und dann einen\n"
  "Call-by-Call-Anbieter mit sekundengenauer Abrechnung zu nutzen.\n\n"
  "Folgende Nodes im Netz 2435 nehmen Points auf:\n"
  "\n%s\n"
  "Waehle Dir einen Uplink aus und besprich mit ihm die\n"
  "Einstellungen (Passwort etc.), waehrend dieses Script das\n"
  "Pointpaket installiert. Nachdem die Programme fertig compiliert und\n"
  "installiert sind, werden die Einstellungen abgefragt und in die\n"
  "Configs uebernommen.\n\n"
  "Falls dieser Text zu lang ist, kannst Du mit Shift-PageUp/PageDn\n"
  "hoch- und runterscrollen.\n";

char *pressReturnText = "<Return>\n";
char *allrightText = "Alles richtig (%c/%c)? ";
char *notFoundText = "%s nicht gefunden.\n";
char *pleaseInstallProgramsText =
  "Bitte erst alle benoetigten Programme installieren.\n";
char *pleaseCopyFilesText =
  "Bitte erst alle benoetigten Dateien ins aktuelle Verzeichnis\n"
  "kopieren.\n";
char *groupExistsText = "\aGruppe '%s' bereits vorhanden, bitte die User (%s) von Hand eintragen\n";
char *userExistsText = "\aUser '%s' bereits vorhanden.\n";
char *createdGroupText = "Gruppe %s (ID: %d) mit den Usern %s angelegt.\n";
char *createdUserText = "User %s (ID: %d) in Gruppe #%d angelegt.\n"
  "Bitte nun ein sicheres, aber leicht zu merkendes Passwort vergeben:\n";
char *creatingUserAndGroupText = "Lege User und Gruppe an.\n";
char *creatingDirsText = "Lege Verzeichnisse an.\n";
char *settingRightsText = "Setze Rechte.\n";
char *createdDirsText = "Verzeichnisse angelegt.\n";
char *unzipSourcesText = "Entpacke Sources.\n";
char *creatingMakefileCfgsText = "Erzeuge Makefile-Configs.\n";
char *createdMakefileCfgsText = "Makefile-Configs erzeugt.\n";
char *compilingText = "Compiliere %s...\n";
char *compiledText = "%s compiliert.\n";
char *installingText = "Installiere %s...\n";
char *installedText = "%s installiert.\n";
char *pleaseExecuteAsRootText = "Dieses Script bitte als root ausfuehren.\n";
char *creatingCfgText = "Schreibe Config.\n";
char *createdCfgText = "Config geschrieben.\n";
char *configLdsoText = "Konfiguriere ld.so (dynamische Libraries).\n";
char *LdsoEntryMadeText = "Eintrag in /etc/ld.so.conf vorgenommen.\n";
char *configLdsoDoneText = "ld.so konfiguriert.\n";
char *creatingUserCfgText = "Erstelle Configs fuer User '%s'.\n";
char *createdUserCfgText = "Configs fuer User '%s' erstellt.\n";
char *configProfileText = "Konfiguriere Login-Profiles.\n";
char *configProfileDoneText = "Login-Profiles konfiguriert.\n";
char *copyingNodelistsText = "Kopiere Nodelisten.\n";
char *copiedNodelistsText = "Nodelisten kopiert.\n";
char *copyingScriptsText = "Kopiere Scripts.\n";
char *copiedScriptsText = "Scripts kopiert.\n";
char *compilingNodelistsText = "Compiliere Nodelisten.\n";
char *compiledNodelistsText = "Nodelisten compiliert.\n";
char *installDoneText = "Installation abgeschlossen.\n"
  "Log Dich jetzt bitte als %s ein und ruf einfach mal \"huskyui\" auf. :)\n";
char *chdirErrorText = "Konnte nicht zu Verzeichnis '%s' wechseln!\n";

char yesKey = 'j';
char noKey = 'n';

#endif


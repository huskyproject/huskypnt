/* german.c - german menus/texts */

#include "common.h"
#include "os.h"
#include "lang.h"

char *langDir = "german";

char *systemConfigTitle = "configuration - system config\n\n";
tMenuEntry systemConfigEntries[numSystemConfigEntries] =
{
  { '1', " 1) Fido-Gruppe: %s\n", groupNameIdx },
  { '2', " 2) Fido-Admin-Benutzer: %s\n", fidoNameIdx },
  { '3', " 3) Erlaubte Benutzer: %s\n", usersIdx },
  { '4', " 4) libc-Version: %s\n", libcVersionIdx },
  { '5', " 5) Debug-Versionen erstellen: %s\n", debugIdx },
  { '6', " 6) Bibliotheks-Verzeichnis: %s\n", libDirIdx },
  { '7', " 7) Programm-Verzeichnis: %s\n", binDirIdx },
  { '8', " 8) Hilfeseiten-Verzeichnis: %s\n", manDirIdx },
  { '9', " 9) Config-Verzeichnis: %s\n", cfgDirIdx },
  { 'a', " a) Verzeichnis fuer Logdateien: %s\n", logDirIdx },
  { 'b', " b) Include-Verzeichnis: %s\n", incDirIdx },
  { 'c', " c) Verzeichnis mit Dokus im Info-Format: %s\n", infoDirIdx },
  { 'd', " d) Verzeichnis mit Dokus im HTML-Format: %s\n", htmlDirIdx },
  { 'e', " e) Script-Verzeichnis: %s\n", scriptDirIdx },
};

char *personalConfigTitle = "configuration - personal config\n\n";
tMenuEntry personalConfigEntries[numPersonalConfigEntries] =
{
  { '1', " 1) Standort: %s\n", locationIdx },
  { '2', " 2) Systembetreiber (also Dein Name :) ): %s\n", sysOpNameIdx }
};

char *fidoDirConfigTitle = "configuration - fido directory config\n\n";
tMenuEntry fidoDirConfigEntries[numFidoDirConfigEntries] =
{
  { '1', " 1) Arbeitsverzeichnis: %s\n", workDirIdx },
  { '2', " 2) ungesicherter Inbound: %s\n", inboundIdx },
  { '3', " 3) gesicherter Inbound: %s\n", protInboundIdx },
  { '4', " 4) lokaler Inbound: %s\n", localInboundIdx },
  { '5', " 5) Outbound: %s\n", outboundIdx },
  { '6', " 6) temporaerer Inbound: %s\n", tempInboundIdx },
  { '7', " 7) temporaerer Outbound: %s\n", tempOutboundIdx },
  { '8', " 8) MessageBase-Verzeichnis: %s\n", msgbaseDirIdx },
  { '9', " 9) Nodelist-Verzeichnis: %s\n", nodelistDirIdx },
  { 'a', " a) Netmail-Verzeichnis: %s\n", netmailDirIdx }
};

char *commConfigTitle = "configuration - communications setup\n\n";
tMenuEntry commConfigEntries[numCommConfigEntries] =
{
  { '1', " 1) isdn device: %s\n", isdnDevIdx },
  { '2', " 2) modem device: %s\n", modemDevIdx },
  { '3', " 3) internationale Vorwahl: %s\n", internatPrefixIdx },
  { '4', " 4) Orts-Vorwahl: %s\n", localPrefixIdx },
  { '5', " 5) Telefonnummer (Sprache): %s\n", voiceNumIdx },
  { '6', " 6) Telefonnummer (Daten): %s\n", dataNumIdx },
  { '7', " 7) Amts-Ziffer: %s\n", amtNumIdx },
  { '8', " 8) Orts-Ziffer: %s\n", localNumIdx },
  { '9', " 9) Auslands-Ziffer: %s\n", internatNumIdx }
};

char *uplinkConfigTitle = "uplink configuration\n\n";
tMenuEntry uplinkConfigEntries[numUplinkConfigEntries] =
{
  { '1', " 1) Point-Nr.: %s\n", pointNrIdx },
  { '2', " 2) Fido-Adresse des Uplinks: %s\n", uplinkAddrIdx },
  { '3', " 3) Name des Uplinks: %s\n", uplinkNameIdx },
  { '4', " 4) Passwort: %s\n", uplinkPwdIdx },
  { '5', " 5) Packer: %s\n", packerIdx }
};

char *homeDirQuestion = "Homeverzeichnis des neuen Users (default: %s)? ";

char *descTexts[numIdx] =
{
  "In diese Gruppe werden alle User eingetragen, die das Fido-System\n"
  "benutzen duerfen\n\n",

  "Tosser etc. laufen unter diesem User und haben damit quasi Admin-Rechte\n"
  "fuer die Fido-Software, d.h. sie koennen Passwoerter lesen, Configs\n"
  "veraendern etc.\n\n",

  "Diese User werden in die Gruppe der User, die das Fido-System benutzen\n"
  "duerfen, eingetragen. Bsp:\n"
  "sascha,tobi\n\n",

  "In diesem Verzeichnis werden die Programm-Bibliotheken abgelegt.\n\n",

  "In diesem Verzeichnis werden die Programme abgelegt.\n\n",

  "In diesem Verzeichnis werden die Hilfe-Seiten abgelegt.\n\n",

  "In diesem Verzeichnis werden die Configs abgelegt.\n\n",

  "In diesem Verzeichnis werden die Logs abgelegt.\n\n",

  "In diesem Verzeichnis werden die Includes abgelegt.\n\n",

  "Als Debug-Versionen compilieren (0 fuer nein, 1 fuer ja)?\n\n",

  "Ist dies ein libc5- oder glibc2-System (libc5/glibc2)?\n\n",

  "Wo wohnst Du?\n\n",

  "Wie heisst Du (Vor- und Nachname)?\n\n",

  "In diesem Verzeichnis werden waehrend des Betriebs einige (teilweise\n"
  "temporaere) Dateien abgelegt, wie z.B. Semaphores (Dateien, die durch\n"
  "ihre Existenz eine bestimmte Aktion bewirken).\n\n",

  "Hier landen ankommende Dateien, z.B. von Filerequests.\n\n",

  "Hier landen ankommende Dateien von passwort-geschuetzten Verbindungen,\n"
  "z.B. Mails vom Uplink.\n\n",

  "Hier kann man PKTs reinwerfen, die nicht auf Passwort etc.\n
  "ueberprueft werden, z.B. um eine Mail automatisch zu posten.\n\n",

  "Hierhin werden die PKTs entpackt, um sie dann zu tossen.\n\n",

  "Hier waehrend die PKTs waehrend des Tossens zwischengelagert,\n"
  "um sie dann zu packen.\n\n",

  "BSO (Binkley-Style-Outbound), hier werden rausgehende Mails\n"
  "fuer den Versand bereitgehalten.\n\n",

  "In diesem Verzeichnis landen alle automatisch angelegten Areas.\n\n",

  "In diesem Verzeichnis werden die Logfiles gespeichert.\n\n",

  "Hier werden die Nodelisten abgelegt.\n\n",

  "In diesem Verzeichnis werden die Scripts zum Tossen, Scannen, etc.\n"
  "abgelegt\n\n",

  "Falls Du ISDN hast, solltest Du hier den ISDN-Port angeben.\n"
  "Unter Linux empfehle ich /dev/ttyI2 zu nutzen, da ttyI0 und ttyI1\n"
  "oft schon von anderen Programmen (z.B. PPP) benutzt werden.\n"
  "Unter DOS und Windows muss ein Modem-Emulator wie z.B. CFos\n"
  "installiert sein, da der Mailer nicht direkt auf die CAPI\n"
  "zugreifen kann.\n\n",

  "Falls Du ein analoges Modem hast, solltest Du hier die Modem-Port\n"
  "angeben.\n"
  "Fuer Linux gilt meisst: Was unter DOS COM1: war, ist unter Linux\n"
  "/dev/ttyS0. COM2: wird zu /dev/ttyS1 usw.\n\n",

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
  "einen FIFO hat.\n\n",

  "Internationale Vorwahl\n"
  "Deutschland: 49\n\n",

  "Ortsvorwahl (z.B. 641 fuer Giessen)\n\n",

  "Eigene (Sprach-)Telefonnumer ohne Vorwahl (z.B. 3012958)\n\n",

  "Eigene (Daten-)Telefonnumer ohne Vorwahl (z.B. 3012959)\n\n",

  "Amtskennziffer fuer Telefonanlage, z.B. 0W\n\n",

  "Ortskennziffer\n"
  "Deutschland: 0\n\n",

  "Auslandskennziffer\n"
  "Deutschland: 00\n\n",

  "Der Packer (auch Archiver genannt) komprimiert (packt) alle rausgehenden\n"
  "Mails, damit die Uebertragungszeit kuerzer ist und die Telefonkosten\n"
  "somit geringer sind.\n\n",

  "Zusammen mit der Adresse Deines Uplinks ergibt Deine Pointnummer\n"
  "eine weltweit eindeutige Adresse, ueber die Du per Netmail erreicht\n"
  "werden kannst.\n\n",

  "Die Adresse Deines Uplinks. Alle rausgehenden Mailpakete werden an\n"
  "diese Adresse geschickt.\n\n",

  "Wie heisst der Uplink (Vor- und Nachname)?\n\n",

  "Es werden nur Mailpakete angenommen, in denen dieses Passwort enthalten\n"
  "ist. Dein Uplink laesst Dich auch nur dann Mails verschicken und\n"
  "abholen, wenn das Passwort vom Mailer praesentiert wird.\n"
  "Dadurch kann kein Fremder einfach Deine Mails lesen oder unter Deinem\n"
  "Namen Mails schreiben, solange Du das Passwort nicht weitergibts.\n"
  "Zumindest theoretisch. Dein Uplink z.B. kann diesen Schutz umgehen,\n"
  "aber der hat Dein Passwort sowieso. :)\n\n",

  "In diesem Verzeichnis wird die Doku im Info-Format abgelegt.\n\n",

  "In diesem Verzeichnis wird die Doku im HTML-Format abgelegt.\n\n",
  ""
};

char *chooseBossText =
  "Nun solltest Du Dir schonmal einen sogenannten \"Boss-Node\" (auch Uplink\n"
  "genannt) aussuchen. Von diesem System bekommst Du dann Deine Mails.\n"
  "Ich empfehle, einen der unten aufgelisteten Nodes zu waehlen, denn die\n"
  "koennen Dir bei Problemen (mit Deiner Fido-Software :) ) gut helfen.\n"
  "Hauptkriterium waren bisher die Telefonkosten. Inzwischen koennte es evtl.\n"
  "billiger sein, einen Node in Ferntarif zu waehlen und dann einen\n"
  "Call-by-Call-Anbieter mit sekundengenauer Abrechnung zu nutzen.\n\n"
  "Folgende deutschsprachigen Nodes nehmen Points auf:\n"
  "\n%s\n"
  "Waehle Dir einen Uplink aus und besprich mit ihm die\n"
  "Einstellungen (Passwort etc.), waehrend dieses Script das\n"
  "Pointpaket installiert. Nachdem die Programme fertig compiliert und\n"
  "installiert sind, werden die Einstellungen abgefragt und in die\n"
  "Configs uebernommen.\n\n"
  "Falls dieser Text zu lang ist, kannst Du (zumindest unter Linux)\n"
  "mit Shift-PageUp/PageDn hoch- und runterscrollen.\n";

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


/* platform-specific header file, linux version */

#define UNIX
#define LINUX
#define OS "Linux"
#define dirSepC '/'
#define dirSepS "/"

char *defaults[numIdx+1] =
{
  "",
  "fido",
  "fido",
  "",
  "/usr/local/lib",
  "/usr/local/bin",
  "/usr/local/man",
  "/usr/local/etc/fido",
  "/var/log/fido",
  "/usr/local/include",
  "0",
  "",
  "",
  "",
  "/var/spool/fido/work",
  "/var/spool/fido/in",
  "/var/spool/fido/in/protect",
  "/var/spool/fido/in/local",
  "/var/spool/fido/in/arctmp",
  "/var/spool/fido/out/tmp",
  "/var/spool/fido/out/zone",
  "/var/spool/fido/echo",
  "/var/spool/fido/nodelist",
  "/var/spool/fido/netmail",
  "/usr/local/scripts",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "",
  "zip",
  "",
  "",
  "",
  "",
  "",
  "",
  ""
};

// returns 0 if file exists
int fexist(char *fname);

// returns 0 if directory exists
int direxist(char *dname);

// returns 0 if successfull
int copyFile(char *sourceName, char *destName);

// returns 0 if successfull
int touchFile(char *fname);

// returns 0 if successfull
int linkFile(char *source, char *dest);

// create directory and parent directories (if needed), returns 0 if
// successfull
int mkdirp(char *dirname);


# README for lwpp
# created 1999-Aug-15 19:28 jmk
# autodate: 1999-Aug-16 03:42

+----------+
| Contents |
+----------+

  - Description
  - History
  - Other Documentation
  - License
  - Disclaimer

+-------------+
| Description |
+-------------+

Lwpp is a lightweight text file preprocessor.  Lwpp reads a file as
input and writes sections of the file as output, based on certain
conditions embedded in the file as preprocessor directives.  Lwpp is
meant mostly to emulate the C preprocessor's `#if[n]def ... #endif'
functionality, but without any of cpp's other C-oriented features
(including macro expansion), and without putting extra newlines into
the output.  Lwpp uses the environment as a dictionary of symbol
definitions, so you can use the value of regular environment variables
(such as HOME or HOST) as symbol definitions without having to define
them again on the command line.

I created lwpp out of a need to preprocess spec files for RPM packages,
so that i could build more than one variant of a package from one
specfile.  I started out using GNU m4 for that purpose, but i
discovered some unfortunate limitations in the m4 language dealing with
commas.  Hence, lwpp.  I hope it's useful to others as well.

+---------+
| History |
+---------+

v1.0 1999-Aug-15

    Initial public release.

+---------------------+
| Other Documentation |
+---------------------+

The lwpp distribution should contain the following other documents:

  INSTALL           - Describes how to install lwpp.
  Lwpp-HOWTO.txt    - Describes how to use lwpp.

+---------+
| License |
+---------+

Lwpp is Copyright © 1999 by Jim Knoble.

Lwpp is open source software and is licensed under version 2 of the GNU
General Public License (GPL) <ftp://ftp.gnu.org/gnu/COPYING-2.0>, or,
at your option, any later version <ftp://ftp.gnu.org/gnu/COPYING>.  For
more information about open source software, please see the Open Source
Initiative's website <http://www.opensource.org/>.

+------------+
| Disclaimer |
+------------+

The software is provided "as is", without warranty of any kind, express
or implied, including but not limited to the warranties of
merchantability, fitness for a particular purpose and noninfringement.
In no event shall the author(s) be liable for any claim, damages or
other liability, whether in an action of contract, tort or otherwise,
arising from, out of or in connection with the software or the use or
other dealings in the software.

Eat your vegetables.

-- 
Jim Knoble
http://www.pobox.com/~jmknoble/


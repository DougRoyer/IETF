
These tools were written by Doug Royer / Doug@SoftwareAndServices.NET

Tools and open source code for IETF protocols.

Documentation:

	Doxyfies exist in several directories.
	Tested using Doxygen version: 1.8.3.1
	(http://www.docygen.org)

	To build the documentation:

		From the command line in each Documentation directory and
		type 'doxygen'.

		It will place the html, manual pages, and xml files
		in directories under Generated the Documentation
		directory you ran doxygen.

		Point your browser at Documentation/Generated/html/index.html
		to see the generated documentation.

Projects:

	libABNF:
		An RFC-4234 ABNF parser. Copy the RFC into a new file, cut
		out everyting except the abnf and feed to this library.

		The next phase will generate C++ objects and code that
		will generate and accept the protocol.

		Other languages are planned.

		Builds:
			Visual Studio 2012
			Solaris with GNU makefile (require some work)







SUFFIXES	=	.cpp

CC		=	CC
CCC		=	CC

.cpp.o:			CC $(CCFLAGS) $(OUTPUT_OPTION) $<

COMPILE.cc	=	CC $(CCFLAGS) $(CFLAGS) -c


CPPFLAGS	=	-I.. -g
CCFLAGS		=	-I.. -g
CFLAGS		=	-I.. -g
LDFLAGS		=	-g

SOURCES.cpp	=	\
			main.cpp

OBJECTS		=	$(SOURCES.cpp:.cpp=.o)

ABNF : $(OBJECTS)
	CC -o $@ $(OBJECTS) -L../SunOS.59.sparc -R../SunOS.59.sparc -lABNF

clean :
	$(RM) -f $(OBJECTS) ABNF




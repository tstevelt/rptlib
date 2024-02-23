
PROG = rptlib

ifeq ($(LDFLAGS),"-m32")
LIB = /usr/local/lib/rptlib.a
else
LIB = /usr/local/lib64/rptlib.a
endif

DEF = /usr/local/include

FILES = \
	$(LIB)(rptinit.o)	\
	$(LIB)(rptcat.o)	\
	$(LIB)(rptdeliv.o)	\
	$(LIB)(rptfuncs.o)	\
	$(LIB)(rptheader.o)	\
	$(LIB)(rptextension.o)	\
	$(LIB)(rptline.o)	\
	$(LIB)(rpttotal.o)	\
	$(LIB)(rptclose.o)	\
	$(LIB)(ValidReportFormat.o)

.SILENT:

$(LIB): $(FILES)
	make include

$(FILES): rptlib.h rptprivate.h

include: $(DEF)/rptlib.h

$(DEF)/rptlib.h: rptlib.h 
	cp -pv rptlib.h $(DEF)/rptlib.h

strip:
	ls -l $(LIB)
	strip --strip-unneeded $(LIB)
	ls -l $(LIB)

clean:
	rm -f $(LIB)

all:
	make clean
	make

.PRECIOUS: $(LIB)


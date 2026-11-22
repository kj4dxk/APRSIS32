
FTPDIR =    /ftp/earthview
CGIDIR =    /usr/local/etc/httpd/cgi-bin
PBMDIR =    ~kelvin/unixtools/netpbm/netpbm/ppm

CFLAGS = -O2
CC	= acc
#CC	 = cc

OBJS	= vplanet.o astro.o gifout.o elements.o orbitcalc.o

vplanet:    $(OBJS)
	cc -o vplanet $(OBJS) -lm
	cp -p vplanet /usr/local/etc/httpd/cgi-bin/vplanet.t
	@rm -f core

vplanet.o:  vplanet.c vplanet.h

astro.o:    astro.c vplanet.h

gifout.o:   gifout.c vplanet.h

elements.o: elements.c elements.h

clean:
	rm -f *.o vplanet *.bak core *.out *.p?m *.gif

#   Collect pieces from various directories as installed and archive
#   in the build directory.  Think of this as "reverse install"

collect:
	rm -rf ftp-dir cgi-dir ppm-dir
	mkdir ftp-dir
	mkdir cgi-dir
	mkdir ppm-dir
	cp $(FTPDIR)/*.html ftp-dir
	cp $(FTPDIR)/*.gif ftp-dir
	cp $(CGIDIR)/Earth cgi-dir
	cp $(CGIDIR)/vplanet-post.html cgi-dir
	cp $(CGIDIR)/*.bmp cgi-dir
	cp $(CGIDIR)/*.tle cgi-dir
	cp $(CGIDIR)/wx-bground.ppm cgi-dir
	cp $(CGIDIR)/WeatherMan cgi-dir
	cp $(PBMDIR)/ppmtowhump.c ppm-dir
	cp $(PBMDIR)/ppmmolw.c ppm-dir

#   Make source release archive

archive:
	make clean
	rm -f /tmp/earthview.tar /tmp/earthview.tar.gz
	tar cfv /tmp/earthview.tar .
	gzip /tmp/earthview.tar
	rm $(FTPDIR)/source/earthview.tar.gz
	cp /tmp/earthview.tar.gz $(FTPDIR)/source/earthview.tar.gz

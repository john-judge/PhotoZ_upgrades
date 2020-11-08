
# sccs related makefile for unix


whoops:
        @echo "This makefile is only for version control on Solaris - use Makefile.vc"

source:
        @echo "Getting libtiff SCCS files"
        @sccs get -s SCCS/s.*
        @cd libtiff; sccs get -s SCCS/s.*

clean:
        @echo "Clean libtiff SCCS files"
        @sccs clean
        @cd libtiff; sccs clean




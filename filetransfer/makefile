    PROGS = fileserver fileclient  
    all:$(PROGS)  
    .PHONY:all  
    fileserver:  
            gcc -g fileserver.c common.h -o fileserver  
    fileclient:  
            gcc -g fileclient.c common.h -o fileclient  
      
    clean:  
            rm $(PROGS)  
    distclean:  
            rm *~ $(PROGS)  

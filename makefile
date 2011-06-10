### Name des Projekts (der Auszuführenden Datei)
EXENAME = tme

### Benötigte Bestanteile
OBJECTS  = 
OBJECTS += obj/${EXENAME}.o 
#OBJECTS += obj/main.o 
#OBJECTS += obj/cfg.o 
#OBJECTS += obj/menu.o 
#OBJECTS += obj/util.o 
#OBJECTS += obj/joypad.o 
#OBJECTS += obj/game.o 
#OBJECTS += obj/isomap.o 

### Linux Variante:
LIBS  =`pkg-config --libs
LIBS += allegro-5.0
#LIBS += allegro_acodec-5.0
#LIBS += allegro_audio-5.0   
#LIBS += allegro_color-5.0   
#LIBS += allegro_dialog-5.0  
LIBS += allegro_font-5.0    
LIBS += allegro_image-5.0    
#LIBS += allegro_main-5.0     
#LIBS += allegro_memfile-5.0  
#LIBS += allegro_physfs-5.0
#LIBS += allegro_primitives-5.0
LIBS += allegro_ttf-5.0
LIBS +=`

CPPFLAGS=-Wall -O3 -s
CPPFLAGS+=-pedantic
### Zusätzliche Flacs, weiß noch nicht genau ihre bedeutung
#CPPFLAGS+=-Wextra -ansi -Isrc

### Zu verwendene Programme
#CPP= g++
CPP= gcc
RM = rm -f
MV = mv

#Regel, damit die Sourcen auch gefunden werden
obj/%.o: src/%.c
	${CPP} $(CFLAGS)  -c $< -o $@

#Eigendliche Erstellungsregel
$(EXENAME): ${OBJECTS}
	${CPP} -o $(EXENAME) ${OBJECTS} ${CPPFLAGS} ${LIBS}

#Regel zur bereinugung
clean:
	$(RM) $(OBJECTS)
	$(RM) $(EXENAME)

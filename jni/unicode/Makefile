CC 		=	gcc

NAME		=	libunicode.a

SRC		=	utf8.c			\
			regress.c		

OBJ		=	utf8.o regress.o

.c.o		:	
			$(CC) -c $<

all		:	
			$(CC) -c $(SRC)
			ar rcs $(NAME) $(OBJ)
			ranlib $(NAME)

clean		:
			rm -rf *.o
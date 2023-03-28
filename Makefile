SRCS	= main.cpp

OBJS	= ${SRCS:.cpp=.o}

NAME	= containers

CXX		= c++

CXFLAGS	= -Wall -Wextra -Werror -std=c++98 -O2

RM		= rm -f

MAKE	= make

all:	${NAME}

bonus:	${NAME}

.cpp.o:
	${CXX} ${CXFLAGS} -c $< -o ${<:.cpp=.o}

${NAME}:	${OBJS}
	${CXX} ${CXFLAGS} -o ${NAME} ${OBJS}

clean:
	${RM} ${OBJS}

fclean: clean
	${RM} ${NAME}

re:	fclean all

.PHONY:	all bonus clean fclean re

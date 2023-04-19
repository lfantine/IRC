NAME	=	ircserv		
SRCS	=	srcs/main.cpp \
				srcs/User.cpp \
				srcs/Server.cpp \
				srcs/Reply.cpp \
				srcs/Channel.cpp\

OBJS	=$(SRCS:.cpp=.o)
FLAGS	= -Wextra -Wall -Werror -std=c++98
CC		= c++
INCS	=.

.cpp.o :
		@${CC} ${FLAGS} -c $< -o ${<:.cpp=.o} -I${INCS}

all		: $(NAME)

${NAME}	: ${OBJS}
		@${CC} ${FLAGS} ${OBJS} -o ${NAME}

clean:
	@echo delete : ${OBJS}
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all
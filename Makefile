# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lhabacuc <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/30 15:52:49 by lhabacuc          #+#    #+#              #
#    Updated: 2024/08/30 15:52:51 by lhabacuc         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #
LIBRARY = libhcloud.a

CC = gcc
CFLAGS = -Wall -Wextra -Werror -I.
AR = ar
ARFLAGS = rcs

SRC = cli_conf.c com_conf.c ser_conf.c
OBJ = $(SRC:.c=.o)

all: $(LIBRARY)

$(LIBRARY): $(OBJ)
	$(AR) $(ARFLAGS) $@ $(OBJ)

%.o: %.c hcloud.h
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(LIBRARY)

re: fclean all

.PHONY: all clean fclean re



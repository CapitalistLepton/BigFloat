CC?=gcc
LIBNAME=BigFloat
INC=-I./include
SRCDIR=src
OBJDIR=obj
FILES=$(wildcard $(SRCDIR)/*.c)
OBJS=$(FILES:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

all: clean setup $(OBJS)

setup:
	@mkdir -pv $(OBJDIR)

clean:
	@rm -rvf $(OBJDIR)/*

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(INC) -c $< -o $@
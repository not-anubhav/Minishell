OBJDIR = obj
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

msh.out: $(OBJS)
	gcc -o $@ $^

$(OBJDIR)/%.o: %.c | $(OBJDIR)
	gcc -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) *.out
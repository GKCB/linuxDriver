objs = a.o b.o c.o
dep_files := $(patsubst %, .%.d, $(objs))
dep_files := $(wildcard $(dep_files))

#很多警告都隐藏错误
CFLAGS = -Werror

test:$(objs)
	gcc -o test $^
	@echo dep_files = $(dep_files)

#不等于空就包含进来
ifneq ($(dep_files),)
include $(dep_files)
endif
	
%.o: %.c
	gcc -c -o $@ $< -MD -MF .$@.d

clean:
	rm *.o test

distclean:
	rm $(dep_files)
	
.PHONY: clean

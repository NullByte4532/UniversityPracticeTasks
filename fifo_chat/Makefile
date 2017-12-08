all: fifo_chat_processes.run fifo_chat_threads.run
fifo_chat_processes.run: fifo_chat_processes.c
		gcc -o fifo_chat_processes.run fifo_chat_processes.c
fifo_chat_threads.run: fifo_chat_threads.c
		gcc -o fifo_chat_threads.run fifo_chat_threads.c -lpthread
clean:
		rm *.run

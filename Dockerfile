FROM gcc:latest

WORKDIR /app

COPY . . 

RUN gcc -pthread -o main main.c 

CMD ["./main"]


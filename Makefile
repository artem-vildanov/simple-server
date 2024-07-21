CC = gcc
CFLAGS = -Wall -g -Iinclude
TARGET = server
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include
SRC = $(wildcard $(SRC_DIR)/*.c)  # Все .c файлы в src
OBJ = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC))  # Объектные файлы

# Создание директории для объектных файлов
$(shell mkdir -p $(OBJ_DIR))

# Правило по умолчанию
all: $(TARGET)

# Компиляция исполняемого файла
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

# Компиляция объектных файлов
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Правило для очистки
clean:
	rm -f $(TARGET) $(OBJ_DIR)/*.o

# Правило для выполнения
run: $(TARGET)
	./$(TARGET)

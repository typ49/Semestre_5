# Variables pour les compilateurs et les drapeaux
CC = gcc
CFLAGS = -Wall -Werror -Wextra -pthread
TARGET1 = TP5_1
TARGET2 = TP5_2

# Cibles par défaut
all: $(TARGET1) $(TARGET2)

$(TARGET1): $(TARGET1).c
	$(CC) $(CFLAGS) -o $(TARGET1) $(TARGET1).c

$(TARGET2): $(TARGET2).c
	$(CC) $(CFLAGS) -o $(TARGET2) $(TARGET2).c

# Cibles pour exécuter les programmes
run_$(TARGET1): $(TARGET1)
	@echo "Exécution de $(TARGET1):"
	./$(TARGET1)
	@echo ""

run_$(TARGET2): $(TARGET2)
	@echo "Exécution de $(TARGET2):"
	./$(TARGET2)
	@echo ""

# Cibles pour exécuter les programmes avec valgrind
valgrind_$(TARGET1): $(TARGET1)
	@echo "Exécution de $(TARGET1) avec valgrind:"
	valgrind ./$(TARGET1)
	@echo ""

valgrind_$(TARGET2): $(TARGET2)
	@echo "Exécution de $(TARGET2) avec valgrind:"
	valgrind ./$(TARGET2)
	@echo ""

valgrind: valgrind_$(TARGET1) valgrind_$(TARGET2)
# Cible pour nettoyer les fichiers objets
clean:
	rm -f $(TARGET1) $(TARGET2)

# Cible pour nettoyer complètement le répertoire
mrproper: clean
	rm -f *~ 

# Cible pour executer les programmes
run: run_$(TARGET1) run_$(TARGET2)

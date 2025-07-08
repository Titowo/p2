# Nombre del compilador C++ a usar
CXX = g++

# Flags de compilación:
# -std=c++17: Usar el estándar C++17
# -Wall -Wextra: Habilitar todas las advertencias (muy recomendado para buen código)
# -g: Incluir información de depuración (útil para gdb)
# -O2: Nivel de optimización 2 (descomentar para versiones finales, no para depuración)
CXXFLAGS = -std=c++17 -Wall -Wextra -g
# CXXFLAGS += -O2

# Directorios del proyecto
SRCDIR = src
DATADIR = data
BUILDDIR = build

# Archivos fuente C++
SOURCES = $(wildcard $(SRCDIR)/*.cpp)

# Archivos objeto correspondientes
OBJECTS = $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(filter-out $(SRCDIR)/LinkedList.tpp,$(SOURCES)))

# Nombre del ejecutable final
TARGET = $(BUILDDIR)/search_engine_project1

.PHONY: all clean run setup

all: setup $(TARGET)

setup:
	@mkdir -p $(BUILDDIR)
	@echo "Directorio '$(BUILDDIR)/' asegurado."

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@
	@echo "Construcción completada: $@"

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo "Compilado: $@"

run: all
	@echo "Ejecutando el programa..."
	./$(TARGET)

clean:
	@echo "Limpiando el directorio de construcción..."
	-DEL /S /Q "$(BUILDDIR)\*.*" > NUL 2>&1
	-RMDIR /S /Q "$(BUILDDIR)" > NUL 2>&1
	@echo "Limpieza completada."
	@rm '-p'
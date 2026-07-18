# Casa Nostra

Proyecto de árbol binario para la sucesión de una familia de la mafia italiana.

Realizado por [Jonner Paz](https://github.com/JonnerPaz) y [Vanessa Pérez](https://github.com/Vaneppv).

## Requisitos

- Compilador con soporte C++20 (g++ 10+, clang 10+, MSVC 2022+)
- Sistema operativo: Linux / Windows

## Compilación

Para compilar, copia y pega el siguiente commando en la consola:

```bash
make
```

Para compilar y correr el proyecto:

```bash
make run
```

## Uso

```bash
./bin/casa_nostra
```

El menú principal permite:

1. **Cargar datos desde CSV** — Lee `bin/datos.csv` y construye el árbol binario.
2. **Editar miembro** — Modifica los campos de un miembro por su ID.
3. **Ver línea de sucesión** — Muestra la línea de sucesión actual (solo vivos).

El programa se ejecuta hasta que el usuario selecciona "Salir".

## Estructura del proyecto

```
bin/
  datos.csv              # Datos de prueba
  casa_nostra            # Ejecutable
src/
  main.cpp               # Punto de entrada
  family_tree/
    family_tree.hpp      # Clase FamilyTree
    family_tree.cpp      # Implementación del árbol
  member/
    member.hpp           # Clase Member (nodo del árbol)
  orphan_node/
    orphan_node.hpp      # Nodo para la cola de huérfanos
    orphan_node.cpp
  orphan_queue/
    orphan_queue.hpp     # Cola FIFO para miembros sin jefe
    orphan_queue.cpp
  utils/
    constants.hpp        # Códigos de color ANSI
  presentation/
    main_menu.hpp        # Clase MainMenu
    main_menu.cpp        # Menú principal con opciones
```

## Formato del CSV

```csv
id,name,last_name,gender,age,id_boss,is_dead,in_jail,was_boss,is_boss
```

- `gender`: `H` (hombre) o `M` (mujer)
- `is_dead`, `in_jail`, `was_boss`, `is_boss`: `0` (no) o `1` (sí)
- `id_boss`: `0` si es la raíz del árbol

## Funcionalidades actuales

- Carga de datos desde CSV y construcción del árbol binario
- Edición de miembros (nombre, apellido, género, edad, estado)
- Reasignación automática de huérfanos al insertar su jefe
- Búsqueda de sucesores vivos (libres y en prisión)
- Mostrar línea de sucesión actual (solo miembros vivos)
- Asignación automática de jefe ante muerte, prisión o edad ≥ 70

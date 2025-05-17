
````markdown
# Evtor-C — Math Expression Evaluator in C

Evtor-C is a lightweight, token-based math expression evaluator written in pure C. It parses infix expressions, constructs a basic token tree,
and evaluates results recursively— all without external libraries or runtime dependencies.

---

## 🚀 Features

* **Tokenizer** for parsing math expressions into string tokens (`+`, `*`, `add(...)`, etc.)
* **Parser** to group tokens into logical structures, supporting nested expressions and function calls
* **Evaluator** for computing results from raw or grouped tokens using recursive logic
* **Dynamic Array System** (`ArrayInfo`) for handling variable-length arrays of pointers or values
* **Function Support**: `add`, `subtract`, `multiply`, `divide`, `sqrt`, `pow`, `log`, `abs`, `sin`, `cos`, `tan`, `max`, `min`, `floor`, `round`, `truncate`, `exp`, etc.
* **Tree Printing**: `PrintTokens` and `PrintTokenBlock` helper functions for debugging and visualization
* **Manual Memory Management** with explicit allocation and deallocation (`malloc`, `realloc`, `free`)

---

## 📦 Installation

1. Copy all source files (`List.c/h`, `Tokenizer.c/h`, `Parser.c/h`, `Evaluator.c/h`, `Strings.c/h`, `Main.c`, etc.) into your project directory.  
2. Compile with a C compiler. For example:
   ```sh
   gcc -std=c11 -O2 List.c Tokenizer.c Strings.c Parser.c Evaluator.c Main.c -o evtor
````

3. Run:

   ```sh
   ./evtor
   ```

---

## 💡 Quick Start

```c
#include <stdio.h>
#include "List.h"
#include "Tokenizer.h"
#include "Parser.h"
#include "Evaluator.h"

int main() {
    const char* expr = "add(2, multiply(3, (4+5)))";

    const ArrayInfo* tokens = Tokenize((char*)expr);
    ArrayInfo* parsed = Parse(tokens);

    if (parsed) {
        double result = Evaluate(parsed);
        printf("Result: %f\n", result);
        FreeArray(parsed);
    }

    FreeArray(tokens);
    return 0;
}
```

---

## 🔢 Example with Complex Expression

```c
const char* expr =
    "((((((2 + 0) + 3) * ((4 * 1) + (0 + 5))) + 2) / (((6 + 6) * 1) * (1 + 0 + 1))) + "
    "((100 / 5) - ((3 + 2) + 0))) + "
    "(((7 * 8) + (9 - 4 + 0)) * (((2 + 3 + 0) * 1) + 2)) - "
    "(((10 + 0 + 5) * 1) * ((6 - 0 - 1))) + "
    "((((50 + 50) / 2) * (((3 + 2) - 0) - (1 + 1)))) + 0 + 0 * 1234 + "
    "add(pow(20,sin((20/1*90)+add(60,70)+90)),20)";

const ArrayInfo* tokens = Tokenize((char*)expr);
ArrayInfo* parsed = Parse(tokens);
double result = Evaluate(parsed);
printf("Result: %f\n", result);
FreeArray(parsed);
FreeArray(tokens);
```

---

## 📘 More Examples

```c
// Logic and precedence
Tokenize("(15 > 1) & 7");
Tokenize("(2 + 3) * (4 + 5) ^ 2");
Tokenize("Add(1, Add(2, Add(3, Add(4, Add(5, 6)))))");
```

---

## 🛠️ Internals

* **ArrayInfo system**: Dynamic array with manual resizing
* **Function tree evaluation**: Recursively evaluates grouped or function tokens
* **Operator dispatch**: Operators mapped to enum values, interpreted at runtime

---

## 🔧 Build Notes

* Pure C11
* Single-threaded
* No external dependencies
* Built for learning, performance, and fun

---


```

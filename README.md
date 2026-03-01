# Acrostic — Lexical Analysis & Puzzle Engine
A high-performance C-based lexical processing system that manages double-linked word databases to generate, solve, and analyze acrostic puzzles from complex text corpora.


## Overview

Acrostic is a command-line utility engineered for sophisticated text analysis and dynamic puzzle generation. Built with a focus on memory efficiency and fast retrieval, the system processes large-scale text files into a structured double-linked list, enabling real-time lexical operations and the automated creation of acrostic structures based on varying difficulty parameters.


### Lexical Database Management
Double-linked list architecture for efficient bidirectional traversal
Word frequency tracking and unique character analysis
Automated timestamping for every lexical insertion via `ctime`
Memory-safe node deletion and list clearing protocols

### Text Processing & Normalization
Robust tokenization engine for sentence-to-word decomposition
Input sanitization with character-only validation and whitespace trimming
Case-insensitive processing with automated uppercase normalization
Bulk loading support for large text corpora (Reuters, Ulysses, etc.)

### Acrostic Generation Engine
Dynamic generation of vertical and horizontal word structures
Configurable difficulty levels (Easy/Hard) for puzzle complexity
Interactive display system for rendered acrostic puzzles
Export/Save functionality to persist generated puzzles to disk

### Command Interface
Intuitive prefix-based command system (e.g., `insert:`, `findfwd:`, `showrev:`)
Forward and reverse lookup capabilities for database auditing
Live memory management and exit state handling

### Core Implementation
C (Standard) -> Core logic and data structures
Double-Linked Lists -> Primary data architecture for lexical storage
`stdio.h` & `stdlib.h` -> I/O and dynamic memory management
`string.h` & `ctype.h` -> String manipulation and lexical filtering
`time.h` -> Temporal metadata for database entries

### Build & Execution
GCC -> Recommended compiler
Standard Makefile -> For automated compilation and linking
Text Assets -> Pre-configured text corpora for immediate database seeding

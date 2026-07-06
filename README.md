# Acrostic

A command-line text-processing program written in C. It reads large text files, breaks them into words, stores those words in a doubly linked list, and lets you search the list and build acrostics from it. Built as a university data structures project to practice linked lists, dynamic memory, and text parsing.

## What it does

- Loads and tokenizes large text files into individual words
- Cleans the input: removes punctuation, trims whitespace, and normalizes case
- Stores words in a doubly linked list, with metadata such as an insertion timestamp
- Uses a prefix-based command interface to insert words, search forward and in reverse, and display entries
- Builds acrostic structures from the stored words, with a selectable difficulty
- Saves generated results to a file

## Example texts

The `example texts/` folder contains sample files to load and test against — news articles, Wikipedia extracts, a novel, and a large list of song titles.

## Built with

- Standard C (`stdio.h`, `stdlib.h`, `string.h`, `ctype.h`, `time.h`)
- A doubly linked list as the core data structure

## Build and run

Compile with GCC:

```
gcc Acrostic.c -o acrostic
```

Run it, then load one of the files in `example texts/` and use the on-screen commands:

```
./acrostic
```

## Files

- `Acrostic.c` — the full program
- `example texts/` — sample text files for testing

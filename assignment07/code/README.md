# README.txt

## Task (f): Writer Mutual Exclusion

1. **Run Programs:**
   - Compile and run `repository.c`.
   - Compile and run `writer.c` in one terminal.
   - Compile and run `writer.c` in a second terminal.

2. **Expected Result:**
   - The second writer waits for the first writer to complete.

3. **Explanation:**
   - Semaphores ensure only one writer can write at a time.

## Task (g): Writer-Reader Mutual Exclusion

1. **Run Programs:**
   - Compile and run `repository.c`.
   - Compile and run `writer.c`.
   - Compile and run `reader.c` in another terminal.

2. **Expected Result:**
   - Reader waits for the writer to finish.

3. **Explanation:**
   - Semaphores block readers while a writer is writing.

## Task (h): Reader-Reader Concurrency

1. **Run Programs:**
   - Compile and run `repository.c`.
   - Compile and run `reader.c` in one terminal.
   - Compile and run `reader.c` in another terminal.

2. **Expected Result:**
   - Both readers read simultaneously.

3. **Explanation:**
   - Semaphores allow concurrent reading by multiple readers.


##FogOS Count Implementation

##Overview :

Uniq reads from either STDIN or a specified file. It compares each line with its immediately preceeding line 
and writes only the first occurence of these lines to standard output.

For eg, if a file contains the below content:

hello
hello
How are you?
How are you?
Are you ok?

uniq <file> will print:

hello
How are you?
Are you ok?

Build process :

Step 1 :
Run 'make qemu'

Step 2 :
Try running uniq with different options

#Default Implementation : reads from stdin, prints to stdout without adjacent duplicates.
uniq

#Default Implementation : reads from input file and prints to stdout without adjacent duplicates
uniq <file>

#prints number of duplicate lines, followed by the line.
uniq -c <file>
uniq --count <file>

#prints one instance of each duplicated line
uniq -d <file>
uniq --repeated <file>

#prints all duplicate lines
uniq -D <file>

#ignores case of characters while comparing two lines and prints the output
uniq -i <file>
uniq --ignore-case <file>

#prints only the uniq lines
uniq -u <file>

#ignores the first N characters while comparing and prints the output
uniq -s N <file>
uniq --skip-chars=N <file>

We can also combine different flags and the output will reflect the combined behaviour of both flags.
Please look at the below examples.

If the file has the content : hello
hello
How are you?
How are you?
Are you ok?

uniq -c -d <file> returns the output
1 hello
2 How are you?
1 Are you ok?

Similarly, we can combine other flags like
uniq -c -i <file>
uniq -c -s N <file>
uniq -i -d <file>
uniq -i -D <file>
uniq -s N -d <file>
uniq -s N -D <file>

except for some mutually contradictory flags like uniq -u -D <file>, uniq -u -d <file> etc. 
These will not return any output.

Similarly, the combination -c -D returns a message that "printing all duplicated lines and repeat counts is meaningless".

##Testing:

- emptyFile.txt
 
  - Expected Output : uniq emptyFile.txt will not print anything as the file is empty.

- test1.txt

  - uniq test1.txt 
	- Expected Output : 
            apple
            banana
            cherry
            date 
  - uniq -c test1.txt
       - Expected Output :
            2 apple
            3 banana
            2 cherry
            1 date

- test2.txt

  - uniq test2.txt
       - Expected Output :
            Apple
            apple
            banana
            Banana
            banana
            cherry
            Cherry
            cherry
            date
  - uniq -i test2.txt
       - Expected Output :
            Apple
            banana
            cherry
            date
  - uniq -s 1 test2.txt
       - Expected Output :
            Apple
            banana
            cherry
            date

- test3.txt
  - uniq -D test3.txt
       - Expected Output :
            apple
            apple
            apple
            Banana
            Banana
            Banana
            Banana
            Banana
            Peach
            Peach
            Peach
            Peach
  - uniq -c -d test3.txt
       - Expected Output :
            3 apple
            5 Banana
            4 Peach
  - uniq -u test3.txt
       - Expected Output :
                                            

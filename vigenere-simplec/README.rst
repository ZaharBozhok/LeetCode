Cyberhaven programming challenge
--------------------------------

Welcome to the Cyberhaven programming challenge!

The challenge is described in the file challenge2.rst. This file is encrypted,
and the encryption key has unfortunately been lost. However, we know that the
encryption key is the SHA1 hash of the special string ``53cr3t-i``, where ``i``
is replaced by a small number (32-bit maximum). We also know that the 
encryption key starts with ``d51ab``.  The following shell commands will 
recover the key for you and decrypt the challenge:

.. sourcecode:: sh

    # Compile the encryption program
    make

    # Find the utility to compute sha1 hashes
    sha1sum=/usr/bin/sha1sum
    [ -x "$sha1sum" ] || sha1sum=/usr/bin/shasum

    # Find the key
    key=$(i=0; \
        while ! echo -n "53cr3t-$i" | "$sha1sum" | grep -q "^d51ab"; do \
            i=$((i+1)); \
        done; \
        echo -n "53cr3t-$i" | "$sha1sum" | cut -d' ' -f1)

    # Decrypt the challenge
    ./vigenere decrypt $key < challenge2.rst

Unfortunately, the shellscript above is quite slow; it only tests a few hundred
keys per second. It might not even finish before your time to solve the
challenge runs out. Thus, we suggest you speed things up ...

Note that the decryption is just the first part of the challenge.
Once you decrypt the challenge2.rst file, you can proceed to solve it as well.

FAQ
___

#. The string ``d51ab`` is in hexadecimal. Please check that the
   output of the SHA1 function you use is in the expected encoding.

Instructions
------------

#. Send your solution no more than 3 hours after the challenge started.
   Please commit the result to the github provided repository.

#. Your code compiles cleanly by either typing 'make', using Visual Studio or
   or any other popular build system you prefer.

#. After submitting the code, you can optionally describe the design of your
   solution in the COMMENTS.txt file and commit it to the repository.
   Describe the data structures, the algorithms, and the tradeoffs you
   made. Alternatively, be prepared to discuss this live during the technical
   interview.

#. If you feel you cannot complete all functionality in the given time,
   then focus on just some of the operations, but do them well. In
   other words, depth is more important than breath. Don't worry if your
   solution is incomplete - please submit it anyway and describe what's
   left to be done in the COMMENTS.txt file and commit it to the repository.

#. We expect the challenge to be your own work. You may consult
   information sources such as programming language references, but you
   may not accept help from third parties.

#. You are allowed to use existing libraries. If these libraries are not
   widely known and available, please include them in your submission.


Hints
-----

Feel free to skip these if you don't need hints :)

#. You can run the bash script on Windows using WSL. Or you could use repl.it
   enviroment configured for you by github classroom. If you're not familar
   with bash, you can use this helpful resource to understand what individual
   commands or simple bash constructs do: https://explainshell.com/

#. At the heart of the script, there are the commands ``echo -n "53cr3t-$i" |
   $sha1sum | grep -q "^d51ab"``. What do the individual parts mean? What is the
   meaning of the pipe ``|`` symbol? If you don't know some of these commands,
   try ``sha1sum --help`` or ``man sha1sum`` (on Mac, ``man shasum``). You can
   also try to execute individual commands or the entire sequence.

#. It seems that the ``grep`` command filters out hashes that start with
   ``d51ab``. What happens if you change the condition, e.g. to ``"^d"`` or
   ``"^d5"``? Can you estimate how many hashes you need to check (on average),
   until you find one that has the required property?

#. Why do you think the shellscript is so slow?

#. What is the length of the key? What happens if you call ``./vigenere
   decrypt`` with an arbitrary key of the right length, that starts with
   ``d51ab``?

#. The Vigenère cypher was invented in the 16th century... it might be not very
   secure.

#. There might be more values of "i" that match the hash.

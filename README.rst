============
tidyc README
============

Introduction
============

This is some research, recommendations, and examples of how best to
use GNU indent for formating C code. It includes some tests done to
figure out the corner case behaviour of GNU indent to figure out how
best to configure/use it.

It has grown to include a `tidyc` script wrapper for `indent` that
addresses several issues indent has, including:

1. Adds support for using regexes for matching user-defined-types for
   the `-T` argument. This means you don't need to explicitly list
   every user-defined type for them to format correctly.

2. Makes `-fca` correctly preserve blank-line paragraphs inside
   "starred comments" with stars on the left hand side.

3. Makes `-nsc` behave as a consistent inverse of `-sc`, removing
   starring of comments without requiring `-fca`.

4. Makes `-ncdb` behave as a consistent inverse of `-cdb`, removing
   blank-line comment delimiters without requiring `-fca`, and also
   removing them from the top of comments.

5. Doesn't treat comments with stars and blank-line delimiters as
   block comments, allowing `-ncdb` to remove blank-line delimiters,
   `-nsc` to remove stars, and `-fca` to reformat them. This is a
   consequence of 3. and 4. above.

6. Always removes and never adds any trailing whitespace (indent adds
   a trailing space after blank lines in starred comments).

7. Better Doxygen comment support with optinal reformating for both
   JavaDoc and Qt style comments that doesn't break Qt style `/*!`
   delimiters, and correctly indents JavaDoc style `/**<` member
   comments.

8.  Adds `-fcd` and `-nfcd` options to enable/disable Doxygen comment
    formatting that includes reindenting, starring/unstarring with
    `-sc` or `-nsc`, rebracketing with `-cdb` or `-ncdb`, and
    reformatting with `-fca` and `-fc1`.

9. Adds `-dqt` and `-ndqt` to select Doxygen Qt or JavaDoc style
   comments. Comment delimiters will be changed to match the preferred
   style.

10. Add `-dab` and `-ndab` options to enable/disable Doxygen AUTOBRIEF
    support, removing or adding `\brief` tags to the first line of
    Doxygen comments.

11. Adds some convenient shortcuts like `-C` for `-fc1 -fca` and `-R`
    for `-sob --ignore-newlines`.

12. Has my preferred defaults which is mostly linux without tabs.
    
This is all implemented by doing pre and post processing with sed, so
is potentially vulnerable to tripping over comment delimiters inside
string constants etc.


Contents
========

=========== ======================================================
Name        Description
=========== ======================================================
README.rst  This file.
DESIGN.rst  Design details on testing done and results.
LICENSE     The GPLv3.
linux.args  Equivalent -linux args sorted for easy comparison.
kr.args     Equivalent -kr args sorted for easy comparison.
gnu.args    Equivalent -gnu args sorted for easy comparison.
orig.args   Equivalent -orig args sorted for easy comparison.
test.c      Source file for testing with.
tidyc       The script for tidying C source code.
=========== =======================================================


Credits
=======

Thank's to GNU for indent.


Conditions
==========

See LICENSE for details.


Install
=======

There is nothing to install here. Just copy tidyc somewhere on your
path.


Usage
=====

To tidy source code just run::

    $ tidyc *.[ch]

It has sensible defaults but can take any indent arguments with the
following additions;

   -L  Resets settings to `-linux`.
   -K  Resets settings to `-kr`.
   -G  Resets settings to `-gnu`.
   -B  Resets settings to `-orig`.
   -C  Reformat comments. Equivalent to `-fc1 -fca`.
   -R  Reformat lines breaks. Equivalent to `-sob --ignore-newlines`.

   -fcd, --format-doxygen-comments
       Enable Doxygen comment formatting. This will enable indenting,
       starring/unstaring based on -sc|-nsc, rebracketing with/without
       blank-line delimiters based on -cdb|-ncdb, and content
       reformating with -fca.

   -nfcd, --dont-format-doxygen-comments
       Disable Doxygen comment formatting. This will disable all
       reformatting of doxygen comments.

    -dqt, --doxygen-use-qt
       Select Doxygen Qt or JavaDoc style comments. This adjusts the
       Doxygen starting comment delimiter.
    
    -ndqt, --doxygen-use-javadoc
       Select Doxygen JavaDoc style comments. 

    -dab|--doxygen-autobrief
        Enable Doxygen autobrief support. This will remove \brief or
	@brief tags from the first line of Doxygen comments
    
    -ndab|--no-doxygen-autobrief
        Disable Doxygen autobrief support. This will add \brief or
	@brief tags depending on `-dqt' to the first line of Doxygen
	comments

    -T <regex>
        Like indent's `-T` except adds support for using extended
	regexes like `/ev_\w+/` to match types like `ev_event`. Note
	that `/w+_t/` is already included by default.

Support
=======

Email abo@minkirri.apana.org.au if you have any questions about this.

I'd be interested in any feedback anyone has, so please don't be shy.

Documentation
-------------

http://minkirri.apana.org.au/~abo/projects/tidyc/
  The project homepage.
  
https://github.com/dbaarda/tidyc
  The project github mirror.

Discussion
----------

Just email abo@minkirri.apana.org.au.

Reporting Problems
------------------

Just email abo@minkirria.apana.org.au.

Development
===========

Just edit it.

Design
======

See DESIGN for the design details of testing done and conclusions drawn.


Plans
=====

See TODO for future plans. Ha! There aren't any :-)


History
=======

This started after I added a "make tidy" target using indent to
https://github.com/dbaarda/LightLdapd. To work around indent needing
-T <mytype> arguments for every user defined type I added
post-processing with sed.

Then when I started working on https://github.com/librsync/librsync I
created the first version of tidy-c and added cmdline options. I
wanted to add a "make tidy" target to librsync but wasn't satisfied
with how it formatted comments. This was complicated by librsync using
doxygen comments which indent treats as "block comments".

At this point I decided to start a proper project for tracking this
work.

----

http://minkirri.apana.org.au/~abo/projects/tidyc/README.rst
$Id: README,v 69a01169087f 2014/11/27 00:12:55 abo $

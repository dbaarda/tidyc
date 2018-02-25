============
tidyc README
============

Introduction
============

This is some research, recommendations, and examples of how best to
use GNU indent for formating C code. It includes some tests done to
figure out the corner case behaviour of GNU indent to figure out how
best to configure/use it.

It has grown to include a ``tidyc`` script wrapper for ``indent`` that
addresses several issues indent has, including:

1. Adds support for using regexes for matching user-defined-types for
   the ``-T`` argument. This means you don't need to explicitly list
   every user-defined type for them to format correctly.

2. Makes ``-fca`` correctly preserve blank-line paragraphs inside
   "starred comments" with stars on the left hand side.

3. Makes ``-nsc`` behave as a consistent inverse of ``-sc``, removing
   starring of comments without requiring ``-fca``.

4. Makes ``-ncdb`` behave as a consistent inverse of ``-cdb``,
   removing blank-line comment delimiters without requiring ``-fca``,
   and also removing them from the top of comments.

5. Doesn't treat comments with stars and blank-line delimiters as
   block comments, allowing ``-ncdb`` to remove blank-line delimiters,
   ``-nsc`` to remove stars, and ``-fca`` to reformat them. This is a
   consequence of 3. and 4. above.

6. Always removes and never adds any trailing whitespace (indent adds
   a trailing space after blank lines in starred comments).

7. Better Doxygen comment support with optional reformating for both
   JavaDoc and Qt style comments that doesn't break Qt style ``/*!``
   delimiters, and correctly indents JavaDoc style ``/**<`` member
   comments.

8. Never reformats anything inside ``\code...\endcode`` blocks inside
   Doxygen comments.

9. Adds ``-fcd`` and ``-nfcd`` options to enable/disable Doxygen
   comment formatting that includes reindenting, starring/unstarring
   with ``-dsc`` or ``-ndsc``, rebracketing with ``-cdb`` or ``-ncdb``,
   and reformatting with ``-fca`` and ``-fc1``.

10. Adds ``-dsc`` and ``-ndsc`` options to add/remove starring for
    Doxygen comments independently of ``-sc`` or ``-nsc`` for non-Doxygen
    comments. Note ``-fcd`` must be set to enable these.

11. Adds ``-dqt`` and ``-ndqt`` to select Doxygen Qt or JavaDoc style
    comments. Comment delimiters will be changed to match the preferred
    style.

12. Add ``-dab`` and ``-ndab`` options to enable/disable Doxygen
    AUTOBRIEF support, removing or adding ``\brief`` tags to the first
    line of Doxygen comments.

13. Adds some convenient shortcuts like ``-C`` for ``-fc1 -fca -fcd``
    to reformat all comments and ``-R`` for ``-sob -nhnl`` to reformat
    all line breaks.

14. Has my preferred defaults which is mostly linux without tabs and
    with starred autobrief JavaDoc Doxygen comments.

15. Adds ``-iec`` and ``-niec`` options to enable/disable ignoring
    ``extern "C" {...}`` enclosing blocks for indenting.

16. Is more repeatable than indent, with repeated runs over the same
    input producing the same output (indent paragraph reformatting
    with ``-fca`` can produce different results in repeated runs).

Contents
========

This is a pretty random collection of little scripts and data files
accumulated during the research and development of the ``tidyc``
script

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
tidyc       The script for tidying C source code.
tidyc_test  The script for running all tests.
tests/*.c   Source files for testing with.
tests/*.out Expected output for each test.
=========== ======================================================


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

   -L  Resets settings to ``-linux``.
   -K  Resets settings to ``-kr``.
   -G  Resets settings to ``-gnu``.
   -B  Resets settings to ``-orig``.
   -C  Reformat comments. Equivalent to ``-fc1 -fca -fcd``.
   -R  Reformat line breaks. Equivalent to ``-sob -nhnl``.
   -fcd, --format-doxygen-comments
       Enable Doxygen comment formatting. This will enable indenting,
       starring/unstaring based on ``-dsc|-ndsc``, rebracketing with/without
       blank-line delimiters based on ``-cdb|-ncdb``, and content
       reformating with ``-fca``.
   -nfcd, --dont-format-doxygen-comments
       Disable Doxygen comment formatting. This will disable all
       reformatting of doxygen comments.
   -dsc, --star-doxygen-comments
       Enable starring of doxygen comments. This will add star prefixes
       to doxygen comment lines with ``-fcd``.
   -ndsc, --dont-star-doxygen-comments
       Disable starring of doxygen comments. This will remove star prefixes
       from doxygen comment lines with ``-fcd``.
   -dqt, --doxygen-use-qt
       Select Doxygen Qt or JavaDoc style comments. This adjusts the
       Doxygen starting comment delimiter.
   -ndqt, --doxygen-use-javadoc
       Select Doxygen JavaDoc style comments.
   -dab, --doxygen-autobrief
       Enable Doxygen autobrief support. This will remove ``\brief`` or
       ``@brief`` tags from the first line of Doxygen comments
   -ndab, --no-doxygen-autobrief
       Disable Doxygen autobrief support. This will add ``\brief`` or
       ``@brief`` tags depending on ``-dqt`` to the first line of Doxygen
       comments.
   -iec, --ignore-extern-c
       Enable ignoring enclosing ``extern "C" {...}`` blocks for
       indenting.
   -niec, --dont-ignore-extern-c
       Disable ignoring enclosing ``extern "C" {...}`` blocks.
   -T <regex>
       Like indent's ``-T`` except adds support for using extended
       regexes like ``/ev_\w+/`` to match types like ``ev_event``. Note
       that ``/w+_t/`` is already included by default.

The default arguments are equivalent to::

    $ tidyc -linux -nut -i4 -ppi2 -l120 -lc80 -fc1 -nfcd -dsc -ndqt -dab -iec -T /w+_t/

Issues
======

The following are the currently known issues;

1. This is all implemented by doing pre and post processing with sed, so
   is potentially vulnerable to tripping over comment delimiters inside
   string constants etc.

2. The contents of ``\code...\endcode`` blocks in Doxygen comments are
   never formatted in any way. This includes starring, unstarring,
   indenting or reformatting. This can result in strange different
   starred/unstarred or indented/unindented sections in the middle of
   Doxygen comments. These code blocks must be manually edited to match
   the surrounding comment. After this reformatting will produce the
   correct result.

3. Incorrectly indented ``\code...\endcode`` blocks in Doxygen comments
   will mess with the indenting of the rest of the comment after the
   code block. Manually correcting the indenting of the code block and
   reformatting will produce the correct result.

4. Paragraphs with embedded ``\code...\endcode`` blocks will mess with
   paragraph reformatting, resulting in long lines that include the
   code block. Don't do that.

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

============
TidyC DESIGN
============

.. contents:: **Contents**

Introduction
============

This describes the inspirations, philosophy, and architecture of the
tidyc project.

It's written by the authors for other developers who might be
interested.

The tidyc project is a collection of research and recommendations on
using GNU indent for formatting C code, including a shell script tidyc
wrapper that improves/extends indent using pre/post-processing using
sed.

Design Details
==============

Inspirations and Origins
------------------------

Preliminary Research
--------------------

There are four different kinds of changes indent can make to comments;

  1. reindenting - adjusting the indent level.

  2. starring/unstarring - adding/removing -sc star prefixes.

  3. rebracketing - moving -cdb comment delimiters in/out of their own
     blank lines. 

  3. reformating - adjusting/rewrapping comment contents.

Indent sees comments are of 3 different types;

  1. block comments starting with `/*[*=_-]`. These are never modified
     in any way.

  2. block comments starting with `/*\n *`. These will be reindented,
     but the contents are never reformatted.

  3. non-block comments. These can be indented, restarred, and
     rebracketed reformated if you specify -fca.

Additionally, there is a difference between;

  1. outside comments that start on lines without any code outside any
     struct/func/etc brackets.

  2. inside comments that start on lines without any code inside any
     struct/func/etc brackets.

  3. code-line comments that start on lines after code. These are
     expected to have comment text on the same line as comment
     delimiters, even if you use -cdb.
     
  4. column1 comments that start in column 1. Without -fc1 these are
     treated like type 1 block comments and never modified in any way.

The options -sc will "star comment lines" and -cdb will put "comment
delimiters on blank lines". Note that this means -sc -sdb will
transform all line-comments into type 2 block comments, preventing
future reformatting. This does not affect code-line comments because
they are never -cdb formatted.

After testing the following observations about indent's behavour
around comments were noted;

  * Indent is very unforgiving and can produce strange results for bad
    C syntax.

  * Block comments are NEVER reformated, restarred, or unstarred.

  * Block '/**' comments are never reindented, even code-line
    comments, which can result in them moving to a following line if
    the reformated code is too long. However '/*\n *' block comments
    will be reindented for code-lines or inside comments.

  * All outside comments have their indentation preserved. However,
    lines within non-block comments can be reindented to align them.

  * All non-block inside comments are always reindented unless they
    are column 1 comments without -fc1.

  * With -sc restarring will be done for non-block comments (with -fc1
    for column 1 comments), but -nsc will not do unstarring unless -fca
    is also given to reformat the whole comment.

  * Without -fc1 comments starting in column 1 are never changed in
    any way, even with -fca. With -fc1 without -fca they will be
    indented or restarred.

  * Without -fca no reformatting or unstarring is done. The only
    comment changes will be reindenting, and restarring.

  * Using --ignore-newlines has no effect on comment formatting. With
    -fca existing line breaks are ignored and paragraphs reformatted.
    Without -fca no paragraph reformatting is done.

  * When reformatting blank lines in comments are seen as paragraph
    markers and will be preserved. However, blank lines with "*"
    prefixes are not seen as paragraph markers, so paragraphs are lost
    when reformatting starred comments, even when using '-sc'. Using
    '-sc' on unstarred paragraphs will correctly reformat them and
    then add stars, which means followup runs will then loose the
    paragraphs.

  * Doxygen supports '/**' or '/*!' starting comments. '/**' is a
    block comment, '/*!' is not. In particular '/**>' attribute or
    argument comments will not be realigned, but '/*!>' comments will.

  * Doxygen '/*!' comments will be changed to '/* !' when reformatted,
    or even '/*\n !' when using -cdb.

  * Using -cdb will always rebracket comment delimiters out for all
    non-code-line non-block comments, but -ncdb will not move them
    back correctly. If not using -sc and -fca it will not add stars to
    the dropped first comment line, resulting in mixed
    starred/unstarred lines for starred comment blocks.

  * Using -ncdb without -fca will not rebracked comment delimiters
    back in. With -fca it will "suck up" trailing comment delimiters,
    but will not "suck down" leading comment delimiters.

  * Using -cdb with '/*!' doxygen comments will move the '!' onto the
    next line.

  * All code-line non-block comments will never have "comment
    delimiters on blank" formatting even with -cdb. Any trailing '*/'
    will be sucked up, but leading '\*'s will not be pulled down.

  * Using -cdb and -sc together will turn non-block non-code-line
    comments into block comments. If not using -fca it will also
    "double star" existing starred blocks that don't have -cdb
    comments.

  * Indent will use tabs wherever they fit unless you use -nut.

This suggests the following;

  * Unless your coding style dictates tabs, always use -nut.

  * Unless you really do want comments in column 1 never modified,
    always use -fc1. Use block comments to indicate comments that
    should not be modified.

  * Unusual block comments using alternatives to '*' leading
    characters like '|' should be replaced with '*' or converted to
    /*= block comments.

  * If using -nsc, you will need to unstar comments manually. Indent
    will not do it for you.

  * For reformatting comments and preserving paragraphs, use -fc1 -fca
    and pre-strip leading '*'s off non-block comments. If you want
    leading stars -sc will add them back.

  * Using -cdb and -sc together makes all non-code-line comments
    into block comments and can result in double-starring. Strip
    leading '*' from all non-block comments to prevent this.

  * If you want comments to be completely unmodifed, including fixed
    indents, use '/**' block comments. If you want them unformatable
    but indentable, use '/*\n *' block comments.

  * If you want Doxygen comments to be unreformatable and unindentable
    use '/**' block comments. If you want them indentable use '/*!'
    non-block comments. However '/*!' comments will also get
    reformatted when using -fca into '/* !' and will need
    post-processing. If using -cdb non-code-line comments become '/*\n
    *!', which is harder to post-process back with sed.

  * If you want to once-off reformat block comments, strip leading
    '*'s and replace '/**' with '/*?' and '/*\s$' with '/*%'. Use
    -ncdb and -sc or -nsc as normal. Post process replacing '/* ?'
    with '/**' and '/* %' with '/*\n'. If you use -nsc but want block
    comments starred, do that in post-processing too, adding '*
    between '/**' and '*/'

There are several possible comment formatting options;


1) Use the different comment types deliberately for their behaviour.
So use `/**` block comments for immutable comments, `/*\n *` comments
for indentable but unreformatable comments, and other comments for
reformatable comments. This means you cannot use -sc and -cdb
together. Using -sc will require pre-processing to strip stars off
non-block comments before -fca (arguably an indent bug). For doxygen
`/*!` can be used for reformatable doxygen comments, but will require
post-processing to change `/* !` back to `/*!`.

Doxygen

Desired behaviour;

Dont treat '/*\n *' comments as block comments. Because -sc -cdb
convert all comments into this form, we want to be able to use -nsc
and -ncdb to reverse this.

Make -fca correctly reformat starred comments. This requires manually
unstarring and relying on -sc to add them back if needed. This also
makes '/*\n *' comments non-block comments.

Make -nsc correctly reverse -sc. This means always unstar non-block
comments, even without -fca.

Make -ncdb reverse -cdb correctly. This means "suck in" the comment
delimiters both before and after the comment, even without -fca.

Always fix doxygen `/*!` comments after -fca and -cdb.

Always reindent code-line comments, even `/**` block comments? Or
maybe only just `/**<` doxygen code-line comments? Note this probably
means we also reformat them with -fca.

Always suck in comment delimiters for code-line comments even without
-fca.

Add `-fcd|--format-doxygen-comments` and
`-nfcd|--dont-format-doxygen-comments` options to optionally format
Doxygen comments, regardless of JavaDoc or Qt Doxygen format.

Add `-djd|--doxygen-use-javadoc` and `-ndjd|--doxygen-use-qt` options
for JavaDoc/Qt Doxygen formats. Have these change between `/**` and
`/*!` formats? What about `\brief` vs `@brief`?

Add `-dab|--doxygen-autobrief` and `-ndab|--no-doxygen-autobrief` to
convert between doxygen AUTOBRIEF and `\brief`?

Add -FCA option to reformat block comments?

Add -SC and -NSC option to star/unstar block comments?

Add -CDB and -NCDB option to rebracket block comments?


Design Philosophy
-----------------

Try to keep things simple.

General Architecture
--------------------

Use indent together with a bit of sed to address deficiencies or
quirks of how indent works.


References
==========

* http://related.com/url


----

http://minkirri.apana.org.au/~abo/tidyc/DESIGN.rst
$Id: DESIGN,v 65b64de6b1e1 2014/01/20 02:32:20 abo $

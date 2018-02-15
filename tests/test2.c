/** Doxygen javadoc style comment with stars and code.
 *
 * This is a sample file for testing formatting of \code...\endcode blocks in Doxygen comments. This is a very long line that goes well past the line length.
 *
 * Example: \code
 *   if (something > 1) {
 *     do_something()
 *   }
 * \endcode
 *
 * And it has another paragraph
 * that is too short.
 *
 * One liner code that would otherwise be wrapped because it's on the end of a very long line: \code a = 10; b = 5; \endcode
 *
 * With another
 * short para before an unindented code block.
 *
\code
if (a==b) {
  c=1;
} \endcode
 *
 * Final
 * paragraph.
*/

   /*! Here is a well formed indented comment with a well formed code block.
    *
    * Example: \code
    *   if (a==b) {
    *     c = 1;
    *   }
    * \endcode
    *
    * With a following paragraph. */

           /*!
	     here is a wierdly indented quote that goes too far   over two lines.

	     Example: \code
	       if (a>b) {
	         c=1;
	       }
	     \endcode

	     With another
	     short paragraph.

\code
if (a=b) {
  c=1;
}
\endcode

	    And a final para.
	    */

int bar(int a)
{
/*! doxygen comment in function, rather long and should be a comment that wraps because its so long.

   Example: \code
     if (a > b) {
       c = 1;
     }
   \endcode

    It also has
 another paragraph.
*/
     return b;
}

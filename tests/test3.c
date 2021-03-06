/** \file test3.c
 * Source testing -iec and doxygen structural commands.
 * 
 * Structural commands should work with adding/removing brief tags.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** \struct foo
 * \brief A struct.
 *
 * With struct details.
 */
typedef struct foo {
int a; /**< a short comment. */
int b; /**< A very long comment that goes way past the end of the max line length.*/
int c; /**< A very long comment that goes way past the end of the max line length.
        *
        * With another paragraph
        * that is too short. */
} foo_t;

/** A func.
 *
 * With func details.
 *
 * @param a [in] an int for a.
 *
 * @returns an int for something.
 */
int bar(int a)
{
     int b; /* not doxygen */

 /* Also not doxygen, but rather long and should be a comment that wraps because its so long.

    It also has another paragraph.
  */
     return b;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

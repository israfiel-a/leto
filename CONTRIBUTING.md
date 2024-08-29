These guidelines are subject to change any time, so make sure to check them often if you are planning to commit code!

## Code Styling
You **must** use the ClangFormat template included with the program. Any changes to said template must be run by a maintainers and done via pull request.

## Naming Conventions
### Headers and Source Files
All header files must include the following @file declaration at the top:

```c
/**
 * @file [HEADER_NAME].h
 * @author [AUTHOR NAME / PSUED] ([AUTHOR GITHUB LINK])
 * @brief [BRIEF DEFINITION]
 * @date [FILE CREATION DATE]
 *
 * @copyright (c) 2024 - the Leto Team
 * This document is under the GNU Affero General Public License v3.0. It
 * can be modified and distributed (commercially or otherwise) freely, and
 * can be used privately and within patents. No liability or warranty is
 * guaranteed. However, on use, the user must state license and copyright,
 * any changes made, and disclose the source of the document. For more
 * information see the @file LICENSE.md file included with this
 * distribution of the Leto source code.
 */
```

If you see a file without this declaration, please rectify it, filling in all information as needed. If short on time or patience, simply open an Issue and maintainers will get to it soon.

Similarly, all source files must include the following @file declaration:

```c
/**
 * @file [HEADER_NAME].h
 * @author [AUTHOR NAME / PSUED] ([AUTHOR GITHUB LINK])
 * @brief [BRIEF DEFINITION]
 * @date [FILE CREATION DATE]
 *
 * @copyright (c) 2024 - the Leto Team
 * This document is under the GNU Affero General Public License v3.0. It
 * can be modified and distributed (commercially or otherwise) freely, and
 * can be used privately and within patents. No liability or warranty is
 * guaranteed. However, on use, the user must state license and copyright,
 * any changes made, and disclose the source of the document. For more
 * information see the @file LICENSE.md file included with this
 * distribution of the Leto source code.
 */
```

Beyond this, all headers must be, number one, guarded, and number two, their guard must follow this convention:

```c
#ifndef __LETO__[FILE_BASENAME]__
```

### Functions
All functions must be named both succinctly and descriptively. Any functions that are extensions / overloads of another should have a single letter descriptor of its change.

```c
// Original function
FILE* OpenFile(file_mode_t mode, const char* path);
// Function with (F)ormatted path
FILE* OpenFileF(file_mode_t mode, const char* path_format, ...);
```

### Documentation
Documentation is a big part of developing Leto. All values, public and private, must follow a strict convention. Some are simple, like the one for file includes:

```c
#include <file.h> // [PURPOSE]
```

Or slightly more complex, like the one for variables:

```c
/**
 * @brief [PURPOSE]
*/
int var = 0;
```

And more complex yet, functions:

```c
/**
 * DESCRIPTION
 *
 * @brief [PURPOSE & CAVEATS]
 *
 * PARAMETERS
 *
 * @param [PARAM] [PURPOSE]
 * or
 * Nothing of note.
 *
 * RETURN VALUE
 *
 * @return [VALUE & PURPOSE]
 * or
 * Nothing of note.
 *
 * WARNINGS
 *
 * @warning [WARNING] -- [INFO]
 * or
 * Nothing of note.
 *
 * ERRORS
 *
 * @exception [ERROR] -- [INFO]
 * or
 * Nothing of note.
 *
 */
void func(void);
```

Documentation transparency is also important, so make sure that if your function calls any subroutines that could potentially throw an error unprevented by your function, note it! Doing so works as follows:

```c
/**
 * -- excerpt
 * @note For possible [warnings / errors] unhandled by this function, see [SUBROUTINE_LIST].
 * -- excerpt
```

If you have anything left unimplemented in your function that you want implemented later, add within the DESCRIPTION block:

```c
/**
 * -- excerpt
 * @brief [PURPOSE & CAVEATS]
 * @todo [INFO]
 * -- excerpt
*/
```

Ideally, all of these will be removed by the time code is pushed to the main branch. However, if any slip through the cracks and maintainers do not already have an issue open, open an issue to let us know.
